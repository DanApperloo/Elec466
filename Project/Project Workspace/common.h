#ifndef _COMMON_H_
#define _COMMON_H_

#define DEBUG
#ifdef DEBUG
#define LOG printf
#else
#define LOG(...) 
#endif

//#define TEST_OUTPUT

//#define LOG_OUTPUT

//#define COMPARE_OUTPUT

#define REG_ADDRESS_SPACE 3


#endif // ifndef _COMMON_H_