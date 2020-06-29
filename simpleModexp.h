#ifndef _MOD_EXP_
#define _MOD_EXP_

#include"simpleBigint.h"
#include"simpleMontgomery.h"

//SimpleModExp 类，
//用来对给定参数调用蒙哥马利做模幂
class SimpleModExp{
public:
    SimpleBigint cm;//底数
    SimpleBigint exp;//指数
    SimpleBigint mod;//模数
    SimpleBigint cmexp;//模幂
    MyMontgomery mym;//蒙哥马利运算器
    SimpleBigint cmr;//cm的蒙哥马利形式
    SimpleModExp(SimpleBigint c,SimpleBigint e,SimpleBigint m):
        cm(c),exp(e),mod(m),mym(c,c,m)
        {cmr=mym.myMontgomeryTo();}
    SimpleModExp()=default;
    //计算cmexp
    void modexp();
};
#endif // _MOD_EXP_
