#include <engine1/core/log.h>
#include <engine1/core/assert.h>
#include <stdio.h>

#define ANSII_RESET  "\033[0m"
#define ANSII_DRED   "\033[38;5;1m"
#define ANSII_RED    "\033[38;5;9m"
#define ANSII_YELLOW "\033[38;5;11m"
#define ANSII_WHITE  "\033[38;5;15m"
#define ANSII_GREEN  "\033[38;5;10m"
#define ANSII_PURPLE "\033[38;5;13m"

void log_out(const char* const message, const uint8_t log_level) {
    ASSERTF(log_level <= E1_LOG_TRACE, "Log level exceded 5, given: %u", log_level);

#ifdef E1_PLAT_LINUX
    switch (log_level) {
        case E1_LOG_CRIT:
            fprintf(stderr, ANSII_DRED "%s\n" ANSII_RESET, message);
            break;

        case E1_LOG_ERR:
            fprintf(stderr, ANSII_RED "%s\n" ANSII_RESET, message);
            break;

        case E1_LOG_WARN:
            fprintf(stderr, ANSII_YELLOW "%s\n" ANSII_RESET, message);
            break;

        case E1_LOG_INFO:
            fprintf(stderr, ANSII_WHITE "%s\n" ANSII_RESET, message);
            break;

        case E1_LOG_DBG:
            fprintf(stderr, ANSII_GREEN "%s\n" ANSII_RESET, message);
            break;

        case E1_LOG_TRACE:
            fprintf(stderr, ANSII_PURPLE "%s\n" ANSII_RESET, message);
            break;

        default:
            fprintf(stderr, "If this prints something has gone very wrong");
            break;
    }
#elif defined (E1_PLAT_WINDOWS)
    // TODO: Logging colors for windows
    fprintf(stderr, "%s\n", message);
#endif
}
