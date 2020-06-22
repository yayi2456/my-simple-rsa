#ifndef _MOD_EXP_
#define _MOD_EXP_
#include"simpleBigint.h"
#include"simplemontgomery.h"
class SimpleModExp{
public:
    SimpleBigint cm;
    SimpleBigint exp;
    SimpleBigint mod;
    SimpleBigint cmexp;
    MyMontgomery mym;
    SimpleBigint cmr;
    SimpleModExp(SimpleBigint c,SimpleBigint e,SimpleBigint m):
        cm(c),exp(e),mod(m),mym(c,c,m)
        {cmr=mym.myMontgomeryTo();}
    SimpleModExp()=default;
    void modexp();
    void simdmodexp();
};
#endif // _MOD_EXP_
