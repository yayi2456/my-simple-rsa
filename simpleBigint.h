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
    //只提供简单的取模运算
    SimpleBigint operator%(const SimpleBigint&bigint2)const;
    SimpleBigint&operator%=(const SimpleBigint&bigint2);
    //为R^2mod N准备的取模运算,使用N调用
    //SimpleBigint R2modN();
    //friend first1inR2(const SimpleBigint&R2);
    friend istream&operator>>(istream&, SimpleBigint&);//以十六进制输出
    friend ostream&operator<<(ostream&,const SimpleBigint&);//十六进制输入
    int numbersLength()const;
    uint32_t getbyIndex(unsigned index)const;
    SimpleBigint shiftDivide32();//调用对象发生变化
    SimpleBigint getfirst(int nums);//mod，实际上取得是一个大数的低位
    SimpleBigint getlast(int nums);//divide，实际上取得是一个大数的高位，一共取nums>numbersLength?numbersLength:nums
    SimpleBigint moveby2_divide(int num);//以2进制为单位向右移动
    SimpleBigint moveby2_mul();
    void trimnumber();
};
#endif
