#ifndef _THREAD_MOD_EXP_
#define _THREAD_MOD_EXP_
#include<pthread.h>
#include"simpleBigint.h"
#include"simplemontgomery.h"
#include<stdlib.h>
class ThreadModexp{
public:
    SimpleBigint cm;
    SimpleBigint cmr;
    SimpleBigint exp;
    SimpleBigint mod;
    unsigned threads=4;//默认4个线程
    vector<SimpleBigint>threadexp;
    SimpleBigint threadRes;
    MyMontgomery mym;
    pthread_mutex_t resmutex=PTHREAD_MUTEX_INITIALIZER;

    ThreadModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m):
        cm(c),exp(e-SimpleBigint(1,0)),mod(m),mym(c,c,m)
        {pthread_mutex_init(&resmutex,NULL);}
    ThreadModexp()=default;
    void initThreadModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c;
        exp=e-SimpleBigint(1,0);
        mod=m;
        mym.initMontgomery(c,c,m);
        pthread_mutex_init(&resmutex,NULL);//cout<<exp<<endl;
    }
    void initcmr(){cmr=mym.myMontgomeryTo();
        threadRes=cmr;}
    //将threadexp中每一项天上一个数字，所有数字加起来等于exp
    void setThreads(unsigned t){threads=t;}
    void assignThreadexp();
    void clearRes(){threadRes=cmr;}

};
extern ThreadModexp tme;
void*threadExpchild(void*);
void*simdthreadExpchild(void*);
SimpleBigint threadmodexpMain(bool simd=false);
#endif // _MOD_EXP_
