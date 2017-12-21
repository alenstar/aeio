//
// Created by jiangwei on 12/21/17.
//

#ifndef LCAP_TLOG_H
#define LCAP_TLOG_H

#include <sys/time.h>
#include <lfq.h>

typedef struct {
    pid_t pid;
    thread_t tid;
    struct timeval tv;
    char* tag;
    char* filename;
    int line;
    int level;
}tlog_info_t;

void tlog_output(tlog_info_t* info, const char* log);
int tlog_init(const char* pathname, int async);
void tlog_deinit();
void tlog_set_level(int level);

#endif //LCAP_TLOG_H
