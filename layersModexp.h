#ifndef _LAYERS_MOD_EXP_
#define _LAYERS_MOD_EXP_
#include"simpleBigint.h"
#include"simpleMontgomery.h"
#include<stdlib.h>
#include<algorithm>
#include<vector>

//LayersModexp 类，
//完成分层的模幂计算
class LayersModexp{
public:
    SimpleBigint cm;//底数
    SimpleBigint exp;//指数
    SimpleBigint mod;//模数
    SimpleBigint cmexp;//模幂
    MyMontgomery mym;//蒙哥马利运算器
    SimpleBigint cmr;//cm的蒙哥马利形式
    unsigned threads=4;//默认每次划分1/4
    unsigned layer=1;//划分次数=layer-1，如果不划分，k=exp
    //krsnum 每一次划分中剩余那部分的值
    vector<SimpleBigint>krsnum;//长度应该是layer//顺序是r_1,r_2...r_{layer-1},k
    //krsvalue 每一层计算出来之后的值
    vector<SimpleBigint>krsvalue;
    //构造函数，完成功能与init一样
    SimpleBigint noreduce;//用于解密中，是cmexp未经过最后一步reduce的值，是cmeexp*r mod n
    LayersModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c+m;
        while(cm.numbersLength()<=m.numbersLength()){
            cm=cm+m;
        }
        exp=e-SimpleBigint(1,0);
        mod=m;
        mym.initMontgomery(cm,cm,m);
        cmr=mym.myMontgomeryTo();
        cmexp=cmr;
        noreduce=cmr;
    }
    // }:
    //     cm(c+m),exp(e-SimpleBigint(1,0)),mod(m),mym(c+m,c+m,m)
    //     {cmr=mym.myMontgomeryTo();
    //     cmexp=cmr;}
    LayersModexp()=default;
    //huafenKrsnum 函数进行层级划分，记录每一层多余的值，最后一个记录最终的指数大小
    void huafenKrsnum_min();//指数较小的时候，通常是32bits以内
    void huafenKrsnum_max(unsigned mul=0);//指数较大的时候，mul是左移的位数，这个时候忽略thread，他就是1，layers就是exp.numberLength
    //initLayersModexp 函数进行初始化
    void initLayersModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c+m;
        while(cm.numbersLength()<=m.numbersLength()){
            cm=cm+m;
        }
        exp=e-SimpleBigint(1,0);
        mod=m;
        mym.initMontgomery(c+m,c+m,m);
        cmr=mym.myMontgomeryTo();
        cmexp=cmr;
        noreduce=cmr;
    }
    //指定层数与每层划分数（这里就叫thread数）
    void setThreadandLayer(unsigned t,unsigned l){threads=t;layer=l;}
    //清理上次计算的结果
    void clearRes(){cmexp=cmr;}
    //计算模幂值cmexp
    void layersmodexp_min();//32bits以内的计算
    void layersmodexp_max(unsigned mul=0);//对应于huafen，如果mul=0，则不会使用layersModexp进行计算

};
//使用顺序：
//1. 构造函数或init
//2. setThreadandLayer
//3. huafenKrsnum
//4. layersmodexp
//5. 从cmexp中拿到结果
#endif // _LAYERS_MOD_EXP_

