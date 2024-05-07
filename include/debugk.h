#ifndef _DBGK_H_
#define _DBGK_H_
#include <printk.h>

// #define DEBUGK // 可以注释，那就不会打印

#ifdef DEBUGK
#define debugk(fmt, ...)                      \
    do {                                      \
        printk("[DEBUG]" fmt, ##__VA_ARGS__); \
        printk("\n");                         \
    } while (0)
#else
#define debugk(...)
#endif

#ifdef DEBUGK
#define debugk_user(fmt, ...)                      \
    do {                                      \
        debugf("[DEBUG]" fmt, ##__VA_ARGS__); \
        debugf("\n");                         \
    } while (0)
#else
#define debugk_user(...)
#endif

#endif // !_DBGK_H_
