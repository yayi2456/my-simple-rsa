#ifndef _MULTI_THREAD_MOD_EXP_
#define _MULTI_THREAD_MOD_EXP_
#include<pthread.h>
#include"simpleBigint.h"
#include"simplemontgomery.h"
#include<stdlib.h>
#include<algorithm>
#include<vector>
class MultiThreadModexp{
public:
    unsigned threads=4;//默认4个线程
    SimpleBigint cm;
    SimpleBigint cmr;
    SimpleBigint exp;
    SimpleBigint mod;
    unsigned layer=1;//划分次数=layer-1，如果不划分，k=exp
    SimpleBigint threadRes;
    MyMontgomery mym;
    vector<SimpleBigint>krsnum;//长度应该是layer//顺序是r_1,r_2...r_{layer-1},k
    vector<SimpleBigint>krsvalue;//

    MultiThreadModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m):
        cm(c),exp(e-SimpleBigint(1,0)),mod(m),mym(c,c,m)
        {}
    MultiThreadModexp()=default;
    void huafenKrsnum();
    void initMultiThreadModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c;
        exp=e-SimpleBigint(1,0);
        mod=m;
        mym.initMontgomery(c,c,m);
    }
    void initcmr(){cmr=mym.myMontgomeryTo();
        threadRes=cmr;}
    //将threadexp中每一项天上一个数字，所有数字加起来等于exp
    void setThreadandLayer(unsigned t,unsigned l){threads=t;layer=l;}
    void clearRes(){threadRes=cmr;}

};
//使用一个全局变量来计算，
//感觉不太合理，但是希望和threadModexp用同样的实现方式
extern MultiThreadModexp mtme;
//void threadExpchild();
//void simdthreadExpchild();
SimpleBigint multiSIMDThreadmodexpMain();
#endif // _MULTI_THREAD_MOD_EXP_

