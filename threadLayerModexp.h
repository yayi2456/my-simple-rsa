#ifndef _THREAD_LAYERS_MOD_EXP_
#define _THREAD_LAYERS_MOD_EXP_
#include"simpleBigint.h"
#include"simpleMontgomery.h"
#include<stdlib.h>
#include<algorithm>
#include<vector>

//LayersModexp 类，
//完成分层的模幂计算
class ThreadLayersModexp{
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
    ThreadLayersModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
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
    ThreadLayersModexp()=default;
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

//MultiSimpleBignt 用于多个SimpleBigint传递给child
class MultiSimpleBignt{
public:
    uint32_t numsofBigint=0;
    vector<SimpleBigint>simpleBigints;
    void*toVoidStar(){
        void*res;
        uint32_t totalsize=0;
        for(int i=0;i<simpleBigints.size();++i){
            totalsize+=simpleBigints[i].numbersLength();
        }
        res=malloc(sizeof(uint32_t)*(1+simpleBigints.size()+totalsize));
        memcpy(res,&numsofBigint,sizeof(uint32_t));
        uint32_t position=sizeof(uint32_t);
        for(int i=0;i<simpleBigints.size();++i){
            uint32_t simplebigintsize=simpleBigints[i].numbersLength();
            memcpy(res+position,&simplebigintsize,sizeof(uint32_t));
            position+=sizeof(uint32_t);
            memcpy(res+position,&(simpleBigints[i].numbers[0]),sizeof(uint32_t)*simplebigintsize);
            position+=(sizeof(uint32_t)*simplebigintsize);
        }
        return res;
    }
    MultiSimpleBignt()=default;
    MultiSimpleBignt(void*voidspace){
        simpleBigints.clear();
        memcpy(&numsofBigint,voidspace,sizeof(uint32_t));
        int position=sizeof(uint32_t);
        for(int i=0;i<numsofBigint;++i){
            SimpleBigint subSimpleBigint;
            subSimpleBigint=(SimpleBigint)(voidspace+position);
            //cout<<subSimpleBigint<<endl;
            position+=(sizeof(uint32_t)*(1+subSimpleBigint.numbersLength()));
            simpleBigints.push_back(subSimpleBigint);
        }
    }
    void AddOne(SimpleBigint simplebigint){
        simpleBigints.push_back(simplebigint);
        numsofBigint+=1;
    }
    friend ostream&operator<<(ostream&out,const MultiSimpleBignt&multibigint);
};


extern SimpleBigint cm1;
extern SimpleBigint mod1;
#endif // _THREAD_LAYERS_MOD_EXP_


