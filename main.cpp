#include <iostream>
#include"simpleBigint.h"
#include"simpleMontgomery.h"
#include"myRSA.h"
#include<time.h>
#include<Windows.h>
#include"crtRSA.h"
using namespace std;
//#define _MAIN_DEBUG_
 
//generateMsg 函数，随机生成一串消息，消息长度需要小于等于n的长度
SimpleBigint generateMsg(unsigned length){
    srand(time(nullptr));//
    SimpleBigint msgsme;
    unsigned upper=(length%32)?length/32+1:length/32;
    for(unsigned j=0;j<upper;++j){
        uint32_t randomv1=rand()%UINT16_MAX;
        uint32_t randomv2=rand()%UINT16_MAX;
        while(!randomv1 && !randomv2){
            randomv1=rand()%UINT16_MAX;
            randomv2=rand()%UINT16_MAX;
        }
        msgsme.numbers.push_back(randomv1*(UINT16_MAX+1)+randomv2);
    }
    return msgsme;
}
void testSimpleRSA(unsigned length){
    SimpleBigint msg=generateMsg(length);
    #ifdef _MAIN_DEBUG_
    msg.getfromString("5e91798767");
    #endif
    cout<<"MSG: "<<msg<<endl;
    MyRSA rsame;
    if(!rsame.generateKey(length)){
        cout<<"quiting..."<<endl;
        return;
    }
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint enmsg=rsame.encryptme(msg);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1.0/freq;
    cout<<"in testSimpleRSA-encrypt, time used="<<intervel<<" s, result= "<<enmsg<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint demsg=rsame.decryptme(enmsg);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<"in testSimpleRSA-decrypt, time used="<<intervel<<" s, result= "<<demsg<<endl;
}

