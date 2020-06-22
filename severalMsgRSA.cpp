#include"severalMsgRSA.h"
//#define _THREAD_MSGRSA_DEBUG
bool SeveralMsgRSAC::initSRSA(unsigned length){
    return srsa.generateKey(length);
}
void SeveralMsgRSAC::assigntasks(){
    msgnums.clear();
    unsigned tmp=msgs.size()/threads;
    unsigned shengyu=msgs.size()-tmp*threads;
    unsigned tmp1=tmp+1;
    for(unsigned i=0;i<threads;++i){
        if(i<shengyu){

            msgnums.push_back(tmp1*(i));
        }
        else{
            msgnums.push_back(tmp1*(shengyu)+tmp*(i-shengyu));
        }
        //cout<<"vector:"<<threadexp[i]<<endl;
    }
    msgnums.push_back(msgs.size());
}
void SeveralMsgRSAC::initMsg(vector<SimpleBigint>&m){

    for(auto it=m.begin();it!=m.end();++it){
        msgs.push_back(*it);
        ctxt.push_back(SimpleBigint("0"));
    }
}
void SeveralMsgRSAC::encryptmsgs(unsigned from,unsigned to){
    SimpleBigint tmp;
    to=to>msgs.size()?msgs.size():to;
    for(auto it=from;it<to;++it){
        tmp=srsa.encryptme(msgs[it]);
        ctxt[it]=(tmp);
    }
}
void SeveralMsgRSAC::simdencryptmsgs(unsigned from,unsigned to){
    SimpleBigint tmp;
    to=to>msgs.size()?msgs.size():to;
    for(auto it=from;it<to;++it){
        tmp=srsa.simdencryptme(msgs[it]);
        ctxt[it]=(tmp);
    }
}
/*
vector<SimpleBigint> SeveralMsgRSAC::decryptmsgs(vector<SimpleBigint>::iterator from,vector<SimpleBigint>::iterator to){
    SimpleBigint tmp;
    vector<SimpleBigint>res;
    for(auto it=from;it!=to;++it){
        tmp=srsa.decryptme(*it);
        res.push_back(tmp);
    }
    return res;
}
*/
void*simdencryptmsgschild(void*data){
    intptr_t me=(intptr_t)data;
    #ifdef _THREAD_MSGRSA_DEBUG
    cout<<"my name is:"<<me<<endl;
    #endif
    if(me>=smr.threads)pthread_exit(nullptr);
    unsigned mytaskbegin=smr.msgnums[me];
    unsigned mytaskend=smr.msgnums[me+1];
    #ifdef _THREAD_MSGRSA_DEBUG
    cout<<"my("<<me<<") task number from:"<<mytaskbegin<<",to:"<<mytaskend<<endl;
    #endif
    smr.simdencryptmsgs(mytaskbegin,mytaskend);
    pthread_exit(nullptr);
}
void*encryptmsgschild(void*data){
    intptr_t me=(intptr_t)data;
    #ifdef _THREAD_MSGRSA_DEBUG
    cout<<"my name is:"<<me<<endl;
    #endif
    if(me>=smr.threads)pthread_exit(nullptr);
    unsigned mytaskbegin=smr.msgnums[me];
    unsigned mytaskend=smr.msgnums[me+1];
    #ifdef _THREAD_MSGRSA_DEBUG
    cout<<"my("<<me<<") task number from:"<<mytaskbegin<<",to:"<<mytaskend<<endl;
    #endif
    smr.encryptmsgs(mytaskbegin,mytaskend);
    pthread_exit(nullptr);
}
void threadencryptmsg(bool simd){
    pthread_t*ptid=new pthread_t[smr.threads];
    void*(*f)(void*)=nullptr;
    if(simd)f=simdencryptmsgschild;
    else f=encryptmsgschild;
    for(unsigned i=0;i<smr.threads;++i){
        pthread_create(&ptid[i],nullptr,f,(void*)i);
    }
    #ifdef _THREAD_MSGRSA_DEBUG
    cout<<"all threads:"<<smr.threads<<" created!"<<endl;
    #endif
    for(unsigned i=0;i<smr.threads;++i){
        pthread_join(ptid[i],nullptr);
    }
    #ifdef _THREAD_MSGRSA_DEBUG
    cout<<"all threads exits!"<<endl;
    #endif
    free(ptid);
}
