#ifndef _COMMON_H_
#define _COMMON_H_

#define INTERNAL_MACHINE

#define DEBUG
#ifdef DEBUG
#define LOG printf
#else
#define LOG(...) 
#endif

#define TEST_OUTPUT

#define REG_ADDRESS_SPACE 3


#endif // ifndef _COMMON_H_