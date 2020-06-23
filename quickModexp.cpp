#include"quickModexp.h"
//#define _QUICK_MODEXP_DEBUG_

void QuickModExp::quickmodexp(){
    SimpleBigint tmpr=cmr;
    SimpleBigint resr=mym.R%mym.N;
    #ifdef _QUICK_MODEXP_DEBUG_
    cout<<"modexp with i=0x"<<1<<" in 0x"<<exp<<" with tmpr="<<tmpr<<endl;
    #endif
    //为了效率，对大数分节处理
    for(int i=0;i<exp.numbersLength();++i){
        for(uint32_t j=exp.numbers[i],jnum=0;jnum<32;j/=2,++jnum){
            if(j%2!=0){
                resr=mym.simdMontgomeryMul_avx256(resr,tmpr);//_avx256
                //myMontgomeryMul(resr,tmpr);
            }
            tmpr=mym.simdMontgomeryMul_avx256(tmpr,tmpr);//simdMontgomeryMul_avx256
        }
    }
    #ifdef _QUICK_MODEXP_DEBUG_
    cout<<"modexpR-1 done with tmpr="<<tmpr<<endl;
    #endif
    //reduction
    SimpleBigint vzero(1,0);
    SimpleBigint resvice=mym.simdMontgomeryMul_avx256(vzero,resr);
    resr=mym.myMontgomeryReduction(resr);

    if(!(resr==resvice)){
        cerr<<"error occurs!"<<endl;
        cerr<<"resvice="<<resvice<<endl;
    }
    #ifdef _QUICK_MODEXP_DEBUG_
    cout<<"reduction done with tmpr="<<tmpr<<endl;
    #endif
    cmexp=resr;
}
