#ifndef _SERVERAL_MSG_
#define _SEVERAL_MSG_
#include<pthread.h>
#include"simpleRSA.h"
class SeveralMsgRSAC{
public:
    vector<SimpleBigint>msgs;
    vector<SimpleBigint>ctxt;
    unsigned threads=4;//默认4个线程
    vector<unsigned>msgnums;
    SimpleRSA srsa;
    bool initSRSA(unsigned length);
    void initMsg(vector<SimpleBigint>&m);
    void encryptmsgs(unsigned from,unsigned to);//vector<SimpleBigint>::iterator from,vector<SimpleBigint>::iterator to);//包括from不包括to
    //vector<SimpleBigint> decryptmsgs(vector<SimpleBigint>::iterator from,vector<SimpleBigint>::iterator to);
    void simdencryptmsgs(unsigned from,unsigned to);
    void assigntasks();
};
extern SeveralMsgRSAC smr;
void*encryptmsgschild(void*);
void*simdencryptmsgschild(void*);
void threadencryptmsg(bool simd=false);
//void*decryptmsgschild(void*);
#endif // _SERVERAL_MSG_
