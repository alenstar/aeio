// server
#include <ae.h>
#include <anet.h>
#include <errno.h>
#include <logdef.h>
#include <stdio.h>

/* Anti-warning macro... */
#define UNUSED(V) ((void)V)
#define NET_IP_STR_LEN 46 /* INET6_ADDRSTRLEN is 46, but we need to be sure */
#define NET_PEER_ID_LEN (NET_IP_STR_LEN + 32) /* Must be enough for ip:port */

static char neterr[ANET_ERR_LEN] = {0x00};

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
    write(fd, buf, nread);
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
        write(fd, "hello world !", 14);
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

int main(int argc, const char **argv) {
    aeEventLoop *loop = aeCreateEventLoop(64);
    if (!loop) {
        LOGE("aeCreateEventLoop failure");
        return 1;
    }
    char neterr[ANET_ERR_LEN] = {0x00};
    int sfd = anetTcpServer(neterr, 8888, NULL, 4);
    if (sfd != ANET_ERR) {
        anetNonBlock(NULL, sfd);
        if (aeCreateFileEvent(loop, sfd, AE_READABLE, onaccept, NULL)) {
            LOGE("Unrecoverable errror createing server file event");
        }
    } else if (errno == EAFNOSUPPORT) {
        LOGE("Not listening to IPv4: unsupported");
    }
    aeMain(loop);

    aeDeleteEventLoop(loop);
    return 0;
}
