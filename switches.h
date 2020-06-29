#ifndef _SWITCHES_
#define _SWITCHES_
///本文件保存有各种宏开关，方便寻找使用
///time test
//#define _TIME_TEST_///for simple montgomery
#define _TEST_TIME_SIMPLE_MONTGOMERY_
#define _TEST_TIME_CRT_RSA
#define _TEST_TIME_QUCK_MODEXP
#define _TEST_TIME_KERNEL_

///parallel test
//#define _OPEN_OPENMP_
//#define _TWO_TASKS_
#define _AVX2_ENABLED_

//#define _OPENMP_MOVE_MUL  //do not open this!!!

///debug
//#define _QUICK_MODEXP_DEBUG_
//#define _QUICK_TWOTASK_DEBUG_

///function main
//#define _TEST_CRT_MAIN

#endif // _SWITCHES_
