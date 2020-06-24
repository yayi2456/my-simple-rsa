#ifndef _CRT_RSA
#define _CRT_RSA
#define _OPEN_OPENMP_
#include"layersModexp.h"
#include"quickModexp.h"
#ifdef _OPEN_OPENMP_
#include<omp.h>
#endif
class CrtRSA{
    SimpleBigint p;
    SimpleBigint q;
    SimpleBigint d;
    SimpleBigint dp;
    SimpleBigint dq;
    SimpleBigint q_1;//q^{-1} mod p
public:
    SimpleBigint e;
    SimpleBigint n;
    //SimpleBigint phin;
    //bool checkEqual(SimpleBigint m);
    bool generateKey(unsigned length=2048);
    //分层的加密和解密函数
    SimpleBigint encryptme_layers(SimpleBigint m,unsigned threads=4,unsigned layers=1);
    SimpleBigint decryptme_layers(SimpleBigint c,unsigned threads=4,unsigned layers=1);
    //快速模幂加密解密
    SimpleBigint encryptme_quick(SimpleBigint m);
    SimpleBigint decryptme_quick(SimpleBigint m);
};
#endif // _CRT_RSA
