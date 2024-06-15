#ifndef _DBGK_H_
#define _DBGK_H_
#include <printk.h>

#define DEBUGK
// 可以注释，那就不会打印

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
#define debugk_user(fmt, ...)                 \
    do {                                      \
        debugf("[DEBUG]" fmt, ##__VA_ARGS__); \
        debugf("\n");                         \
    } while (0)
#else
#define debugk_user(...)
#endif

#define SHELL
// 可以注释，那就不会打印
#ifdef SHELL
#define shellk(fmt, ...)                      \
    do {                                      \
        printk( fmt, ##__VA_ARGS__); \
    } while (0)
#else
#define shellk(...)
#endif

#endif // !_DBGK_H_
