//
// Created by vance on 2016/8/6.
//

#ifndef NATIVEIMAGEMERGE_INC_H
#define NATIVEIMAGEMERGE_INC_H

#include <stdio.h>
#include <vector>

#define DEBUG_TAG "ImageCompare"

#define _DEBUG_LOG
#include "log.h"

#define _DEBUG_PERFORMANCE

#ifdef _DEBUG_PERFORMANCE

#include <time.h>
#define TRACE_INIT(tag) \
            char* ___trace_tag = tag; \
            LOGD(___trace_tag, "==> Trace init"); \
            clock_t ___trace_total_time = 0; \
            clock_t ___trace_begin_time = 0; \
            clock_t ___trace_end_time = 0;

#define TRACE_BEGIN \
            ___trace_begin_time = clock();

#define TRACE_END(msg) \
            ___trace_end_time = clock(); \
            ___trace_total_time += (___trace_end_time - ___trace_begin_time); \
            LOGD(___trace_tag, "[%s]# %ldms", msg, ((___trace_end_time - ___trace_begin_time) / 1000));

#define TRACE_FINISH \
            LOGD(___trace_tag, "==> finished.# %ldms", (___trace_total_time / 1000));

#else

#define TRACE_INIT(tag)
#define TRACE_BEGIN
#define TRACE_END(msg)
#define TRACE_FINISH

#endif

// values
#define MAX_INT_VALUE 2147483647
#define MIN_INT_VALUE -2147483648

#define INVALID_DISTANCE MAX_INT_VALUE

#endif //NATIVEIMAGEMERGE_INC_H
