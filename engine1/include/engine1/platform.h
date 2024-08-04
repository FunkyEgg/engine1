#ifndef ENGINE1_PLATFORM_H
#define ENGINE1_PLATFORM_H

#ifdef _WIN32
#define E1_PLAT_WINDOWS
#ifndef _WIN64
#error "64 bit is required"
#endif
#elif defined(__linux__)
#define E1_PLAT_LINUX
#else
#error "Unsupported platform"
#endif

#endif
