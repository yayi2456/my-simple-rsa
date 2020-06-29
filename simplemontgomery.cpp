#include"simplemontgomery.h"

//#define _SIMPLE_MONTGOMERY_DEBUG
SimpleBigint R2modN;
vector<double> timeintervels;

//扩展的欧几里得算法，
//用来计算x0^{-1}
uint32_t extendEuclideanx0_1_32(uint32_t x0){
    //return x0_1
    int s0=1,s1=0,t0=0,t1=1;
    uint64_t r0=pow(2,32),r1=x0;
    while(r1!=0){
        uint64_t q=r0/r1;
        uint64_t tmp=r0;
        r0=r1;
        r1=tmp-q*r1;
        tmp=s0;
        s0=s1;
        s1=tmp-q*s1;
        tmp=t0;
        t0=t1;
        t1=tmp-q*t1;
        //cout<<"s0="<<s0<<",s1="<<s1<<",t0="<<t0<<",t1="<<t1<<endl;
    }
    if(r0!=1)return 0;
    else{
        return t0;
    }
}

//calR2modN 函数计算R^2 mod N
SimpleBigint MyMontgomery::calR2modN(){
    SimpleBigint t=R%N;
    //cout<<"R%N="<<t<<endl;
    t*=t;
    t%=N;
    //cout<<"R2modN_S="<<t<<endl;
    return t;
}

//使用亨泽尔引理计算N'，
//N' 用来reduce
SimpleBigint MyMontgomery::calN_1(){
    //f(x)=N*r+1
    //SimpleBigint p(2,0);//p=2
    uint32_t p=2;
    SimpleBigint pk(2,0);//2^1
    SimpleBigint rk(1,0);//k=1,N*N'=-1 mod 2,N'=1
    //SimpleBigint fx_1(N);
    uint32_t t=0;
    SimpleBigint frk(N*rk+SimpleBigint(1,0));
    for(unsigned k=2;k<=32*(R.numbersLength()-1);++k){
        //f(r)=N*N'+1
        //cout<<"frk="<<frk<<endl;
        SimpleBigint tfr_1;//t mod p = p-tfr_1 mod p
        tfr_1=frk.moveby2_divide(k-1);
        //cout<<"k-1=0x"<<k-1<<" frk=0x"<<frk<<" tfr_1=0x"<<tfr_1<<endl;
        //SimpleBigint np(p);//cout<<"can i find"<<endl;
        //t=0/1;t*N=-tfr_1 mod p --> tN=-tfr_1 mod 2;N is a prime,t \in [0,1]
        //if t==0 then tfr_1.getbyindex(0)%2!=1 --> 偶数
        //if t==1 then trf_1.getbyindex(0)%2!=0 --> 奇数
        //这样省去驱魔的过程

         if(tfr_1.getbyIndex(0)%2!=0){
            //奇数
            //cout<<"\n\nupdate:"<<tfr_1.getbyIndex(0)<<endl;
            t=1;
        }
        else if(tfr_1.getbyIndex(0)%2!=1){
            t=0;
        }

        /*while(tfr_1>np){
            np+=p;
        }//cout<<"find p"<<endl;
        tfr_1=np-tfr_1;//cout<<"hhh"<<endl;
        //tN=tfr_1 mod p;tN=tfr_1 mod 2;N is a prime,t \in [0,1]
        if(tfr_1.numbersLength()<=0)t=0;//
        else if(tfr_1.getbyIndex(0)%2!=0){
            //奇数
            t=1;
        }
        else if(tfr_1.getbyIndex(0)%2!=1){
            t=0;
        }*/
        //cout<<"tfr_1="<<tfr_1<<endl;
        if(t){
            rk=(rk+pk);
            frk=N*rk+SimpleBigint(1,0);
        }
        //cout<<" pk=0x"<<pk<<" t=0x"<<t<<" rk=0x"<<rk<<endl;
        //else rk=rk;
        pk=pk*p;
        //cout<<"index="<<k<<",t="<<t<<",pk="<<pk<<",rk="<<rk<<endl;
    }
    //cout<<"N_1="<<rk<<endl;
    return rk;
}
//myMontgomeryTo 将大数转入蒙哥马利形式
///isA为true则转A，否则转B
SimpleBigint MyMontgomery::myMontgomeryTo(bool isA){
    //S=REDC((a mod N)*(R^2 mod N))
/*    SimpleBigint TmodN,REDCT;
    if(isA)TmodN=A%N;
    else TmodN=B%N;
    //cout<<"TmodN="<<TmodN<<endl;
    REDCT=(R2modN*TmodN)%N;
    //cout<<"REDCT="<<REDCT<<endl;
    //SimpleBigint S;
    //S=simpleMontgomeryReduction(REDCT,N);
    SimpleBigint R(1,TSIZE-1);
    SimpleBigint S((R*T)%N);
    //cout<<"to m form"<<":T="<<T<<",TR="<<S<<endl;
    return S;*/
    SimpleBigint m,t;
    SimpleBigint tmp;
    if(isA)tmp=A%N;
    else tmp=B%N;
    SimpleBigint result,value=tmp*R2modN;//R2modN->R
    //value=value%N;
    //cout<<"change to M-form: "<<value<<endl;
    result=myMontgomeryReduction(value);
    //result=value;
    #ifdef _SIMPLE_MONTGOMERY_DEBUG
    cout<<"myMontgomeryTo: isA="<<isA<<",res="<<result<<endl;
    cout<<"A="<<A<<",R="<<R<<endl;
    #endif
    return result;

}

