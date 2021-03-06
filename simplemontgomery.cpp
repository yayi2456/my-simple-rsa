#include"simplemontgomery.h"

//#define _SIMPLE_MONTGOMERY_DEBUG
//#define _SIMPLE_MONTGOMERY_DEBUG_AVX
//#define _TIME_TEST_
SimpleBigint R2modN;
vector<double> timeintervels;

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

SimpleBigint MyMontgomery::calR2modN(){
    #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    SimpleBigint t=R%N;
    #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000.0/freq;
    cout<<"in calR2modN-R%N, time used="<<intervel<<" ms"<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    //cout<<"R%N="<<t<<endl;
    t*=t;
    #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1000.0/freq;
    cout<<"in calR2modN-t*t, time used="<<intervel<<" ms"<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    t%=N;
    #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1000.0/freq;
    cout<<"in calR2modN-t%N, time used="<<intervel<<" ms"<<endl;
    #endif
    //cout<<"R2modN_S="<<t<<endl;
    return t;
}
//使用亨泽尔引理计算N'
SimpleBigint MyMontgomery::calN_1(){
    //f(x)=N*r+1
    //SimpleBigint p(2,0);//p=2
    uint32_t p=2;
    SimpleBigint pk(2,0);//2^1
    SimpleBigint rk(1,0);//k=1,N*N'=-1 mod 2,N'=1
    //SimpleBigint fx_1(N);
    uint32_t t=0;
    SimpleBigint frk(N+SimpleBigint(1,0));
    SimpleBigint ntmp=N;
    #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
    long long head,freq,tail;
    double summove=0,sumgetby=0,summul=0,sumrk=0,sumrk1=0,sumrk2=0,sumrkres=0;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq);
    #endif
    for(unsigned k=2;k<=32*(R.numbersLength()-1);++k){
        //f(r)=N*N'+1
        //cout<<"frk="<<frk<<endl;
        SimpleBigint tfr_1;//t mod p = p-tfr_1 mod p
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        tfr_1=frk.moveby2_divide(k-1);
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        summove+=(tail-head)*1000.0/freq;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
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
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        sumgetby+=(tail-head)*1000.0/freq;
        #endif
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        //ntmp=ntmp*2;//
        ntmp.moveby2_mul();
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        sumrk1+=(tail-head)*1000.0/freq;
        #endif
        //cout<<"tfr_1="<<tfr_1<<endl;
        if(t){
            #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
            QueryPerformanceCounter((LARGE_INTEGER*)&head);
            #endif
            rk=(rk+pk);
            //rk=1+t1*2+t2*2^2+...+tn-1*2^n-1
            #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
            QueryPerformanceCounter((LARGE_INTEGER*)&tail);
            sumrkres+=(tail-head)*1000.0/freq;
            QueryPerformanceCounter((LARGE_INTEGER*)&head);
            #endif
            //frk=((N+1+(t1*2))N+1+(t2*2*2))N+1...
            //frk=N*rk+SimpleBigint(1,0);
            frk=frk+ntmp;
            //if(!(frk==debug_frk))cerr<<"\n\n==========\n\n";
            #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
            QueryPerformanceCounter((LARGE_INTEGER*)&tail);
            sumrk2+=(tail-head)*1000.0/freq;
            #endif
        }
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        sumrk+=(tail-head)*1000.0/freq;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        //cout<<" pk=0x"<<pk<<" t=0x"<<t<<" rk=0x"<<rk<<endl;
        //else rk=rk;
        //pk=pk*p;
        pk.moveby2_mul();
        //cout<<"pk="<<pk<<endl;
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        summul+=(tail-head)*1000.0/freq;
        #endif
        //cout<<"index="<<k<<",t="<<t<<",pk="<<pk<<",rk="<<rk<<endl;
    }
    //cout<<"N_1="<<rk<<endl;
    #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
    cout<<"in mym-caln1-moveby2, time used="<<summove<<" ms"<<endl;
    cout<<"in mym-caln1-getbyndex, time used="<<sumgetby<<" ms"<<endl;
    cout<<"in mym-caln1-rk1, time used="<<sumrk1<<" ms"<<endl;
    cout<<"in mym-caln1-rkres, time used="<<sumrkres<<" ms"<<endl;
    cout<<"in mym-caln1-rk2, time used="<<sumrk2<<" ms"<<endl;
    cout<<"in mym-caln1-mul, time used="<<summul<<" ms"<<endl;
    #endif
    return rk;
}
SimpleBigint MyMontgomery::myMontgomeryTo(bool isA){
    //S=REDC((a mod N)*(R^2 mod N))
    SimpleBigint m,t;
    SimpleBigint tmp;
    if(isA)tmp=A;//%N;
    else tmp=B;//%N;
    SimpleBigint result,value=tmp*R2modN;
    //cout<<"change to M-form: "<<value<<endl;
    result=myMontgomeryReduction(value);
    return result;
}

