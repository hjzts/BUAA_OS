#ifndef _DBGK_H_
#define _DBGK_H_
#include<printk.h>

#define DEBUGK

#ifdef DEBUGK
#define debugk(fmt, ...) do { printk("debugk::",fmt, ##__VA_ARGS__); } while(0)
#else 
#define debugk(...) 
#endif
#endif // !_DBGK_H_