void testSimpleLayersRSA(unsigned length,unsigned threads=1,unsigned layers=1){
    SimpleBigint msg=generateMsg(length);
    #ifdef _MAIN_DEBUG_
    msg.getfromString("5e91724c5708257259364054542156a7234945fe57ad144757195f4f52713b7444b14c1523d67a6379f5079410235daf7dff066d1ffb21de7bba26d145183533");//5e91724c5708257259364054542156a7234945fe57ad144757195f4f52713b7444b14c1523d67a6379f5079410235daf7dff066d1ffb21de7bba26d145183533");
    #endif
    cout<<"MSG: "<<msg<<endl;
    MyRSA rsame;
    if(!rsame.generateKey(length)){
        cout<<"quiting..."<<endl;
        return;
    }
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint enmsg=rsame.encryptme_layers(msg,threads,layers);
    //SimpleBigint enmsg(1,0);
    //enmsg.getfromString("96b67d5fdf2060efcaae4fb24613684127b2feef5eda8786768930d024e3a43f783caaf1acfe1da3dc4bfd9ccf2c26acb5596bfd84a9bb62c454d2c91095f982");
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000.0/freq;
    cout<<"in testSimpleLayersRSA-encrypt, time used="<<intervel<<" ms, result= "<<enmsg<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint demsg=rsame.decryptme_layers(enmsg,threads,layers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<"in testSimpleLayersRSA-decrypt, time used="<<intervel<<" s, result= "<<demsg<<endl;
    cout<<"COMPARE:"<<endl;
    cout<<msg<<endl<<demsg<<endl;

}
void testSimpleQuickRSA(unsigned length){
    SimpleBigint msg;
    #ifdef _MAIN_DEBUG_
    msg.getfromString("4dab05dd19f573485ea3408704f3136d21e5799a33b37b8d6be20df34b997028683855eb4c675faf261c30135c422e614f080d2d7c02496513a4596d4fbd7d27");
    //5e91724c5708257259364054542156a7234945fe57ad144757195f4f52713b7444b14c1523d67a6379f5079410235daf7dff066d1ffb21de7bba26d145183533");//5e91724c5708257259364054542156a7234945fe57ad144757195f4f52713b7444b14c1523d67a6379f5079410235daf7dff066d1ffb21de7bba26d145183533");
    #else
    msg=generateMsg(length);
    #endif // _MAIN_DEBUG_
    cout<<"MSG: "<<msg<<endl;
    MyRSA rsame;
    if(!rsame.generateKey(length)){
        cout<<"quiting..."<<endl;
        return;
    }
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint enmsg=rsame.encryptme_quick(msg);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000.0/freq;
    cout<<"in testSimpleQuickRSA-encrypt, time used="<<intervel<<" ms, result= "<<enmsg<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint demsg=rsame.decryptme_quick(enmsg);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<"in testSimpleQuickRSA-decrypt, time used="<<intervel<<" s, result= "<<demsg<<endl;
    cout<<"COMPARE:"<<endl;
    cout<<msg<<endl<<demsg<<endl;
}

void testCrtLayersRSA(unsigned length,unsigned threads=1,unsigned layers=1){
    SimpleBigint msg;
    #ifdef _MAIN_DEBUG_
    msg.getfromString("4dab05dd19f573485ea3408704f3136d21e5799a33b37b8d6be20df34b997028683855eb4c675faf261c30135c422e614f080d2d7c02496513a4596d4fbd7d27");
    //5e91724c5708257259364054542156a7234945fe57ad144757195f4f52713b7444b14c1523d67a6379f5079410235daf7dff066d1ffb21de7bba26d145183533");//5e91724c5708257259364054542156a7234945fe57ad144757195f4f52713b7444b14c1523d67a6379f5079410235daf7dff066d1ffb21de7bba26d145183533");
    #else
    msg=generateMsg(length);
    #endif // _MAIN_DEBUG_
    cout<<"MSG: "<<msg<<endl;
    CrtRSA rsame;
    if(!rsame.generateKey(length)){
        cout<<"quiting..."<<endl;
        return;
    }
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #ifndef _MAIN_DEBUG_
    SimpleBigint enmsg=rsame.encryptme_layers(msg,threads,layers);
    #else
    SimpleBigint enmsg(1,0);
    enmsg.getfromString("a24051dbb94c876cf3701a4dd6ad7f6ad49083ae0182d86eade776a169ca1b877af4f7ffee100c4c605da3dd4d975b6cc146da9bfe6d50ce1a566ffcb7d25f31");
    //96b67d5fdf2060efcaae4fb24613684127b2feef5eda8786768930d024e3a43f783caaf1acfe1da3dc4bfd9ccf2c26acb5596bfd84a9bb62c454d2c91095f982");
    #endif // _MAIN_DEBUG_
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000.0/freq;
    cout<<"in testCrtLayersRSA-encrypt, time used="<<intervel<<" ms, result= "<<enmsg<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint demsg=rsame.decryptme_layers(enmsg,threads,layers);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<"in testCrtLayersRSA-decrypt, time used="<<intervel<<" s, result= "<<demsg<<endl;
    cout<<"COMPARE:"<<endl;
    cout<<msg<<endl<<demsg<<endl;
}
void testCrtQuickRSA(unsigned length){
    SimpleBigint msg;
    #ifdef _MAIN_DEBUG_
    msg.getfromString("4dab05dd19f573485ea3408704f3136d21e5799a33b37b8d6be20df34b997028683855eb4c675faf261c30135c422e614f080d2d7c02496513a4596d4fbd7d27");
    //5e91724c5708257259364054542156a7234945fe57ad144757195f4f52713b7444b14c1523d67a6379f5079410235daf7dff066d1ffb21de7bba26d145183533");//5e91724c5708257259364054542156a7234945fe57ad144757195f4f52713b7444b14c1523d67a6379f5079410235daf7dff066d1ffb21de7bba26d145183533");
    #else
    msg=generateMsg(length);
    #endif // _MAIN_DEBUG_
    cout<<"MSG: "<<msg<<endl;
    CrtRSA rsame;
    if(!rsame.generateKey(length)){
        cout<<"quiting..."<<endl;
        return;
    }
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #ifndef _MAIN_DEBUG_
    SimpleBigint enmsg=rsame.encryptme_quick(msg);
    #else
    SimpleBigint enmsg(1,0);
    enmsg.getfromString("a24051dbb94c876cf3701a4dd6ad7f6ad49083ae0182d86eade776a169ca1b877af4f7ffee100c4c605da3dd4d975b6cc146da9bfe6d50ce1a566ffcb7d25f31");
    //96b67d5fdf2060efcaae4fb24613684127b2feef5eda8786768930d024e3a43f783caaf1acfe1da3dc4bfd9ccf2c26acb5596bfd84a9bb62c454d2c91095f982");
    #endif // _MAIN_DEBUG_
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000.0/freq;
    cout<<"in testCrtQuickRSA-encrypt, time used="<<intervel<<" ms, result= "<<enmsg<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint demsg=rsame.decryptme_quick(enmsg);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<"in testCrtQuickRSA-decrypt, time used="<<intervel<<" s, result= "<<demsg<<endl;
    cout<<"COMPARE:"<<endl;
    cout<<msg<<endl<<demsg<<endl;
}
int main(int argc,char*argv[]){
    unsigned length=512;
    int threads=16,layers=5;
    if(argc>1)length=strtol(argv[1],nullptr,10);
    //testSimpleLayersRSA(length,threads,layers);
    //cout<<"-------------======================---------------"<<endl;
    //testCrtLayersRSA(length,threads,layers);
    //cout<<"-------------======================---------------"<<endl;
    //testCrtQuickRSA(length);
    //cout<<"----------------------------"<<endl;
    testSimpleQuickRSA(length);
    return 0;
}