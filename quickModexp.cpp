#include"quickModexp.h"
//#define _QUICK_MODEXP_DEBUG_
//#define _QUICK_TWOTASK_DEBUG_
void QuickModExp::quickmodexp(){
    //为了效率，对大数分节处理
    #ifndef _TWO_TASKS_
    SimpleBigint tmpr=cmr;
    SimpleBigint resr=mym.R%mym.N;
    for(int i=0;i<exp.numbersLength();++i){
        for(uint32_t j=exp.numbers[i],jnum=0;jnum<32;j/=2,++jnum){
            if(j%2!=0){
                resr=mym.simdMontgomeryMul_avx256(resr,tmpr);//_avx256
                //myMontgomeryMul(resr,tmpr);
            }
            tmpr=mym.simdMontgomeryMul_avx256(tmpr,tmpr);//simdMontgomeryMul_avx256
        }
    }
    #else
    #ifdef _QUICK_MODEXP_DEBUG_
    cout<<"two-tasks"<<endl;
    #endif // _QUICK_MODEXP_DEBUG_
    SimpleBigint tmpr=cmr;
    SimpleBigint resr=mym.R%mym.N;
    int allones=0,allbits=0;
    bool isone[allbits+1]={0};
    bool isok[allbits+1]={0};
    allbits=exp.numbersLength()*32;
    SimpleBigint precomputed2i[allbits+1];
    #ifdef _QUICK_MODEXP_DEBUG_
    cout<<"begin"<<endl;
    #endif // _QUICK_MODEXP_DEBUG_
    #pragma omp parallel num_threads(2)
    {
    for(int i=0;i<exp.numbersLength();++i){
        for(uint32_t j=exp.numbers[i],jnum=0;jnum<32;j/=2,++jnum){
            SimpleBigint thistmpr=tmpr;
            if(j%2==0){
                #pragma omp for
                for(int num=0;num<2;++num){
                    if(num==0);
                    else{
                        tmpr=mym.myMontgomeryMul(tmpr,tmpr);//simdMontgomeryMul_avx256
                    }
                    #ifdef _QUICK_MODEXP_DEBUG_
                    cout<<"at the "<<omp_get_thread_num()<<",i="<<i*32+jnum<<",tmpr="<<tmpr<<endl;
                    #endif // _QUICK_MODEXP_DEBUG_
                }
            #ifdef _QUICK_MODEXP_DEBUG_
            cout<<"j%2==0 end"<<endl;
            #endif // _QUICK_MODEXP_DEBUG_
            }
            if(j%2!=0){
                #pragma omp for
                for(int nump=0;nump<2;++nump){
                    if(nump==0){
                        resr=mym.myMontgomeryMul(resr,thistmpr);
                    }else{
                        tmpr=mym.myMontgomeryMul(tmpr,tmpr);//simdMontgomeryMul_avx256
                    }
                }
                #ifdef _QUICK_MODEXP_DEBUG_
                cout<<"j%2!=0 end"<<endl;
                #endif // _QUICK_MODEXP_DEBUG_
            }
        }
    }
    }

    /*#pragma omp parallel for
    for(int my_id=0;my_id<2;++my_id){
        if(my_id==0){
            precomputed2i[0]=tmpr;
            //计算2^i
            int place=0;
            for(int i=0;i<exp.numbersLength();++i){
                for(uint32_t j=exp.numbers[i],jnum=0;jnum<32;j/=2,++jnum){
                    if(j%2!=0){
                        #pragma omp flush(precomputed2i)
                    }
                    ++place;
                    tmpr=mym.simdMontgomeryMul_avx256(tmpr,tmpr);//simdMontgomeryMul_avx256
                    precomputed2i[place]=tmpr;
                    //[place])
                    //[place])
                }
            }
        }else{
            int place=0;
            for(int i=0;i<exp.numbersLength();++i){
                for(uint32_t j=exp.numbers[i],jnum=0;jnum<32;j/=2,++jnum){
                    if(j%2!=0){
                        while(precomputed2i[place].numbersLength()==0){
                            //cout<<"waiting for isok at place="<<place<<endl;
                            //#pragma omp flush(precomputed2i)
                            Sleep(0);
                        }
                        resr=mym.simdMontgomeryMul_avx256(resr,precomputed2i[place]);
                        #ifdef _QUICK_TWOTASK_DEBUG_
                        cout<<"mul done,place="<<place<<endl;
                        #endif
                    }
                    ++place;
                }
            }
        }
    }*/
    #endif
    #ifdef _QUICK_MODEXP_DEBUG_
    cout<<"modexpR-1 done with tmpr="<<tmpr<<endl;
    #endif
    #ifdef _QUICK_TWOTASK_DEBUG_
    cout<<"redution begin"<<endl;
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
