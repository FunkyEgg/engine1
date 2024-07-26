#ifndef ENGINE1_CORE_ASSERT_H
#define ENGINE1_CORE_ASSERT_H

#ifdef DEBUG

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond, message) \
    do { if (!(cond)) { fprintf(stderr, message "\n"); exit(-1); } } while (0)
#define ASSERTF(cond, message, ...) \
    do { if (!(cond)) { fprintf(stderr, message "\n", __VA_ARGS__); exit(-1); } } while (0)

#else
#define ASSERT(cond, message) (void(0))
#define ASSERTF(cond, message, ...) (void(0))
#endif

#endif
