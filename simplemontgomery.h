#ifndef _SIMPLE_MONTGOMERY_
#define _SIMPLE_MONTGOMERY_

#include"simpleBigint.h"
#include<math.h>
#include <immintrin.h>
#include<cstring>
#include<windows.h>
#include"bigintAdd.h"
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
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        long long head,freq,tail,headb,tailb;
        QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&headb);
        #endif
        A=a;
        B=b;
        N=n;
        R.getfromuint32(1,b.numbers.size()-1);
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        R2modN=calR2modN();
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        double intervel=(tail-head)*1000.0/freq;
        cout<<"in mym-calr2modn, time used="<<intervel<<" ms"<<endl;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        #endif
        N_1=calN_1();
        #ifdef _TEST_TIME_SIMPLE_MONTGOMERY_
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        intervel=(tail-head)*1000.0/freq;
        cout<<"in mym-caln1, time used="<<intervel<<" ms"<<endl;
        QueryPerformanceCounter((LARGE_INTEGER*)&tailb);
        intervel=(tailb-headb)*1000.0/freq;
        cout<<"in mym-mym-all, time used="<<intervel<<" ms"<<endl;
        #endif
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
    SimpleBigint simdMontgomeryMul_avx256(SimpleBigint&AR, SimpleBigint&BR);

};
//extern SimpleBigint R2modN;
//n=2^32
//a=x0

//����x_0^-1��ʹ��x*x^{-1}=1 mod R��ʹ����չŷ����ü���
    //����x_0��32bits�Ķ�������
uint32_t extendEuclideanx0_1_32(uint32_t x0);
//�޸İ溯��
SimpleBigint simdMulBig_32(SimpleBigint X,uint32_t y0);
#endif // _SIMPLE_MONTGOMERY_
