#include"threadModexp.h"
//#define _THREAD_MODEXP_DEBUG
void ThreadModexp::assignThreadexp(){
    threadexp.clear();
    SimpleBigint t(threads,0);
    SimpleBigint tmp=exp/t;
    SimpleBigint shengyu=exp-tmp*t;
    #ifdef _THREAD_MODEXP_DEBUG
    cout<<"exp="<<exp<<",t="<<t<<",shengyu:"<<shengyu<<",shengyu.size()"<<shengyu.numbersLength()<<",shengyu[0]:"<<shengyu.numbers[0]<<endl;
    #endif // _THREAD_MODEXP_DEBUG
    for(unsigned i=0;i<threads;++i){
        if(shengyu.numbersLength()!=0 && i<shengyu.numbers[0]){
            SimpleBigint tmp1(tmp+SimpleBigint(1,0));
            threadexp.push_back(tmp1);
            #ifdef _THREAD_MODEXP_DEBUG
            cout<<"in i="<<i<<" ,we have tmp="<<tmp<<",and tmp1="<<tmp1<<endl;
            #endif // _THREAD_MODEXP_DEBUG
        }
        else{
            threadexp.push_back(tmp);
        }
        //cout<<"vector:"<<threadexp[i]<<endl;
    }
}
/*
SimpleBigint ThreadModexp::dotwoMul(unsigned a,unsigned b){
    if(a>=threadRes.size() || b>= threadRes.size())
        return SimpleBigint(0,0);
    SimpleBigint res;
    res=mym.myMontgomeryMul(threadRes[a],threadRes[b]);
    return res;
}*/
void*threadExpchild(void*data){
    intptr_t me=(intptr_t)data;
    #ifdef _THREAD_MODEXP_DEBUG
    //cout<<"my name is:"<<me<<endl;
    #endif
    if(me>=tme.threads)pthread_exit(nullptr);
    SimpleBigint mytasks;
    mytasks=tme.threadexp[me];
    #ifdef _THREAD_MODEXP_DEBUG
    cout<<"my("<<me<<") task number is:"<<mytasks<<endl;
    #endif
    SimpleBigint resr=tme.cmr;
    for(SimpleBigint i(2,0);mytasks>=i;i+=SimpleBigint(1,0)){
        resr=tme.mym.myMontgomeryMul(resr,tme.cmr);
    }
    pthread_mutex_lock(&tme.resmutex);
    if(!(mytasks==SimpleBigint(0,0))){
        #ifdef _THREAD_MODEXP_DEBUG
        cout<<"my("<<me<<")mul:"<<tme.threadRes<<","<<resr<<endl;
        #endif
        tme.threadRes=tme.mym.myMontgomeryMul(tme.threadRes,resr);
        #ifdef _THREAD_MODEXP_DEBUG
        cout<<"my("<<me<<")mul:"<<tme.threadRes<<endl;//<<","<<resr<<"mym:"<<tme.mym.R<<","<<tme.mym.N<<endl;
        #endif

    }
    pthread_mutex_unlock(&tme.resmutex);
    #ifdef _THREAD_MODEXP_DEBUG
    //cout<<"my("<<me<<") work is done,bye~"<<endl;
    #endif
    pthread_exit(nullptr);
}
void*simdthreadExpchild(void*data){
    intptr_t me=(intptr_t)data;
    #ifdef _THREAD_MODEXP_DEBUG
    //cout<<"my name is:"<<me<<endl;
    #endif
    if(me>=tme.threads)pthread_exit(nullptr);
    SimpleBigint mytasks;
    mytasks=tme.threadexp[me];
    #ifdef _THREAD_MODEXP_DEBUG
    cout<<"my("<<me<<") task number is:"<<mytasks<<endl;
    #endif
    SimpleBigint resr=tme.cmr;
    for(SimpleBigint i(2,0);mytasks>=i;i+=SimpleBigint(1,0)){
        resr=tme.mym.simdMontgomeryMul(resr,tme.cmr);
    }
    pthread_mutex_lock(&tme.resmutex);
    if(!(mytasks==SimpleBigint(0,0))){
        #ifdef _THREAD_MODEXP_DEBUG
        cout<<"my("<<me<<")mul:"<<tme.threadRes<<","<<resr<<endl;
        #endif
        tme.threadRes=tme.mym.simdMontgomeryMul(tme.threadRes,resr);
        #ifdef _THREAD_MODEXP_DEBUG
        cout<<"my("<<me<<")mul:"<<tme.threadRes<<endl;//<<","<<resr<<"mym:"<<tme.mym.R<<","<<tme.mym.N<<endl;
        #endif

    }
    pthread_mutex_unlock(&tme.resmutex);
    #ifdef _THREAD_MODEXP_DEBUG
    //cout<<"my("<<me<<") work is done,bye~"<<endl;
    #endif
    pthread_exit(nullptr);
}
/*
void*ThreadModexp::twoMulchild(void*data){
    Nodes*aandb=(Node*)data;
    unsigned a=aandb->a;
    unsigned b=aandb->b;
    SimpleBigint res=dotwoMul(a,b);
    pthread_exit((void*)res);

}*/

SimpleBigint threadmodexpMain(bool simd){

    pthread_t*ptid=new pthread_t[tme.threads];
    void*(*f)(void*)=nullptr;
    if(simd)f=simdthreadExpchild;
    else f=simdthreadExpchild;
    for(unsigned i=0;i<tme.threads;++i){
        pthread_create(&ptid[i],nullptr,f,(void*)i);
    }
    #ifdef _THREAD_MODEXP_DEBUG
    cout<<"all threads:"<<tme.threads<<" created!"<<endl;
    #endif
    for(unsigned i=0;i<tme.threads;++i){
        pthread_join(ptid[i],nullptr);
    }
    #ifdef _THREAD_MODEXP_DEBUG
    cout<<"all threads exits!"<<endl;
    #endif
    free(ptid);
    #ifdef _THREAD_MODEXP_DEBUG
    cout<<"modexpR-1 done with threadRes="<<tme.threadRes<<endl;
    #endif
    tme.threadRes=tme.mym.myMontgomeryReduction(tme.threadRes);
    #ifdef _THREAD_MODEXP_DEBUG
    cout<<"the result is:"<<tme.threadRes<<endl;
    #endif
    return tme.threadRes;
}
