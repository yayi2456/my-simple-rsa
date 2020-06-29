#include"quickModexp.h"

void QuickModExp::quickmodexp(){
    //为了效率，对大数分节处理
    #ifndef _TWO_TASKS_
    #ifdef _TEST_TIME_QUCK_MODEXP
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    SimpleBigint tmpr=cmr;
    SimpleBigint resr=mym.myMontgomeryReduction(mym.R2modN);//mym.R%mym.N;
    #ifdef _TEST_TIME_QUCK_MODEXP
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000.0/freq;
    cout<<"in quickModexp-r%n, time used="<<intervel<<" ms"<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    long long head2,tail2,headres,tailres;
    double sum2=0,sumres=0;
    #endif
    for(int i=0;i<exp.numbersLength();++i){
        for(uint32_t j=exp.numbers[i],jnum=0;jnum<32;j/=2,++jnum){
            if(j%2!=0){
                #ifdef _TEST_TIME_QUCK_MODEXP
                QueryPerformanceCounter((LARGE_INTEGER*)&headres);
                #endif
                #ifndef _AVX2_ENABLED_
                resr=mym.myMontgomeryMul(resr,tmpr);//simdMontgomeryMul_avx256
                #else
                resr=mym.simdMontgomeryMul_avx256(resr,tmpr);
                #endif
                #ifdef _TEST_TIME_QUCK_MODEXP
                QueryPerformanceCounter((LARGE_INTEGER*)&tailres);
                sumres+=((tailres-headres)*1000.0/freq);
                #endif
                //myMontgomeryMul(resr,tmpr);
            }
            #ifdef _TEST_TIME_QUCK_MODEXP
            QueryPerformanceCounter((LARGE_INTEGER*)&head2);
            #endif
            #ifndef _AVX2_ENABLED_
            tmpr=mym.myMontgomeryMul(tmpr,tmpr);//simdMontgomeryMul_avx256
            #else
            tmpr=mym.simdMontgomeryMul_avx256(tmpr,tmpr);//simdMontgomeryMul_avx256
            #endif
            #ifdef _TEST_TIME_QUCK_MODEXP
            QueryPerformanceCounter((LARGE_INTEGER*)&tail2);
            sum2+=((tail2-head2)*1000.0/freq);
            #endif
        }
    }
    #ifdef _TEST_TIME_QUCK_MODEXP
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1000.0/freq;
    cout<<"in quickModexp-core, time used="<<intervel<<" ms"<<endl;
    cout<<"in quickModexp-2^i, time used="<<sum2<<" ms"<<endl;
    cout<<"in quickModexp-res*m, time used="<<sumres<<" ms"<<endl;
    cout<<"in quickModexp-one-m, time used="<<(sum2/(32*exp.numbersLength()))<<" ms"<<endl;
    #endif
    #else
    #ifdef _TEST_TIME_QUCK_MODEXP
    long long head,freq,tail;
    QueryPerformanceFrequency ( (LARGE_INTEGER*)& freq) ;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    SimpleBigint tmpr=cmr;
    SimpleBigint resr=mym.myMontgomeryReduction(mym.R2modN);//mym.R%mym.N;
    #ifdef _TEST_TIME_QUCK_MODEXP
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    double intervel=(tail-head)*1000.0/freq;
    cout<<"in quickModexp-twotask-r%n, time used="<<intervel<<" ms"<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    int allones=0,allbits=0;
    bool isone[allbits+1]={0};
    bool isok[allbits+1]={0};
    allbits=exp.numbersLength()*32;
    SimpleBigint precomputed2i[allbits+1];
    /*被废弃的一版
    #pragma omp parallel num_threads(2)
    {
    for(int i=0;i<exp.numbersLength();++i){
        for(uint32_t j=exp.numbers[i],jnum=0;jnum<32;j/=2,++jnum){
            SimpleBigint thistmpr=tmpr;
                #pragma omp for
                for(int num=0;num<2;++num){
                    if(num==0){
                        if(j%2!=0){
                            resr=mym.myMontgomeryMul(resr,thistmpr);
                        }
                    }
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
        }
    }
*/
    long long headone,tailone;
    double sumone=0;
    #pragma omp parallel for
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
                    #ifdef _TEST_TIME_QUCK_MODEXP
                    QueryPerformanceCounter((LARGE_INTEGER*)&headone);
                    #endif
                    #ifndef _AVX2_ENABLED_
                    tmpr=mym.myMontgomeryMul(tmpr,tmpr);//simdMontgomeryMul_avx256
                    #else
                    tmpr=mym.simdMontgomeryMul_avx256(tmpr,tmpr);//simdMontgomeryMul_avx256
                    #endif
                    #ifdef _TEST_TIME_QUCK_MODEXP
                    QueryPerformanceCounter((LARGE_INTEGER*)&tailone);
                    sumone+=((tailone-headone)*1000.0/freq);
                    #endif
                    //tmpr=mym.simdMontgomeryMul_avx256(tmpr,tmpr);//simdMontgomeryMul_avx256
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
                        #ifndef _AVX2_ENABLED_
                        resr=mym.myMontgomeryMul(resr,precomputed2i[place]);//simdMontgomeryMul_avx256
                        #else
                        resr=mym.simdMontgomeryMul_avx256(resr,precomputed2i[place]);
                        #endif
                        //resr=mym.simdMontgomeryMul_avx256(resr,precomputed2i[place]);
                        #ifdef _QUICK_TWOTASK_DEBUG_
                        cout<<"mul done,place="<<place<<endl;
                        #endif
                    }
                    ++place;
                }
            }
        }
    }
    #ifdef _TEST_TIME_QUCK_MODEXP
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1000.0/freq;
    cout<<"in quickModexp-twotask, time used="<<intervel<<" ms"<<endl;
    cout<<"in quickModexp-one-m, time used="<<(sumone/(32*exp.numbersLength()))<<" ms"<<endl;
    #endif
    #endif
    #ifdef _QUICK_MODEXP_DEBUG_
    cout<<"modexpR-1 done with tmpr="<<tmpr<<endl;
    #endif
    #ifdef _QUICK_TWOTASK_DEBUG_
    cout<<"redution begin"<<endl;
    #endif
    //reduction
    //SimpleBigint vzero(1,0);
    //SimpleBigint resvice=mym.simdMontgomeryMul_avx256(vzero,resr);
    #ifdef _TEST_TIME_QUCK_MODEXP
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    #endif
    resr=mym.myMontgomeryReduction(resr);

   /* if(!(resr==resvice)){
        cerr<<"error occurs!"<<endl;
        cerr<<"resvice="<<resvice<<endl;
    }*/
    #ifdef _TEST_TIME_QUCK_MODEXP
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    intervel=(tail-head)*1000.0/freq;
    cout<<"in quickModexp-reduce, time used="<<intervel<<" ms"<<endl;
    #endif
    cmexp=resr;
}