//myMontgomeryMul 对两个蒙哥马利形式的大数进行莫乘运算，返回ARBRR^{-1}mod N
SimpleBigint MyMontgomery::myMontgomeryMul( SimpleBigint&AR, SimpleBigint&BR){
    SimpleBigint D;
    #ifdef _SIMPLE_MONTGOMERY_DEBUG
    cout<<"AR="<<AR<<"BR="<<(BR)<<endl;
    #endif
    const uint32_t ar0=AR.getbyIndex(0);
    const uint32_t n0=N.getbyIndex(0);
    uint32_t n0_1=extendEuclideanx0_1_32(n0);
    uint32_t d0=0;
    uint32_t q0=0;
    for(int i=0;i<R.numbersLength()-1;++i){//R->BR
        uint32_t bri=BR.getbyIndex(i);
        D+=(AR*bri);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        cout<<"bri="<<bri<<",AR*bri="<<(AR*bri)<<",D="<<D<<endl;
        #endif
        uint64_t tmp1=/*D.getbyIndex(0)*/(d0+ar0*bri),tmp2=(UINT32_MAX-n0_1+1);//包括了取模操作
        q0=tmp1*tmp2;//需要取模
        SimpleBigint q(q0,0);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        cout<<"tmp1="<<tmp1<<",tmp2="<<tmp2<<",q="<<q<<endl;
        #endif
        D+=(N*q);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        cout<<"N*q="<<N*q<<",D="<<D<<endl;
        #endif
        D.shiftDivide32();
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        cout<<"D="<<D<<endl;
        #endif
        d0=D.getbyIndex(0);
    }
    //return D%N;
    if(N>D)return D;
    else return D-N;
}
//myMontgomeryReduction 对ABR mod N做reduction，返回AB mod N
//本算法参考wiki
SimpleBigint MyMontgomery::myMontgomeryReduction( SimpleBigint ResR){
    SimpleBigint m;
    #ifdef _DEBUG
    cout<<"resr="<<ResR<<endl;
    #endif
    m=(N_1*(ResR.getfirst(R.numbersLength()-1))).getfirst(R.numbersLength()-1);
    #ifdef _DEBUG
    cout<<"m="<<m<<endl;
    #endif
    SimpleBigint t;
    t=(ResR+N*m);
    #ifdef _DEBUG
    cout<<"t="<<t<<endl;
    #endif
    t=t.getlast(t.numbersLength()-R.numbersLength()+1);
    #ifdef _DEBUG
    cout<<"t="<<t<<endl;
    #endif

    if(t>=N)return t-N;
    return t;
}