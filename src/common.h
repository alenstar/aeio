#ifndef COMMON__H
#define COMMON__H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef WIN32
typedef int SOCKET;
#endif
#define TIO_BASE_FIELDS SOCKET fd;void* userdata

#define TIO_OK 0
#define TIO_SUCCESSS TIO_OK

#ifdef WIN32
#define SELSECT_IONUM 60
#else
#define SELSECT_IONUM 256
#endif

#endif // COMMON__H