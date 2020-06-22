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
    unsigned threads=4;//Ĭ��4���߳�
    SimpleBigint cm;
    SimpleBigint cmr;
    SimpleBigint exp;
    SimpleBigint mod;
    unsigned layer=1;//���ִ���=layer-1����������֣�k=exp
    SimpleBigint threadRes;
    MyMontgomery mym;
    vector<SimpleBigint>krsnum;//����Ӧ����layer//˳����r_1,r_2...r_{layer-1},k
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
    //��threadexp��ÿһ������һ�����֣��������ּ���������exp
    void setThreadandLayer(unsigned t,unsigned l){threads=t;layer=l;}
    void clearRes(){threadRes=cmr;}

};
//ʹ��һ��ȫ�ֱ��������㣬
//�о���̫��������ϣ����threadModexp��ͬ����ʵ�ַ�ʽ
extern MultiThreadModexp mtme;
//void threadExpchild();
//void simdthreadExpchild();
SimpleBigint multiSIMDThreadmodexpMain();
#endif // _MULTI_THREAD_MOD_EXP_

