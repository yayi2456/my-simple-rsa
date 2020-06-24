#ifndef _QUICK_MOD_EXP
#define _QUICK_MOD_EXP
#include"simpleBigint.h"
#include"simplemontgomery.h"
#include<omp.h>
#define _TWO_TASKS_

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
        mym.initMontgomery(cm,cm,m);
        cmr=mym.myMontgomeryTo();
    }
    QuickModExp()=default;
    void initQuick(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c+m;
        while(cm.numbersLength()<=m.numbersLength()){
            cm=cm+m;
        }
        exp=e;
        mod=m;
        mym.initMontgomery(cm,cm,m);
        cmr=mym.myMontgomeryTo();
    }
    void quickmodexp();
};
#endif // _QUICK_MOD_EXP
