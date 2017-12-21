// server
#include <ae.h>
#include <anet.h>
#include <errno.h>
#include <logdef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>

#include <lfq.h>

/* Anti-warning macro... */
#define UNUSED(V) ((void)V)
#define NET_IP_STR_LEN 46 /* INET6_ADDRSTRLEN is 46, but we need to be sure */
#define NET_PEER_ID_LEN (NET_IP_STR_LEN + 32) /* Must be enough for ip:port */

static char neterr[ANET_ERR_LEN] = {0x00};

typedef struct {
    struct lfq_ctx mq;
    int port;
    int fd;
} io_ctx_t;

int tcp_server(int* port) {
    int sock = -1;
    struct sockaddr_in server;
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        perror("Could not create socket");
        return -1;
    }
    // setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    // server.sin_addr.s_addr = INADDR_ANY;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(0);

    //Bind
    if( bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        close(sock);
        return -1;
    }

    //Listen
    if(listen(sock, 1) < 0) {
        close(sock);
        return -1;
    }
    socklen_t len = sizeof(server);
    if (getsockname(sock, (struct sockaddr*) &server, &len) != 0) {
        perror("getsockname()");
        return -1;
    }
    *port = ntohs(server.sin_port);

    return sock;
}
int tcp_client(int port) {
    int sock;
    struct sockaddr_in server;
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        perror("Could not create socket");
        return -1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( port);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect failed. Error");
        return -1;
    }
    return sock;
}

pthread_t thread_create(void*(*fn)(void*), void* args, int detach) {
    pthread_t t;
    if(pthread_create(&t, NULL, fn, args) != 0){
        return -1;
    }
    if(detach) {
        pthread_detach(t);
    }
    return t;
}

void onevent(int fd, void *data, int mask) {
    LOGD("onevent: %d", fd);
    return;
}

/* Remove the specified client from global lists where the client could
 * be referenced, not including the Pub/Sub channels.
 * This is used by freeClient() and replicationCacheMaster(). */
void unlinkFileEvent(aeEventLoop *loop, int fd) {

    /* Certain operations must be done only if the client has an active socket.
     * If the client was already unlinked or if it's a "fake client" the
     * fd is already set to -1. */
    if (fd != -1) {
        /* Unregister async I/O handlers and close the socket. */
        aeDeleteFileEvent(loop, fd, AE_READABLE);
        aeDeleteFileEvent(loop, fd, AE_WRITABLE);
        close(fd);
        fd = -1;
    }
}

void readQueryFromClient(aeEventLoop *loop, int fd, void *privdata, int mask) {
    // LOGD("readQueryFromClient");
    UNUSED(privdata);
    int nread, readlen;
    size_t qblen;
    UNUSED(mask);
    char buf[1024];
    nread = read(fd, buf, 1024);
    if (nread == -1) {
        if (errno == EAGAIN) {
            return;
        } else {
            LOGE("Reading from client: %s", strerror(errno));
            unlinkFileEvent(loop, fd);
            return;
        }
    } else if (nread == 0) {
        LOGI("Client closed connection");
        unlinkFileEvent(loop, fd);
        return;
    }
    // write(fd, buf, nread);
    LOGD("readQueryFromClient:%d", nread);
}

// void onaccept(int fd, void *data, int mask) {
//    LOGD("onaccept: %d", fd);
//    return;
//}

#define MAX_ACCEPTS_PER_CALL 1000
static void acceptCommonHandler(aeEventLoop *loop, int fd, int flags,
                                char *ip) {
    if (fd != -1) {
        anetNonBlock(NULL, fd);
        anetEnableTcpNoDelay(NULL, fd);
        // anetKeepAlive(NULL, fd, server.tcpkeepalive);
        if (aeCreateFileEvent(loop, fd, AE_READABLE, readQueryFromClient,
                              NULL) == AE_ERR) {
            close(fd);
            LOGE("not create file event");
            return;
        }
        // write(fd, "hello world !", 14);
    }
}

void onaccept(aeEventLoop *loop, int fd, void *privdata, int mask) {
    int cport, cfd, max = MAX_ACCEPTS_PER_CALL;
    char cip[NET_IP_STR_LEN];
    // UNUSED(loop);
    UNUSED(mask);
    UNUSED(privdata);

    while (max--) {
        cfd = anetTcpAccept(neterr, fd, cip, sizeof(cip), &cport);
        if (cfd == ANET_ERR) {
            if (errno != EWOULDBLOCK)
                LOGE("Accepting client connection: %s", neterr);
            return;
        }
        LOGD("Accepted %s:%d", cip, cport);
        acceptCommonHandler(loop, cfd, 0, cip);
    }
}

void io_ctx_deinit(io_ctx_t* ctx) {
    lfq_clean(&ctx->mq);
}
int io_ctx_init(io_ctx_t* ctx) {
    lfq_init(&ctx->mq, 4);
    return 0;
}
int io_notify(io_ctx_t* ctx, char code){
    int n = write(ctx->fd, &code, 1);
    if (n != 1) {
        perror("write failed");
        return -1;
    }
    return 0;
}
void* notify_init(void* args) {
    io_ctx_t* ctx= (io_ctx_t*)args;
    int fd = tcp_client(ctx->port);
    ctx->fd = fd;
    if (fd < 0) {
        LOGD("tcp_client connect failed");
        return NULL;
    }

    for(;;) {
        if (io_notify(ctx, 1) != 0){
            perror("write failed");
            break;
        }
        usleep(10);
    }

    return NULL;
}

int main(int argc, const char **argv) {
    io_ctx_t ctx;
    aeEventLoop *loop = aeCreateEventLoop(64);
    if (!loop) {
        LOGE("aeCreateEventLoop failure");
        return 1;
    }
    char neterr[ANET_ERR_LEN] = {0x00};
    int sfd = anetTcpServer(neterr, 8888, "127.0.0.1", 4);
    if (sfd != ANET_ERR) {
        anetNonBlock(NULL, sfd);
        if (aeCreateFileEvent(loop, sfd, AE_READABLE, onaccept, NULL)) {
            LOGE("Unrecoverable errror createing server file event");
        }
    } else if (errno == EAFNOSUPPORT) {
        LOGE("Not listening to IPv4: unsupported");
    }

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getsockname(sfd, (struct sockaddr*) &addr, &len) != 0) {
        perror("getsockname()");
        return -1;
    }
    ctx.port = ntohs(addr.sin_port);
    if(thread_create(notify_init, &ctx, 1) == -1) {
        aeDeleteEventLoop(loop);
        LOGD("thread_create failed");
        return -1;
    }

    aeMain(loop);

    aeDeleteEventLoop(loop);
    return 0;
}
