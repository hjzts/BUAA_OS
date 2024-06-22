#ifndef _DBGK_H_
#define _DBGK_H_
#include <printk.h>

// #define DEBUGK
// #define SHELL
// 可以注释，那就不会打印
#define RETURN_VALUE
// 用于测试需要有子进程返回值时
// #define IPC
// 用来表示使用IPC的方法来获取返回值
#define APPEND 
// 表示输出append重定向

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


#ifdef SHELL
#define shellk(fmt, ...)                      \
    do {                                      \
        printk( fmt, ##__VA_ARGS__); \
    } while (0)
#else
#define shellk(...)
#endif

#endif // !_DBGK_H_
