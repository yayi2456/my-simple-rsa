#include"multiThreadModexp.h"
//#define _MULTI_THREAD_MODEXP_DEBUG
void MultiThreadModexp::huafenKrsnum(){
    krsnum.clear();
    krsvalue.clear();
    SimpleBigint t(threads,0);
    SimpleBigint tmp=exp;
    for(unsigned i=1;i<layer;++i){
        tmp=tmp/t;
        SimpleBigint shengyu=tmp-tmp*t;
        krsnum.push_back(shengyu);
        krsvalue.push_back(SimpleBigint(0,0));
        #ifdef _MULTI_THREAD_MODEXP_DEBUG
        cout<<"the tmp is:"<<tmp<<",shengyu is"<<shengyu;
        cout<<endl;
    #endif // _MULTI_THREAD_MODEXP_DEBUG
    }
    if(SimpleBigint(0,0)==tmp){
        cerr<<"thread^layer is too large, decrease layer or thread!"<<endl;
        return;
    }
    krsnum.push_back(tmp);
    krsvalue.push_back(SimpleBigint(0,0));
    #ifdef _MULTI_THREAD_MODEXP_DEBUG
    cout<<"the krsnum is:";
    for(auto it=krsnum.begin();it!=krsnum.end();++it){
        cout<<(*it)<<" ";
    }
    cout<<endl;
    #endif // _MULTI_THREAD_MODEXP_DEBUG
}
/*
void*ThreadModexp::twoMulchild(void*data){
    Nodes*aandb=(Node*)data;
    unsigned a=aandb->a;
    unsigned b=aandb->b;
    SimpleBigint res=dotwoMul(a,b);
    pthread_exit((void*)res);

}*/

SimpleBigint multiSIMDThreadmodexpMain(){
    //assign在main外面做
    if(mtme.krsnum.size()!=mtme.layer){
        cerr<<"wrong in assignment, try again."<<endl;
        mtme.threadRes=SimpleBigint(0,0);
        return SimpleBigint(0,0);
    }
    //找最大值
    SimpleBigint maxi(0,0);
    for(unsigned i=0;i<mtme.layer;++i){
        if(mtme.krsnum[i]>maxi)maxi=mtme.krsnum[i];
    }
/*    for(auto it=mtme.krsnum.begin();it!=mtme.krsnum.end();++it){
            if(maxi==*(it)){
                mtme.krsnum.erase(it);
                break;
            }
    }*/
    #ifdef _MULTI_THREAD_MODEXP_DEBUG
    cout<<"the maxi is"<<maxi<<endl;
    SimpleBigint sbp(0,0);
    #endif
    SimpleBigint resr=mtme.cmr;
    unsigned j=0;
    for(auto it=mtme.krsnum.begin();it!=mtme.krsnum.end();++j,++it){
        if(SimpleBigint(1,0)==*(it)){
            mtme.krsvalue[j]=resr;
            mtme.krsnum.erase(it);
            #ifdef _MULTI_THREAD_MODEXP_DEBUG
            sbp=sbp+(*it);cout<<"add1-";
            #endif
            break;
        }
    }
    for(SimpleBigint i(2,0);maxi>=i;i+=SimpleBigint(1,0)){
        resr=mtme.mym.simdMontgomeryMul(resr,mtme.cmr);
        j=0;
        for(auto it=mtme.krsnum.begin();it!=mtme.krsnum.end();++j,++it){
            if(i==*(it)){
                mtme.krsvalue[j]=resr;
                mtme.krsnum.erase(it);
                #ifdef _MULTI_THREAD_MODEXP_DEBUG
                sbp=sbp+(*it);cout<<"add1-";
                #endif
                break;
            }
        }
    }
    #ifdef _MULTI_THREAD_MODEXP_DEBUG
    cout<<"all the exp="<<sbp<<endl;
    #endif
    //mtme.krsvalue[mtme.layer-1]=resr;
    #ifdef _MULTI_THREAD_MODEXP_DEBUG
    cout<<"the krsnum is:";
    for(auto it=mtme.krsnum.begin();it!=mtme.krsnum.end();++it){
        cout<<(*it)<<" ";
    }
    cout<<"the krsvalue is:";
    for(auto it=mtme.krsvalue.begin();it!=mtme.krsvalue.end();++it){
        cout<<(*it)<<" ";
    }
    cout<<endl;
    #endif
    //现在开始合并
    SimpleBigint tmp(mtme.krsvalue[mtme.layer-1]);
    for(unsigned i=1;i<mtme.layer;++i){
        for(unsigned j=1;j<mtme.threads;++j){
            #ifdef _MULTI_THREAD_MODEXP_DEBUG
            cout<<"the before-result of i="<<i<<",j="<<j<<",is:"<<tmp<<endl;
            #endif
            tmp=mtme.mym.myMontgomeryMul(tmp,mtme.krsvalue[mtme.layer-1]);
            #ifdef _MULTI_THREAD_MODEXP_DEBUG
            cout<<"the hebing-result of i="<<i<<",j="<<j<<",is:"<<tmp<<endl;
            #endif
        }
        if(!(mtme.krsvalue[mtme.layer-1-i]==SimpleBigint(0,0)))
            tmp=mtme.mym.myMontgomeryMul(tmp,mtme.krsvalue[mtme.layer-1-i]);
        mtme.krsvalue[mtme.layer-1]=tmp;//更新
    }
    mtme.threadRes=mtme.mym.myMontgomeryMul(tmp,mtme.threadRes);
    #ifdef _MULTI_THREAD_MODEXP_DEBUG
    cout<<"the unreducted-result is:"<<mtme.threadRes<<endl;
    #endif
    mtme.threadRes=mtme.mym.myMontgomeryReduction(mtme.threadRes);
    #ifdef _MULTI_THREAD_MODEXP_DEBUG
    cout<<"the result is:"<<mtme.threadRes<<endl;
    #endif
    return mtme.threadRes;
}
