/*
 * logging.h
 *
 *  Created on: Sep 7, 2016
 *      Author: krcka
 */

#ifndef INC_LOGGING_H_
#define INC_LOGGING_H_

#include "config.h"

#ifdef TRACE
#define TRACE_PRINT(fmt, ...) \
        do { printf("TRACE: %s(): " fmt "\r\n", __func__, \
                               ## __VA_ARGS__); } while (0)
#else
#define TRACE_PRINT(fmt, ...) \
        do {  } while (0)
#endif


#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
        do { printf("DBG: %s(): " fmt "\r\n", __func__, \
                               ## __VA_ARGS__); } while (0)
#else
#define DEBUG_PRINT(fmt, ...) \
        do { } while (0)
#endif


#define WARN_PRINT(fmt, ...) \
        do { printf("WARN: %s(): " fmt "\r\n", __func__, \
                                ## __VA_ARGS__); } while (0)

#define ERR_PRINT(fmt, ...) \
        do { printf("ERR: %s(): " fmt "\r\n", __func__, \
                                ## __VA_ARGS__); } while (0)

#define FATAL_PRINT(fmt, ...) \
        do { printf("FATAL: %s(): " fmt "\r\n", __func__, \
                                ## __VA_ARGS__); } while (0)

#endif /* INC_LOGGING_H_ */