SimpleBigint MyMontgomery::myMontgomeryMul( SimpleBigint&AR, SimpleBigint&BR){
    SimpleBigint D;
    const uint32_t ar0=AR.getbyIndex(0);
    const uint32_t n0=N.getbyIndex(0);
    uint32_t n0_1=extendEuclideanx0_1_32(n0);
    uint32_t d0=0;
    uint32_t q0=0;
    for(int i=0;i<R.numbersLength()-1;++i){
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

///修改版simd
//用于计算AR*bri
SimpleBigint simdMulBig_32(SimpleBigint X,uint32_t y0){
    unsigned xlength=X.numbersLength();
    if(xlength%2!=0)X.numbers.push_back(0);//不影响，凑数的，以防内存读取到其他数值
    #ifdef _SIMPLE_MONTGOMERY_DEBUG
    cout<<"SIMD:X="<<X<<endl;
    #endif
    uint32_t*res1=(uint32_t*)malloc((xlength+1)*sizeof(uint32_t));
    uint32_t*c_1=(uint32_t*)malloc((xlength+2)*sizeof(uint32_t));
    memset(res1,0,(xlength+1)*sizeof(uint32_t));
    memset(c_1,0,(xlength+2)*sizeof(uint32_t));
    uint32_t extend_y0[4];
    extend_y0[0]=extend_y0[1]=extend_y0[2]=extend_y0[3]=y0;
    __m128i dxi,dy0,res128;
    for(unsigned i=0;i<xlength;i+=2){
        //数据取入
        //dxi=_mm_loadqu_si128((__m128i*)(X+i));//a3,a2,a1,a0
        //dy0=_mm_loadqu_si128((__m128i*)extend_y0);//0s0s
        dxi=_mm_loadl_epi64((__m128i*)(&X.numbers[i]));//0,0,x1,x0
        dy0=_mm_loadl_epi64((__m128i*)extend_y0);//0,0,y0,y0
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        uint32_t tmp[4];
        _mm_store_si128((__m128i*)(tmp),dxi);
        cout<<"dxi:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        _mm_store_si128((__m128i*)(tmp),dy0);
        cout<<"dy0:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        #endif
        dxi=_mm_shuffle_epi32(dxi,0xd8);//0,x1,0x,x0
        dy0=_mm_shuffle_epi32(dy0,0xd8);//0,y0,0,y0
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        _mm_store_si128((__m128i*)(tmp),dxi);
        cout<<"dxi:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        _mm_store_si128((__m128i*)(tmp),dy0);
        cout<<"dy0:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        #endif
        res128=_mm_mul_epu32(dxi,dy0);//c1,r1,c0,r0
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        _mm_store_si128((__m128i*)(tmp),res128);
        cout<<"res128:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        #endif
        res128=_mm_shuffle_epi32(res128,0xd8);//c1,c0,r1,r0
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        _mm_store_si128((__m128i*)(tmp),res128);
        cout<<"res128:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        #endif
        _mm_storel_epi64((__m128i*)(res1+i),res128);
        res128=_mm_shuffle_epi32(res128,0x4e);//r1,r0,c1,c0
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        _mm_store_si128((__m128i*)(tmp),res128);
        cout<<"res128:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        #endif
        //难以有效处理c0+r1产生的进位，
        //进位操作
        //目前将数据存储，不以SIMD方式处理加法
        //直接使用大数加法完成相加的部分
        _mm_storel_epi64((__m128i*)(c_1+i+1),res128);
    }
    //无需处理剩余数据：多申请了32bit空间
    //1. 若xlength为奇数，最后一次处理中得到的是0,cx,0,rx
    //2. 如果xlength是偶数，最后一次处理是cx,cx-1,rx,rx-1，最后多申请的0没有使用
    SimpleBigint resme(res1,0,xlength),cme(c_1,0,xlength+1);
    resme.trimnumber();cme.trimnumber();X.trimnumber();
    SimpleBigint resres=resme+cme;

    #ifdef _SIMPLE_MONTGOMERY_DEBUG
    for(int i=0;i<xlength+1;++i){
        cout<<res1[i]<<" ";
    }cout<<endl;
    for(int i=1;i<xlength+2;++i){
        cout<<c_1[i]<<" ";
    }cout<<endl;
    cout<<"resme="<<resme<<",cme="<<cme<<endl;
    cout<<"resres="<<resres<<endl;
    #endif
    free(res1);free(c_1);
    return resres;

}

SimpleBigint MyMontgomery::simdMontgomeryMul(SimpleBigint&AR, SimpleBigint&BR){
    SimpleBigint D;
    const uint32_t ar0=AR.getbyIndex(0);
    const uint32_t n0=N.getbyIndex(0);
    uint32_t n0_1=extendEuclideanx0_1_32(n0);
    uint32_t q=0,d0=0;
    uint64_t tmp2=(UINT32_MAX-n0_1+1);
    for(int i=0;i<R.numbersLength()-1;++i){
        uint32_t bri=BR.getbyIndex(i);
        D+=simdMulBig_32(AR,bri);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        cout<<"bri="<<bri<<",AR*bri="<<(AR*bri)<<",D="<<D<<endl;
        #endif
        uint64_t tmp1=(d0+ar0*bri);
        q=tmp1*tmp2;//需要取模
        //SimpleBigint q(q0,0);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        cout<<"tmp1="<<tmp1<<",tmp2="<<tmp2<<",q="<<q<<endl;
        #endif
        D+=simdMulBig_32(N,q);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        cout<<"N*q="<<N*q<<",D="<<D<<endl;
        #endif
        D.shiftDivide32();
        #ifdef _SIMPLE_MONTGOMERY_DEBUG
        cout<<"D="<<D<<endl;
        #endif
        d0=D.getbyIndex(0);
    }
    D.trimnumber();
    if(N>D)return D;
    else return D-N;
}


///avx
///256
//用于计算X*y0
SimpleBigint simdMulBig_32_256(SimpleBigint X,uint32_t y0){
    unsigned xlength=X.numbersLength();
    //将X做成128/32=4的整数个数据
    unsigned zerosNeeded=4-xlength%4;
    zerosNeeded==4?zerosNeeded=0:zerosNeeded=zerosNeeded;
    for(int i=0;i<zerosNeeded;++i){
        X.numbers.push_back(0);
    }
    xlength+=zerosNeeded;
    #ifdef _SIMPLE_MONTGOMERY_DEBUG_AVX
    cout<<"SIMD-AVX:X="<<X<<endl;
    cout<<"y0="<<y0<<endl;
    cout<<"xlength="<<xlength<<endl;
    cout<<"zerosNeeded="<<zerosNeeded<<endl;
    #endif
    #ifdef _TIME_TEST_
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    //res
    uint32_t*res1=(uint32_t*)malloc((xlength)*sizeof(uint32_t));
    //进位数据
    uint32_t*c_1=(uint32_t*)malloc((xlength+1)*sizeof(uint32_t));
    memset(res1,0,(xlength)*sizeof(uint32_t));
    memset(c_1,0,(xlength+1)*sizeof(uint32_t));
    uint32_t extend_y0[8];
    extend_y0[0]=extend_y0[2]=extend_y0[4]=extend_y0[6]=y0;
    extend_y0[1]=extend_y0[3]=extend_y0[5]=extend_y0[7]=0;
    uint64_t loadmask[4];
    //只load低位数据
    loadmask[0]=loadmask[1]=0xFFFFFFFFFFFFFFFF;
    loadmask[2]=loadmask[3]=0;
    __m256i loadmasklowbitonly=_mm256_loadu_si256((__m256i*)(loadmask));
    uint64_t storemask[4];
    storemask[0]=storemask[1]=0;
    storemask[2]=storemask[3]=0xFFFFFFFFFFFFFFFF;
    __m256i storemaskhighbitsonly=_mm256_loadu_si256((__m256i*)(storemask));
    /*uint64_t loadmask0[4];
    loadmask0[0]=loadmask0[1]=0;
    loadmask0[2]=loadmask0[3]=0;*/
    __m256i dxi,dy0,res256;
    //__m128i resr,resc;//,resraddrescr,resraddrescc;
    //__m256i allzero;
    //allzero=_mm256_loadu_si256((__m256i*)(&loadmask0[0]));
    /*uint32_t idx[8];
    idx[0]=4;idx[1]=0;idx[2]=1;idx[3]=2;idx[4]=3;idx[5]=5;idx[6]=6;idx[7]=7;
    __m256i idxchoose32t=_mm256_loadu_si256((__mm256i*)(&idx[0]));
    uint64_t bitwiseand[4];
    bitwiseand[0]=bitwiseand[1]=bitwiseand[2]=bitwiseand[3]=1;
    __m256i all4ones=_mm256_loadu_si256((__mm256i*)(&bitwiseand[0]));*/
    dy0=_mm256_loadu_si256((__m256i*)extend_y0);//0,y0,0,y0,0,y0,0,y0
    for(unsigned i=0;i<xlength;i+=4){
        //数据取入
        //dxi=_mm256_loadu_si256((__m256i*)(&X.numbers[i]));//xx,xx,xx,xx,x3,x2,x1,x0
        //dxi=_mm256_permute2f128_si256(dxi,allzero,0x20);//0,0,0,0,x3,x2,x1,x0
        //dy0=_mm256_loadu_si256((__m256i*)extend_y0);//0,0,0,0,y0,y0,y0,y0
        //dxi=_mm256_maskload_epi64((__int64*)(&X.numbers[i]),loadmasklowbitonly);//0,0,0,0,x3,x2,x1,x0
        dxi=_mm256_loadu_si256((__m256i*)(&X.numbers[i]));//xx,xx,xx,xx,x3,x2,x1,x0
        //dy0=_mm256_maskload_epi64((__int64*)(extend_y0),loadmasklowbitonly);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG_AVX
        uint32_t tmp[4];
        uint32_t tmp3[8];
        _mm256_storeu_si256((__m256i*)tmp3,dxi);
        cout<<"acutal dxi:"<<tmp3[7]<<" "<<tmp3[6]<<" "<<tmp3[5]<<" "<<tmp3[4]<<" "<<tmp3[3]<<" "<<tmp3[2]<<" "<<tmp3[1]<<" "<<tmp3[0]<<endl;
        _mm256_storeu_si256((__m256i*)tmp3,dy0);
        cout<<"acutal dy0:"<<tmp3[7]<<" "<<tmp3[6]<<" "<<tmp3[5]<<" "<<tmp3[4]<<" "<<tmp3[3]<<" "<<tmp3[2]<<" "<<tmp3[1]<<" "<<tmp3[0]<<endl;
        #endif
        dxi=_mm256_permute4x64_epi64(dxi,0xd8);//0,0,x3,x2,0,0,x1,x0
        //dy0=_mm256_permute4x64_epi64(dy0,0xd8);
        dxi=_mm256_shuffle_epi32(dxi,0xd8);//0,x3,0,x2,0,x1,0,x0
        //dy0=_mm256_shuffle_epi32(dy0,0xd8);//0,y0,0,y0,0,y0,0,y0

        res256=_mm256_mul_epu32(dxi,dy0);//c3,r3,c2,r2,c1,r1,c0,r0
        res256=_mm256_shuffle_epi32(res256,0xd8);//c3,c2,r3,r2,c1,c0,r1,r0
        res256=_mm256_permute4x64_epi64(res256,0xd8);//c3,c2,c1,c0,r3,r2,r1,r0

        _mm256_maskstore_epi64((__int64*)(res1+i),loadmasklowbitonly,res256);//低位存储
        _mm256_maskstore_epi64((__int64*)(c_1+i-3),storemaskhighbitsonly,res256);//高位存储
        //resr=_mm256_extracti128_si256(res256,0x0);//r3,r2,r1,r0
        //resc=_mm256_extracti128_si256(res256,0x1);//c3,c2,c1,c0
        /*resr=_mm256_unpacklo(res256,allzero);//0,0,r3,r2,0,0,r1,r0
        resr=_mm256_permute4x64_epi64(resr,0xd8);//0,0,0,0,r3,r2,r1,r0
        resc=_mm256_unpackhi(res256,allzeor);//0,0,c3,c2,0,0,c1,c0
        resc=_mm256_permute4x64_epi64(resc,0xd8);//0,0,0,0,c3,c2,c1,c0
*/
        //resc=_mm256_permutevar8x32_epi32(resc,idxchoose32t);//0,0,0,c3,c2,c1,c0,0
        //计算加和
        //resraddrescr=_mm256_add_epi64(resr,resc);//0,0,0,c3,rr3,rr2,rr1,rr0
        //上述加和，高128位不可能进位，低128位，每64位可能存在一个进位，
        //分析就可知，进位只可能是1或者0.
        //如果是1，则rr*<c*且rr*<r*，
        //如果rr*<c*或者rr*<r*，一定能够断定存在进位
        //resraddrescc=_mm256_cmpgt_epi64(resr,resraddrescr);//如果存在进位，则那64bit全为1//0,0,cc1',cc0'
        //resraddrescc=_mm256_and_si256(resraddrescc,all4ones);//如果有进位，对应的64bit低位是1//0,0,cc1,cc0
        //resraddrescc=_mm256_permute4x64_epi64(resraddrescc,0xd2);//0,cc1,cc0,0
        //现在应得到2*8个32bit的数据:C=0,0,0,cc1,0,cc2,0,0
        //Res=0,0,0,c3,rr3,rr2,rr1,rr0
        //实际上只要2*5个就可以了：C'=cc1,0,cc2,0,0; Res'=c3,rr3,rr2,rr1,rr0
        //C已经移位妥当

        //_mm_storeu_si128((__m128i*)(&res1[i]),resr);
        //_mm_storeu_si128((__m128i*)(c_1+i+1),resc);

        #ifdef _SIMPLE_MONTGOMERY_DEBUG_AVX
        _mm256_maskstore_epi64((__int64*)(tmp),loadmasklowbitonly,res256);//低位存储
        cout<<"resr:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        _mm256_maskstore_epi64((__int64*)(tmp-4),storemaskhighbitsonly,res256);//高位存储
        cout<<"resc:"<<tmp[3]<<" "<<tmp[2]<<" "<<tmp[1]<<" "<<tmp[0]<<endl;
        uint32_t tmp2[8];
        _mm256_maskstore_epi64((__int64*)(tmp2),loadmasklowbitonly,res256);//低位存储
        cout<<"resr:"<<tmp2[7]<<" "<<tmp2[6]<<" "<<tmp2[5]<<" "<<tmp2[4]<<" "<<tmp2[3]<<" "<<tmp2[2]<<" "<<tmp2[1]<<" "<<tmp2[0]<<endl;
        _mm256_maskstore_epi64((__int64*)(tmp2),storemaskhighbitsonly,res256);//高位存储
        cout<<"resc:"<<tmp2[7]<<" "<<tmp2[6]<<" "<<tmp2[5]<<" "<<tmp2[4]<<" "<<tmp2[3]<<" "<<tmp2[2]<<" "<<tmp2[1]<<" "<<tmp2[0]<<endl;
        _mm256_storeu_si256((__m256i*)tmp2,res256);
        cout<<"acutal res256:"<<tmp2[7]<<" "<<tmp2[6]<<" "<<tmp2[5]<<" "<<tmp2[4]<<" "<<tmp2[3]<<" "<<tmp2[2]<<" "<<tmp2[1]<<" "<<tmp2[0]<<endl;
        cout<<"end-"<<endl;
        #endif
    }
    #ifdef _TIME_TEST_
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000000.0/freq;
    cout<<"in AVX, time used="<<intervel<<" us"<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    //X被指为4的整数倍，不会有多余数据
    SimpleBigint resme(res1,0,xlength),cme(c_1,0,xlength+1);
    resme.trimnumber();cme.trimnumber();X.trimnumber();
    SimpleBigint resres=resme+cme;
    #ifdef _TIME_TEST_
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1000000.0/freq;
    cout<<"in SIMPLE-ADD, time used="<<intervel<<" us"<<endl;
    #endif
    #ifdef _SIMPLE_MONTGOMERY_DEBUG_AVX
    for(int i=0;i<xlength+1;++i){
        cout<<res1[i]<<" ";
    }cout<<endl;
    for(int i=1;i<xlength+2;++i){
        cout<<c_1[i]<<" ";
    }cout<<endl;
    cout<<"resme="<<resme<<",cme="<<cme<<endl;
    cout<<"resres="<<resres<<endl;
    #endif
    free(res1);free(c_1);
    return resres;
}

SimpleBigint MyMontgomery::simdMontgomeryMul_avx256(SimpleBigint&AR, SimpleBigint&BR){
    SimpleBigint D;
    const uint32_t ar0=AR.getbyIndex(0);
    const uint32_t n0=N.getbyIndex(0);
    uint32_t n0_1=extendEuclideanx0_1_32(n0);
    uint32_t q=0,d0=0;
    uint64_t tmp2=(UINT32_MAX-n0_1+1);
    for(int i=0;i<R.numbersLength()-1;++i){
        uint32_t bri=BR.getbyIndex(i);
        D+=simdMulBig_32_256(AR,bri);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG_AVX
        cout<<"AVX-256:("<<i<<"/"<<R.numbersLength()-1<<")"<<endl;
        cout<<"bri="<<bri<<",AR*bri="<<(AR*bri)<<",D="<<D<<endl;
        #endif
        uint64_t tmp1=(d0+ar0*bri);
        q=tmp1*tmp2;//需要取模
        //SimpleBigint q(q0,0);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG_AVX
        cout<<"tmp1="<<tmp1<<",tmp2="<<tmp2<<",q="<<q<<endl;
        #endif
        D+=simdMulBig_32_256(N,q);
        #ifdef _SIMPLE_MONTGOMERY_DEBUG_AVX
        cout<<"N*q="<<N*q<<",D="<<D<<endl;
        #endif
        D.shiftDivide32();
        #ifdef _SIMPLE_MONTGOMERY_DEBUG_AVX
        cout<<"D="<<D<<endl;
        #endif
        d0=D.getbyIndex(0);
    }
    D.trimnumber();
    if(N>D)return D;
    else return D-N;
}

/*
void simd_muladd(uint32_t*dest,uint32_t*bignum,uint32_t scalar,unsigned length){
    //本方法参考2004-Parallel Cryptographic Arithmetic Using a Redundant Montgomery Representation
    if(length%4!=0)length=length/4;
    uint32_t extend_scalar[4];
    extend_scalar[0]=scalar;extend_scalar[1]=0;extend_scalar[2]=scalar;extend_scalar[3]=0;
    __m128i d0,d1,d2,d3,d5,c0,c1;
    __m128i Mask13;
    uint32_t mask[4]={UINT32_MAX,0,UINT32_MAX,0};
    Mask13=_mm_load_si128((__m128i*)&mask);//mask1&3
    c0=_mm_setzero_si128();
    c1=_mm_setzero_si128();
    for(unsigned i=0;i<length;i+=4){
        //数据取入
        d0=_mm_load_si128((__m128i*)(bignum+i));//a3,a2,a1,a0
        d1=_mm_load_si128((__m128i*)extend_scalar);//0s0s
        d2=_mm_loadu_si128((__m128i*)(dest+i));//b3,b2,b1,b0
  //      cout<<"in data:"<<length<<","<<i<<",bignum:"<<bignum[i]<<" "<<bignum[i+1]<<" "<<bignum[i+2]<<" "<<bignum[i+3]<<",dest:";
  //      cout<<"i="<<i<<"  "<<dest[i]<<" "<<dest[i+1]<<" "<<dest[i+2]<<" "<<dest[i+3]<<endl;
        //shuffle：分别计算3，1以及2，0
//        uint32_t tmp[4];
 //       _mm_store_si128((__m128i*)(tmp),d0);
  //      cout<<"d0:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
   //     _mm_store_si128((__m128i*)(tmp),d2);
   //     cout<<"d2:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
        d3=_mm_shuffle_epi32(d0,0xb1);//a2,a3,a0,a1
        d5=_mm_shuffle_epi32(d2,0xb1);//b2,b3,b0,b1

        //相乘
        d0=_mm_mul_epu32(d0,d1);//0,a2s,0,a0s
        d1=_mm_mul_epu32(d3,d1);//0,a3s,0,a1s
 //       _mm_storeu_si128((__m128i*)(tmp),d0);
   //     cout<<"d0:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
  //      _mm_storeu_si128((__m128i*)(tmp),d1);
 //    //   cout<<"d1:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
        //将b加到d0,d1
        d2=_mm_and_si128(d2,Mask13);
        d0=_mm_add_epi64(d0,d2);//64位运算，得到进位
        d0=_mm_add_epi64(d0,c0);
        d5=_mm_and_si128(d5,Mask13);
        d1=_mm_add_epi64(d1,d5);
        d1=_mm_add_epi64(d1,c1);
 //       _mm_storeu_si128((__m128i*)(tmp),d0);
   //     cout<<"d0:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
 //       _mm_storeu_si128((__m128i*)(tmp),d1);
   //     cout<<"d1:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
        //再次shuffle改变b0,b1中的内容顺序
        d0=_mm_shuffle_epi32(d0,0xb1);
        d1=_mm_shuffle_epi32(d1,0xb1);
 /       _mm_storeu_si128((__m128i*)(tmp),d0);
        cout<<"d0:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
        _mm_storeu_si128((__m128i*)(tmp),d1);
        cout<<"d1:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;/
        //Mask得到进位
        c0=_mm_and_si128(d0,Mask13);
        c1=_mm_and_si128(d1,Mask13);
 //       _mm_storeu_si128((__m128i*)(tmp),c0);
 //       cout<<"c0:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
 //       _mm_storeu_si128((__m128i*)(tmp),c1);
 //       cout<<"c1:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
        //shuffle以便unpack
        d0=_mm_shuffle_epi32(d0,0xd8);
        d1=_mm_shuffle_epi32(d1,0xd8);
 /       _mm_storeu_si128((__m128i*)(tmp),d0);
       cout<<"d0:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;
        _mm_storeu_si128((__m128i*)(tmp),d1);
        cout<<"d1:"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<endl;/
        //unpack
        d2=_mm_unpackhi_epi32(d0,d1);

        //cout<<"i store"<<endl;
        //cout<<i<<TSIZE<<endl;
       // uint32_t tmp[4]={0};
        //_mm_store_si128((__m128i*)tmp,d2);
        //cout<<tmp[0]<<tmp[1]<<tmp[2]<<tmp[3]<<(dest[i+length]=6)<<endl;
        _mm_storeu_si128((__m128i*)(dest+i),d2);
        //cout<<"1done "<<dest[i]<<" "<<dest[i+1]<<" "<<dest[i+2]<<" "<<dest[i+3]<<endl;
    }
    //无剩余数据：TSIZE被补齐成4的倍数
}*/

/*SimpleBigint simplemmulmod(const SimpleBigint&A,const SimpleBigint&B,const SimpleBigint&N,unsigned length){
    SimpleBigint AR(simpleMontgomeryTo(A,N)),BR(simpleMontgomeryTo(B,N));
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint D(simpleMontgomeryMul(AR,BR,N));
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000000.0/freq;
    timeintervels.push_back(intervel);
    //SimpleBigint D(simd_MontgomeryMul(AR,BR,N));
    SimpleBigint Res(D*SimpleBigint(1,TSIZE)%N);
    //SimpleBigint Res(simpleMontgomeryReduction(D,N));
    return Res;
}*/
//SimpleBigint simd_mulmod(const SimpleBigint&A,const SimpleBigint&B,const SimpleBigint&N,unsigned length/*=TSIZE*/){
 /*   SimpleBigint AR(simpleMontgomeryTo(A,N)),BR(simpleMontgomeryTo(B,N));
    //SimpleBigint D(simpleMontgomeryMul(AR,BR,N));
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    SimpleBigint D(simd_MontgomeryMul(AR,BR,N,length));
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000000.0/freq;
    timeintervels.push_back(intervel);
    SimpleBigint Res(D*SimpleBigint(1,TSIZE)%N);
    //SimpleBigint Res(simpleMontgomeryReduction(D,N));
    return Res;
}*/

