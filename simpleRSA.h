#ifndef _SIMPLE_RSA
#define _SIMPLE_RSA
#include"simpleModexp.h"
class SimpleRSA{
public:
    SimpleBigint p;
    SimpleBigint q;
    SimpleBigint d;
    SimpleBigint e;
    SimpleBigint n;
    //SimpleBigint phin;
    //bool checkEqual(SimpleBigint m);
    bool generateKey(unsigned length=2048);
    SimpleBigint encryptme(SimpleBigint m);
    SimpleBigint simdencryptme(SimpleBigint m);
    //SimpleBigint decryptme(SimpleBigint c);
};
#endif // _SIMPLE_RSA_
