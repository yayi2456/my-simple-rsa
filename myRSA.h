#ifndef _MY_RSA_
#define _MY_RSA_
#include"simpleModexp.h"
#include"layersModexp.h"
#include"quickModexp.h"
//MyRSA 类， 
//构建一个有基本参数的加密器，
//用来对消息m加密解密
class MyRSA{
    SimpleBigint p;
    SimpleBigint q;
    SimpleBigint d;
public:
    SimpleBigint e;
    SimpleBigint n;
    //SimpleBigint phin;
    //bool checkEqual(SimpleBigint m);
    //密钥生成函数，其实只是一个赋值
    bool generateKey(unsigned length=2048);
    //加密函数
    SimpleBigint encryptme(SimpleBigint m);
    //解密函数
    SimpleBigint decryptme(SimpleBigint c);
    //分层的加密和解密函数
    SimpleBigint encryptme_layers(SimpleBigint m,unsigned threads=4,unsigned layers=1);
    SimpleBigint decryptme_layers(SimpleBigint c,unsigned threads=4,unsigned layers=1);
    //快速模幂加密解密
    SimpleBigint encryptme_quick(SimpleBigint m);
    SimpleBigint decryptme_quick(SimpleBigint c);
};
#endif // _MY_RSA_
