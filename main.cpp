#include <iostream>
#include"simpleBigint.h"
#include"simplemontgomery.h"
#include"simpleRSA.h"
#include"threadModexp.h"
#include"severalMsgRSA.h"
#include<time.h>
#include"multiThreadModexp.h"
using namespace std;
//#define _MAIN_DEBUG_

void testAdd(){
    SimpleBigint num1,num2;
    cin>>num1;
    cout<<"num1="<<num1<<endl;
    cin>>num2;
    cout<<"num2="<<num2<<endl;
    cout<<"num1+num2="<<(num1+num2)<<endl;
    cout << "Hello world!" << endl;
}
void testMul(){
    SimpleBigint num1,num2;
    cin>>num1;
    cout<<"num1="<<num1<<endl;
    cin>>num2;
    cout<<"num2="<<num2<<endl;
    cout<<"num1*num2="<<(num1*num2)<<endl;
    cout<<"Hello"<<endl;
}
void testCompare(){
    SimpleBigint num1,num2;
    while(1){
        cin>>num1;
        cout<<"num1="<<num1<<endl;
        cin>>num2;
        cout<<"num2="<<num2<<endl;
        cout<<"num1>=num2="<<(num1>=num2)<<endl;
        cout<<"num1>num2="<<(num1>num2)<<endl;
        cout<<"num1==num2="<<(num1==num2)<<endl;
        cout<<"Hello"<<endl;
    }

}
void testMinus(){
    SimpleBigint num1,num2;
    cin>>num1;
    cout<<"num1="<<num1<<endl;
    cin>>num2;
    cout<<"num2="<<num2<<endl;
    cout<<"num1-num2="<<(num1-num2)<<endl;
    cout<<"Hello"<<endl;
}
void testMod(){
    SimpleBigint num1("10000000000000000"),num2("1528d4a3db");
    while(1){
            int cut;
        cin>>cut;
    cout<<"num1="<<num1<<endl;
    //cin>>num2;
    cout<<"num2="<<num2<<endl;
    cout<<"num1%num2="<<(num1%num2)<<endl;
    cout<<"Hello"<<endl;
    }

}
void testDiv(){
    SimpleBigint num1,num2;
    while(1){
        cin>>num1;
    cout<<"num1="<<num1<<endl;
    cin>>num2;
    cout<<"num2="<<num2<<endl;
    cout<<"num1/num2="<<(num1/num2)<<endl;
    }

    cout<<"Hello"<<endl;
}
void testDivideshift(){
    while(1){
        SimpleBigint num1;
        cin>>num1;
        cout<<"num1="<<num1<<endl;;
        cout<<num1.shiftDivide32()<<endl;
    }

}
void testMontMulAcc(){
    MyMontgomery mym;
    while(1){
    SimpleBigint num1,num2/*("12345678909876543211234567890987654321")*/,numN("2aeb");//c8832ddcd9e6477890bf173fe81e911551ed14add9edfdd6fb9b8c5ef10f5db6eb3716590dcee0a6367bf960cb6fc2ac904f879f9065758106df053c7f0f06ad");
//    SimpleBigint num1,num2,numN;
    cin>>num1;
    cout<<"num1="<<num1<<endl;
    //cin>>num2;
    num2=num1;
    cout<<"num2="<<num2<<endl;
//    cin>>numN;
    cout<<"numN="<<numN<<endl;
    mym.initMontgomery(num1,num2,numN);
    cout<<"R="<<mym.R<<endl;
    cout<<"r2modn="<<mym.R2modN<<endl;
    cout<<"A*R="<<mym.A*mym.R<<endl;
    cout<<"B*R="<<mym.B*mym.R<<endl;

    SimpleBigint AR,BR;
    AR=mym.myMontgomeryTo();BR=mym.myMontgomeryTo(false);
    cout<<"N_1="<<mym.calN_1()<<endl;
    cout<<"TEST:AR="<<AR<<" , BR="<<BR<<endl;
    SimpleBigint resR;
    resR=mym.myMontgomeryMul(AR,BR);
    SimpleBigint resRR=mym.simdMontgomeryMul(AR,BR);
    cout<<"TEST:myMontgomeryMul(num1,num2)="<<resR<<"\nREDUCTION:"<<mym.myMontgomeryReduction(resR)<<endl;
    cout<<"TEST:simdMontgomeryMul(num1,num2)="<<resRR<<"\nREDUCTION:"<<mym.myMontgomeryReduction(resRR)<<endl;
    cout<<"TEST:myMontgomeryReduction(AR*BR)="<<mym.myMontgomeryReduction(mym.myMontgomeryReduction(AR*BR))<<endl;
    SimpleBigint num12(num1*num2);
    cout<<"real:A*B="<<num12<<", A*B mod N="<<(num12)%numN<<endl;

    //cout<<"simd="<<simd_mulmod(num1,num2,numN)<<endl;
    cout<<"Hello"<<endl;
    int ext;
    cin>>ext;
    }
}
void testR2modN_S(){
    SimpleBigint num1;
    cin>>num1;
    cout<<"N="<<num1<<endl;
    //SimpleBigint r2modn=new MyR2modN_S(num1);
//    cout<<"r2modn="<<r2modn<<endl;
    cout<<"HELLO"<<endl;
}
void testdividenum(){
    while(1){
    string num;
    int exp;
    cin>>num;
    cout<<num<<endl;
    cin>>exp;
    cout<<exp<<endl;;
    SimpleBigint b(num);
    cout<<"b="<<b<<endl;
    cout<<"res:"<<b.moveby2_divide(exp)<<endl;;
    }
}
void testN_1(){
//while(1){
    string num;
//    int exp;

    SimpleBigint sb("64973299089798798787868");
    SimpleBigint nn("2aeb");
    MyMontgomery b(sb,sb,nn);
    cout<<sb<<","<<nn<<endl;
    SimpleBigint rese=b.calN_1();
    cout<<"N_1:"<<rese<<endl;
    cout<<"isequal?";
    SimpleBigint ggg((rese*b.N+SimpleBigint(2,0)));
    cout<<ggg<<"and"<<b.R<<endl;
 //   }
}
//N=11
void processTimeintervels(double&maxi,double&mini,double&avg){
    maxi=0,mini=UINT_MAX,avg=0;
    for(unsigned i=0;i<timeintervels.size();++i){
        double tmp=timeintervels[i];
        if(maxi<tmp)maxi=tmp;
        else if(mini>tmp)mini=tmp;
        avg+=tmp;
    }
    avg/=timeintervels.size();
    timeintervels.clear();
    vector<double>().swap(timeintervels);
}
/*
typedef SimpleBigint(*Montgomery)(const SimpleBigint&,const SimpleBigint&,const SimpleBigint&,unsigned);
void executemon(unsigned nums,Montgomery montgomeryfunc,unsigned bits=128){
    unsigned maxbits=1024<bits?1024:bits;
    srand(time(nullptr));//
    for(unsigned i=128;i<=maxbits;i*=2){
        for(unsigned k=0;k<nums;++k){
            SimpleBigint tmpbigint1,tmpbigint2,tmpN;
            for(unsigned j=0;j<i/32;++j){
                uint32_t randomv=rand()%UINT32_MAX;
                tmpbigint1.numbers.push_back(randomv);
                randomv=rand()%UINT32_MAX;
                tmpbigint2.numbers.push_back(randomv);
                randomv=rand()%UINT32_MAX;
                tmpN.numbers.push_back(randomv);
            }
            montgomeryfunc(tmpbigint1,tmpbigint2,tmpN,i/32);
        }
        double maxi,mini,avg;
        processTimeintervels(maxi,mini,avg);
        cout<<"bits="<<i<<",numbers="<<nums<<":maxi="<<maxi<<",mini="<<mini<<",avg="<<avg<<" us"<<endl;
    }
}*/
/*
void execute_simd(unsigned nums,unsigned bits=32){
    unsigned maxbits=1024<bits?1024:bits;
    srand(time(nullptr));
    for(unsigned i=128;i<=maxbits;i*=2){
        for(unsigned k=0;k<nums;++k){
            SimpleBigint tmpbigint1,tmpbigint2,tmpN;
            for(unsigned j=0;j<i/32;++j){
                uint32_t randomv=rand()%UINT32_MAX;
                tmpbigint1.numbers.push_back(randomv);
                randomv=rand()%UINT32_MAX;
                tmpbigint2.numbers.push_back(randomv);
                randomv=rand()%UINT32_MAX;
                tmpN.numbers.push_back(randomv);
            }
            simd_mulmod(tmpbigint1,tmpbigint2,tmpN,i/32);
        }
        double maxi,mini,avg;
        processTimeintervels(maxi,mini,avg);
        cout<<"bits="<<i<<",numbers="<<nums<<":maxi="<<maxi<<",mini="<<mini<<",avg="<<avg<<" us"<<endl;
    }
}*/
void testsimpleRSA(){
    while(1){
        string onemsg;
        cin>>onemsg;
        SimpleBigint msg(onemsg);
        cout<<"MSG: "<<msg<<endl;
        unsigned length;
        cin>>length;
        SimpleRSA rsame;
        if(!rsame.generateKey(length)){
            cout<<"quiting..."<<endl;
            break;
        }
        rsame.encryptme(msg);
    }
}
vector<SimpleBigint>generateMsgs(unsigned nums,unsigned length){
    srand(time(nullptr));//
    vector<SimpleBigint>msgsme;
    for(unsigned k=0;k<nums;++k){
        SimpleBigint tmp;
        unsigned upper=(length%32)?length/32+1:length/32;
        for(unsigned j=0;j<upper;++j){
            uint32_t randomv=rand()%UINT32_MAX;
            while(!randomv)randomv=rand()%UINT32_MAX;
            tmp.numbers.push_back(randomv);
        }
        msgsme.push_back(tmp);
    }
    return msgsme;
}
SimpleBigint generateMsg(unsigned length){
    srand(time(nullptr));//
    SimpleBigint msgsme;
    unsigned upper=(length%32)?length/32+1:length/32;
    for(unsigned j=0;j<upper;++j){
        uint32_t randomv=rand()%UINT32_MAX;
        while(!randomv)randomv=rand()%UINT32_MAX;
        msgsme.numbers.push_back(randomv);
    }
    return msgsme;
}
ThreadModexp tme;
void testthreadmodp(){
    unsigned length,msglength,thethread;
    cout<<"length=?"<<endl;
    cin>>length;
    cout<<"thread?"<<endl;
    cin>>thethread;
    msglength=length+32;
    //cout<<"msg length?"<<endl;
    //cin>>msglength;
    SimpleRSA toolrsa;
    toolrsa.generateKey(length);
    if(msglength/32<=toolrsa.n.numbersLength())cerr<<"error,selecet msgsize longer than length"<<endl;
    SimpleBigint num1=generateMsg(msglength);

    SimpleBigint exp(toolrsa.e),mod1(toolrsa.n);
    //SimpleBigint num1,exp,mod1("c8832ddcd9e6477890bf173fe81e911551ed14add9edfdd6fb9b8c5ef10f5db6eb3716590dcee0a6367bf960cb6fc2ac904f879f9065758106df053c7f0f06ad");
    //cin>>num1>>exp;
    #ifdef _MAIN_DEBUG_
    cout<<"num1="<<num1<<endl;
    cout<<"exp="<<exp<<endl;
    cout<<"n="<<mod1<<endl;
    #endif
    tme.initThreadModexp(num1,exp,mod1);
    #ifdef _MAIN_DEBUG_
    cout<<"initThreadModexp done"<<endl;
    #endif
    tme.initcmr();
    tme.setThreads(thethread);
    #ifdef _MAIN_DEBUG_
    cout<<"initcmr done"<<endl;
    #endif
    tme.assignThreadexp();
    #ifdef _MAIN_DEBUG_
    cout<<"ready to create thread..."<<endl;
    cout<<tme.cm<<endl<<tme.cmr<<endl<<tme.exp<<endl<<tme.mod<<endl<<tme.mym.A<<endl<<tme.mym.B<<endl<<tme.mym.N<<endl<<tme.mym.N_1<<endl<<tme.mym.R2modN<<endl<<tme.mym.R<<endl<<tme.threadexp[0]<<endl<<tme.threadexp[1]<<endl<<tme.threadexp[2]<<endl<<tme.threadexp[3]<<endl<<tme.threadRes<<endl<<tme.threads<<endl;
    #endif
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    threadmodexpMain();
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1.0/freq;
    cout<<"in thread-nonsimd , time used="<<intervel<<" s, result= "<<tme.threadRes<<endl;
    tme.clearRes();//清除原来计算的结果
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    threadmodexpMain(true);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<"in thread-simd , time used="<<intervel<<" s, result= "<<tme.threadRes<<endl;
    SimpleModExp sme(num1,exp,mod1);
    //cout<<sme.cm<<endl<<tme.cmr<<endl<<tme.exp<<endl<<tme.mod<<endl<<tme.mym.A<<endl<<tme.mym.B<<endl<<tme.mym.N<<endl<<tme.mym.N_1<<endl<<tme.mym.R2modN<<endl<<tme.mym.R<<endl<<tme.threadexp[0]<<endl<<tme.threadexp[1]<<endl<<tme.threadexp[2]<<endl<<tme.threadexp[3]<<endl<<tme.threadRes<<endl<<tme.threads<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    sme.modexp();
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<"in simple , time used="<<intervel<<" s, result= "<<sme.cmexp<<endl;//奇怪*/

}
SeveralMsgRSAC smr;
void testSeveralMsg(){
    unsigned length;
    cout<<"length=?"<<endl;
    cin>>length;
    smr.initSRSA(length);
    srand(time(nullptr));//
    cout<<"how many msgs?"<<endl;
    unsigned nums;
    cin>>nums;
    vector<SimpleBigint>msgsme=generateMsgs(nums,length+4);
    smr.initMsg(msgsme);
    cout<<"init msg done"<<endl;
    smr.assigntasks();
    cout<<"assign tasks done"<<endl;
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    //QueryPerformanceCounter((LARGE_INTEGER*)&head);
    //threadencryptmsg();
    //QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel;
    //intervel=(tail-head)*1.0/freq;
    //cout<<"in several-nonsimd , time used="<<intervel<<" s"<<endl;//45/10/512
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    threadencryptmsg(true);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<endl<<"in several-simd , time used="<<intervel<<" s"<<endl;//45/10/512
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    smr.simdencryptmsgs(0,msgsme.size());
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<endl<<"in non-several , time used="<<intervel<<" s"<<endl;//160/10/512
}
MultiThreadModexp mtme;
void testmultiThreadmodp(unsigned length,unsigned thethread,unsigned thelayer){
    /*unsigned length,thethread,thelayer;
    cout<<"length=?";
    cin>>length;
    cout<<"thread=?";
    cin>>thethread;
    cout<<"layer=?";
    cin>>thelayer;*/
    cout<<"length="<<length<<",threads="<<thethread<<",layers="<<thelayer<<endl;
    unsigned msglength;
    msglength=length+32;
    //cout<<"msg length?"<<endl;
    //cin>>msglength;
    SimpleRSA toolrsa;
    toolrsa.generateKey(length);
    if(msglength/32<=toolrsa.n.numbersLength())cerr<<"error,selecet msgsize longer than length"<<endl;
    SimpleBigint num1=generateMsg(msglength);

    SimpleBigint exp(toolrsa.e),mod1(toolrsa.n);
    //SimpleBigint num1,exp,mod1("c8832ddcd9e6477890bf173fe81e911551ed14add9edfdd6fb9b8c5ef10f5db6eb3716590dcee0a6367bf960cb6fc2ac904f879f9065758106df053c7f0f06ad");
    //cin>>num1>>exp;
    #ifdef _MAIN_DEBUG_
    cout<<"num1="<<num1<<endl;
    cout<<"exp="<<exp<<endl;
    cout<<"n="<<mod1<<endl;
    #endif
    mtme.initMultiThreadModexp(num1,exp,mod1);
    #ifdef _MAIN_DEBUG_
    cout<<"initThreadModexp done"<<endl;
    #endif
    mtme.initcmr();
    mtme.setThreadandLayer(thethread,thelayer);
    #ifdef _MAIN_DEBUG_
    cout<<"initcmr done"<<endl;
    #endif
    mtme.huafenKrsnum();
    #ifdef _MAIN_DEBUG_
    cout<<"ready to create thread..."<<endl;
    cout<<tme.cm<<endl<<tme.cmr<<endl<<tme.exp<<endl<<tme.mod<<endl<<tme.mym.A<<endl<<tme.mym.B<<endl<<tme.mym.N<<endl<<tme.mym.N_1<<endl<<tme.mym.R2modN<<endl<<tme.mym.R<<endl<<tme.threadexp[0]<<endl<<tme.threadexp[1]<<endl<<tme.threadexp[2]<<endl<<tme.threadexp[3]<<endl<<tme.threadRes<<endl<<tme.threads<<endl;
    #endif
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    multiSIMDThreadmodexpMain();
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000.0/freq;
    cout<<endl<<"in multi-thread-simd , time used="<<intervel<<" ms"<<endl<<"result= "<<mtme.threadRes<<endl;
/*    SimpleModExp sme(num1,exp,mod1);
    //cout<<sme.cm<<endl<<tme.cmr<<endl<<tme.exp<<endl<<tme.mod<<endl<<tme.mym.A<<endl<<tme.mym.B<<endl<<tme.mym.N<<endl<<tme.mym.N_1<<endl<<tme.mym.R2modN<<endl<<tme.mym.R<<endl<<tme.threadexp[0]<<endl<<tme.threadexp[1]<<endl<<tme.threadexp[2]<<endl<<tme.threadexp[3]<<endl<<tme.threadRes<<endl<<tme.threads<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    sme.modexp();
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1.0/freq;
    cout<<"in simple , time used="<<intervel<<" s, result= "<<sme.cmexp<<endl;//奇怪*/
    //if(thelayer<=2){//如果大于2就不用浪费时间了
    tme.initThreadModexp(num1,exp,mod1);
    #ifdef _MAIN_DEBUG_
    cout<<"initThreadModexp done"<<endl;
    #endif
    tme.initcmr();
    tme.setThreads(thethread);
    #ifdef _MAIN_DEBUG_
    cout<<"initcmr done"<<endl;
    #endif
    tme.assignThreadexp();
    #ifdef _MAIN_DEBUG_
    cout<<"ready to create thread..."<<endl;
    cout<<tme.cm<<endl<<tme.cmr<<endl<<tme.exp<<endl<<tme.mod<<endl<<tme.mym.A<<endl<<tme.mym.B<<endl<<tme.mym.N<<endl<<tme.mym.N_1<<endl<<tme.mym.R2modN<<endl<<tme.mym.R<<endl<<tme.threadexp[0]<<endl<<tme.threadexp[1]<<endl<<tme.threadexp[2]<<endl<<tme.threadexp[3]<<endl<<tme.threadRes<<endl<<tme.threads<<endl;
    #endif
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    threadmodexpMain(true);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1000.0/freq;
    cout<<endl<<"in thread-simd , time used="<<intervel<<" ms"<<endl<<"result= "<<tme.threadRes<<endl;
    cout<<"-------------------------------------------------"<<endl;
    //}
}
int main(int argc,char*argv[]){
    //unsigned nums=100;
    //cout<<"simd way"<<endl;
    //executemon(100,simd_mulmod,1024);
   //cout<<"simple way"<<endl;
    //executemon(100,simplemmulmod,1024);
    //comment: N must be a prime as our implementation of Hensel's lemma depends on the prime
    //if N is not a prime, N' will be calculated incorrectly and all the data in this program wrong
    //注释：N必须选择单数，因为有算法（Hensel等）依赖。
    //若N非素数，程序执行错误
    //1528D4A3DB
    //2.M不能太短，至少需要使得R>N
    //testMontMulAcc();
    //testsimpleRSA();
    //testMod();
    //testSeveralMsg();
    //testMontMulAcc();
    //testSeveralMsg();
    if(argc<=2)return 0;
    unsigned length=strtol(argv[1],nullptr,10);
    unsigned threads=strtol(argv[2],nullptr,10);
    unsigned layers=strtol(argv[3],nullptr,10);
    testmultiThreadmodp(length,threads,layers);
    //testthreadmodp();
    return 0;
}
