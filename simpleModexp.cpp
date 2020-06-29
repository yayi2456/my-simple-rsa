
#include"simpleModexp.h"

//#define _SIMPLE_MODEXP_DEBUG_

void SimpleModExp::modexp(){
    SimpleBigint tmpr=cmr;
    //SimpleBigint(MyMontgomery::*f)(SimpleBigint&,SimpleBigint&)=nullptr;
    //if(simd)f=&MyMontgomery::simdMontgomeryMul;
    //else f=&MyMontgomery::myMontgomeryMul;
    #ifdef _SIMPLE_MODEXP_DEBUG_
    cout<<"modexp with i=0x"<<1<<" in 0x"<<exp<<" with tmpr="<<tmpr<<endl;
    #endif
    for(SimpleBigint i(2,0);exp>=i;i+=SimpleBigint(1,0)){
        tmpr=mym.myMontgomeryMul(tmpr,cmr);//顺序不要更改
        #ifdef _SIMPLE_MODEXP_DEBUG_
        //cout<<"modexp with i=0x"<<i<<" in 0x"<<exp<<"mym:"<<mym.R<<","<<mym.N<<endl;
        cout<<"modexp with i=0x"<<i<<" in 0x"<<exp<<" with tmpr="<<tmpr<<",cmr="<<cmr<<endl;
        #endif
    }
    #ifdef _SIMPLE_MODEXP_DEBUG_
    cout<<"modexpR-1 done with tmpr="<<tmpr<<endl;
    #endif
    //reduction
    tmpr=mym.myMontgomeryReduction(tmpr);
    #ifdef _SIMPLE_MODEXP_DEBUG_
    cout<<"reduction done with tmpr="<<tmpr<<endl;
    #endif
    cmexp=tmpr;
}