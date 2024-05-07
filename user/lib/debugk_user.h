#ifndef _DBGK_H_
#define _DBGK_H_

#define DEBUGK // 可以注释，那就不会打印

#ifdef DEBUGK
#define debugk_user(fmt, ...)                      \
    do {                                      \
        debugf("[DEBUG]" fmt, ##__VA_ARGS__); \
        debugf("\n");                         \
    } while (0)
#else
#define debugk(...)
#endif
#endif // !_DBGK_H_
