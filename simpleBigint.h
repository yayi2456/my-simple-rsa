#ifndef _SIMPLE_BIGINT_
#define _SIMPLE_BIGINT_
#include<vector>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<iomanip>
#include<math.h>
#include <pmmintrin.h>
#include"switches.h"
#include<omp.h>
//#define _DEBUG

using namespace std;

//#define UINT32_MAX 4294967295 //FFFFFFFF

class SimpleBigint{
    public:
    vector<uint32_t>numbers;

    SimpleBigint()=default;
    SimpleBigint(string num);
    SimpleBigint(const SimpleBigint&bigint);
    SimpleBigint(const uint64_t num,unsigned mulsize);
    SimpleBigint(uint32_t*num,unsigned from,unsigned to):numbers(num+from,num+to){}
    SimpleBigint(void*);
    void getfromString(string num);
    void getfromuint32(const uint32_t num,unsigned mulsize);
    SimpleBigint& operator=(const SimpleBigint& bigint);
    SimpleBigint operator+(const SimpleBigint& bigint2)const;
    SimpleBigint& operator+=(const SimpleBigint& bigint2);
    SimpleBigint operator*(const SimpleBigint&bigint2)const;
    SimpleBigint operator*(uint32_t)const;
    SimpleBigint&operator*=(const SimpleBigint&bigint2);
    SimpleBigint operator-(const SimpleBigint& bigint2)const;
    SimpleBigint& operator-=(const SimpleBigint& bigint2);
    bool operator>(const SimpleBigint&bigint2)const;
    bool operator>=(const SimpleBigint&bigint2)const;
    bool operator<(const SimpleBigint&bigint2)const;
    bool operator==(const SimpleBigint&bigint2)const;
    SimpleBigint operator/(const SimpleBigint&bigint2)const;
    SimpleBigint&operator/=(const SimpleBigint&bigint2);
    //ֻ�ṩ�򵥵�ȡģ����
    SimpleBigint operator%(const SimpleBigint&bigint2)const;
    SimpleBigint&operator%=(const SimpleBigint&bigint2);
    //ΪR^2mod N׼����ȡģ����,ʹ��N����
    //SimpleBigint R2modN();
    //friend first1inR2(const SimpleBigint&R2);
    friend istream&operator>>(istream&, SimpleBigint&);//��ʮ���������
    friend ostream&operator<<(ostream&,const SimpleBigint&);//ʮ����������
    int numbersLength()const;
    uint32_t getbyIndex(unsigned index)const;
    SimpleBigint shiftDivide32();//���ö������仯
    SimpleBigint getfirst(int nums);//mod��ʵ����ȡ����һ�������ĵ�λ
    SimpleBigint getlast(int nums);//divide��ʵ����ȡ����һ�������ĸ�λ��һ��ȡnums>numbersLength?numbersLength:nums
    SimpleBigint moveby2_divide(int num);//��2����Ϊ��λ�����ƶ�
    SimpleBigint moveby2_mul();
    void trimnumber();
};
#endif
