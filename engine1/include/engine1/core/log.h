#ifndef ENGINE1_CORE_LOG_H
#define ENGINE1_CORE_LOG_H

#include <engine1/platform.h>
#include <stdint.h>

#ifndef LOG_CONTEXT
#define LOG_CONTEXT "DEFAULT"
#endif

typedef enum E1LogLevel {
    E1_LOG_CRIT  = 0,
    E1_LOG_ERR   = 1,
    E1_LOG_WARN  = 2,
    E1_LOG_INFO  = 3,
    E1_LOG_DBG   = 4,
    E1_LOG_TRACE = 5
} E1LogLevel;

void log_out(const char* const message, uint8_t log_level);

#if LOG_LEVEL >= 5
#define LOG_TRACE(msg) log_out("(TRACE){"LOG_CONTEXT"}:" msg, E1_LOG_TRACE)
#else
#define LOG_TRACE(msg)
#endif

#if LOG_LEVEL >= 4
#define LOG_DEBUG(msg) log_out("(TRACE){"LOG_CONTEXT"}:" msg, E1_LOG_DBG)
#else
#define LOG_DEBUG(msg)
#endif

#if LOG_LEVEL >= 3
#define LOG_INFO(msg) log_out("(INFO){"LOG_CONTEXT"}:" msg, E1_LOG_INFO)
#else
#define LOG_INFO(msg)
#endif

#if LOG_LEVEL >= 2
#define LOG_WARN(msg) log_out("(WARN){"LOG_CONTEXT"}:" msg, E1_LOG_WARN)
#else
#define LOG_WARN(msg)
#endif

#if LOG_LEVEL >= 1
#define LOG_ERROR(msg) log_out("(ERROR){"LOG_CONTEXT"}:" msg, E1_LOG_ERR)
#else
#define LOG_ERROR(msg)
#endif

#if LOG_LEVEL >= 0
#define LOG_CRIT(msg) log_out("(CRITICAL){"LOG_CONTEXT"}:" msg, E1_LOG_CRIT)
#else
#define LOG_CRIT(msg)
#endif

#endif
