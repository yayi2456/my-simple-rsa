#ifndef _SIMPLE_MONTGOMERY_
#define _SIMPLE_MONTGOMERY_

#include"simpleBigint.h"
#include<math.h>
#include <pmmintrin.h>
#include<cstring>
#include<windows.h>
//R=(2^32)^M��M=B.size()-1�������Ҫ�ֽ�B�Ļ������Ǿͷֽ�B��
//����
class MyMontgomery{
public:
    unsigned r_exp=32;
    SimpleBigint A;//����a
    SimpleBigint B;//����b
    SimpleBigint N;//����n��ģ��
    SimpleBigint R;//����r�����ơ�
    SimpleBigint R2modN;
    SimpleBigint N_1;
    MyMontgomery(SimpleBigint a,SimpleBigint b,SimpleBigint n):
        A(a),B(b),N(n),R(1,b.numbers.size()-1)//R=2^32�����ɴ�����ʾ��ȷ�������׼���
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
    //Ψһ��Ҫ����ȡģ����ĺ�����Ԥ�ȼ���
    //����R^2 mod N
    SimpleBigint calR2modN();
    //����N^{-1}��ʹ��N*N^{-1}= -1 mod N��ʹ�ú�����������
    SimpleBigint calN_1();
    //����AR��BR�����ݲ���ѡ�����isAΪ�棬����AR���������BR
    SimpleBigint myMontgomeryTo(bool isA=true);
    //�˷��������
    //return AR*BR*R^{-1} mod N
    SimpleBigint myMontgomeryMul( SimpleBigint&AR, SimpleBigint&BR);
    //reduction
    //return S=ResR*R^{-1} mod N
    SimpleBigint myMontgomeryReduction(SimpleBigint ResR);
    //SIMD����
    SimpleBigint simdMontgomeryMul(SimpleBigint&AR, SimpleBigint&BR);

};
#define TSIZE 32

extern vector<double> timeintervels;
//extern SimpleBigint R2modN;
//n=2^32
//a=x0

//����x_0^-1��ʹ��x*x^{-1}=1 mod R��ʹ����չŷ����ü���
    //����x_0��32bits�Ķ�������
uint32_t extendEuclideanx0_1_32(uint32_t x0);
//�޸İ溯��
SimpleBigint simdMulBig_32(SimpleBigint X,uint32_t y0);
                   //uint32_t*dest,uint32_t*bignum,uint32_t scalar,uint32_t length=TSIZE);//dest=dest+bignum*scalar
//SimpleBigint AddBig_Big(SimpleBigint X,SimpleBigint Y);

//SimpleBigint simpleMontgomeryMul(const SimpleBigint&AR,const SimpleBigint&BR,const SimpleBigint& N,unsigned length=TSIZE);
//input:AR,Montgomery form A;BR,Montgomery form of B; N, modulo
//return AR*BR*R^{-1} mod N
//SimpleBigint simplemmulmod(const SimpleBigint&A,const SimpleBigint&B,const SimpleBigint&N,unsigned length=TSIZE);

//SimpleBigint simd_mulmod(const SimpleBigint&A,const SimpleBigint&B,const SimpleBigint&N,unsigned length=TSIZE);

#endif // _SIMPLE_MONTGOMERY_
