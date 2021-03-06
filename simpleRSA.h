#ifndef _SIMPLE_RSA
#define _SIMPLE_RSA
#include"simpleModexp.h"
#include"layersModexp.h"
#include"threadLayerModexp.h"
#include"quickModexp.h"
class SimpleRSA{
    SimpleBigint p;
    SimpleBigint q;
    SimpleBigint d;
public:
    SimpleBigint e;
    SimpleBigint n;
    //SimpleBigint phin;
    //bool checkEqual(SimpleBigint m);
    bool generateKey(unsigned length=2048);
    SimpleBigint encryptme(SimpleBigint m);
    SimpleBigint simdencryptme(SimpleBigint m);
    //分层的加密和解密函数
    SimpleBigint encryptme_layers(SimpleBigint m,unsigned threads=4,unsigned layers=1);
    SimpleBigint decryptme_layers(SimpleBigint c,unsigned threads=4,unsigned layers=1);
    //分层并行的加密和解密函数
    SimpleBigint encryptme_layers_multi(SimpleBigint m,unsigned threads=4,unsigned layers=1);
    SimpleBigint decryptme_layers_multi(SimpleBigint c,unsigned threads=4,unsigned layers=1);
    //快速模幂加密解密
    SimpleBigint encryptme_quick(SimpleBigint m);
    SimpleBigint decryptme_quick(SimpleBigint m);
};
#endif // _SIMPLE_RSA_
