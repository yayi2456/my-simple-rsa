#ifndef _QUICK_MOD_EXP
#define _QUICK_MOD_EXP
#include"simpleBigint.h"
#include"simplemontgomery.h"
#include<omp.h>
//#define _TWO_TASKS_

class QuickModExp{
public:
    SimpleBigint cm;
    SimpleBigint exp;
    SimpleBigint mod;
    SimpleBigint cmexp;
    MyMontgomery mym;
    SimpleBigint cmr;
    QuickModExp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c+m;
        while(cm.numbersLength()<=m.numbersLength()){
            cm=cm+m;
        }
        exp=e;
        mod=m;
        #ifdef _TEST_TIME_QUCK_MODEXP
        long long head,freq,tail;
        QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        mym.initMontgomery(cm,cm,m);
        #ifdef _TEST_TIME_QUCK_MODEXP
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        double intervel=(tail-head)*1000.0/freq;
        cout<<"in quickModexp-initmym, time used="<<intervel<<" ms"<<endl;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        cmr=mym.myMontgomeryTo();
        #ifdef _TEST_TIME_QUCK_MODEXP
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        intervel=(tail-head)*1000.0/freq;
        cout<<"in quickModexp-initmym, time used="<<intervel<<" ms"<<endl;
        #endif
    }
    QuickModExp()=default;
    void initQuick(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c+m;
        while(cm.numbersLength()<=m.numbersLength()){
            cm=cm+m;
        }
        exp=e;
        mod=m;
        #ifdef _TEST_TIME_QUCK_MODEXP
        long long head,freq,tail;
        QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        mym.initMontgomery(cm,cm,m);
        #ifdef _TEST_TIME_QUCK_MODEXP
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        double intervel=(tail-head)*1000.0/freq;
        cout<<"in quickModexp-initmym, time used="<<intervel<<" ms"<<endl;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        cmr=mym.myMontgomeryTo();
        #ifdef _TEST_TIME_QUCK_MODEXP
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        intervel=(tail-head)*1000.0/freq;
        cout<<"in quickModexp-initmym, time used="<<intervel<<" ms"<<endl;
        #endif
    }
    void quickmodexp();
};
#endif // _QUICK_MOD_EXP
