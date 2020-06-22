#ifndef _SIMPLE_MONTGOMERY_
#define _SIMPLE_MONTGOMERY_

#include"simpleBigint.h"
#include<math.h>
#include <pmmintrin.h>
#include<cstring>
#include<windows.h>
//R=(2^32)^M，M=B.size()-1，（如果要分解B的话，我们就分解B）
//其中
class MyMontgomery{
public:
    unsigned r_exp=32;
    SimpleBigint A;//大数a
    SimpleBigint B;//大数b
    SimpleBigint N;//大数n：模数
    SimpleBigint R;//大数r：进制。
    SimpleBigint R2modN;
    SimpleBigint N_1;
    MyMontgomery(SimpleBigint a,SimpleBigint b,SimpleBigint n):
        A(a),B(b),N(n),R(1,b.numbers.size()-1)//R=2^32，这由大数表示法确定，容易计算
        {R2modN=calR2modN();N_1=calN_1();}
    MyMontgomery()=default;
    void initMontgomery(SimpleBigint a,SimpleBigint b,SimpleBigint n){
        A=a;
        B=b;
        N=n;
        R.getfromuint32(1,b.numbers.size()-1);
        R2modN=calR2modN();
        N_1=calN_1();
    }
    //唯一需要除法取模计算的函数，预先计算
    //计算R^2 mod N
    SimpleBigint calR2modN();
    //计算N^{-1}，使得N*N^{-1}= -1 mod N，使用亨泽尔引理计算
    SimpleBigint calN_1();
    //计算AR或BR，根据参数选择，如果isA为真，计算AR，否则计算BR
    SimpleBigint myMontgomeryTo(bool isA=true);
    //乘法计算过程
    //return AR*BR*R^{-1} mod N
    SimpleBigint myMontgomeryMul( SimpleBigint&AR, SimpleBigint&BR);
    //reduction
    //return S=ResR*R^{-1} mod N
    SimpleBigint myMontgomeryReduction(SimpleBigint ResR);
    //SIMD程序
    SimpleBigint simdMontgomeryMul(SimpleBigint&AR, SimpleBigint&BR);

};
#define TSIZE 32

extern vector<double> timeintervels;
//extern SimpleBigint R2modN;
//n=2^32
//a=x0

//计算x_0^-1，使得x*x^{-1}=1 mod R，使用扩展欧几里得计算
    //其中x_0是32bits的二进制数
uint32_t extendEuclideanx0_1_32(uint32_t x0);
//修改版函数
SimpleBigint simdMulBig_32(SimpleBigint X,uint32_t y0);
                   //uint32_t*dest,uint32_t*bignum,uint32_t scalar,uint32_t length=TSIZE);//dest=dest+bignum*scalar
//SimpleBigint AddBig_Big(SimpleBigint X,SimpleBigint Y);

//SimpleBigint simpleMontgomeryMul(const SimpleBigint&AR,const SimpleBigint&BR,const SimpleBigint& N,unsigned length=TSIZE);
//input:AR,Montgomery form A;BR,Montgomery form of B; N, modulo
//return AR*BR*R^{-1} mod N
//SimpleBigint simplemmulmod(const SimpleBigint&A,const SimpleBigint&B,const SimpleBigint&N,unsigned length=TSIZE);

//SimpleBigint simd_mulmod(const SimpleBigint&A,const SimpleBigint&B,const SimpleBigint&N,unsigned length=TSIZE);

#endif // _SIMPLE_MONTGOMERY_
