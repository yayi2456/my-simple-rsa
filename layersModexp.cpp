#include"layersModexp.h"
//#define _LAYERS_MODEXP_DEBUG
void LayersModexp::huafenKrsnum_min(){
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"begin:huafenKrsnum"<<" thraeds="<<threads<<",layers="<<layer;
    cout<<endl;
    #endif // _LAYERS_MODEXP_DEBUG
    krsnum.clear();
    krsvalue.clear();
    SimpleBigint t(threads,0);//
    SimpleBigint tmp=exp;
    for(unsigned i=1;i<layer;++i){
        SimpleBigint tmps=tmp/t;
        SimpleBigint shengyu=tmp-tmps*t;
        krsnum.push_back(shengyu);
        krsvalue.push_back(SimpleBigint(0,0));
        tmp=tmps;
        //防止threads^layer过大，导致最终的商为0
        //因此在最终为0之前就组织继续划分，并更新layer值
        if(SimpleBigint(0,0)==tmp){
            #ifdef _LAYERS_MODEXP_DEBUG
            cout<<"INFO: layer changed: from "<<layer<<"to "<<i<<endl;
            cout<<"the krsnum is:";
            for(auto it=krsnum.begin();it!=krsnum.end();++it){
                cout<<(*it)<<" , ";
            }
            cout<<endl;
            #endif // _LAYERS_MODEXP_DEBUG
            layer=i;
            break;
            // cerr<<"thread^layer is too large, decrease layer or thread!"<<endl;
            // cerr<<"exp="<<exp<<",threads="<<threads<<",layers="<<layer<<endl;
            //return;
        }

        #ifdef _LAYERS_MODEXP_DEBUG
        cout<<"the tmp is:"<<tmp<<",shengyu is"<<shengyu;
        cout<<endl;
        #endif // _LAYERS_MODEXP_DEBUG
    }
    //得到最后的商（如果没有更新layer，就意味着商不是0，需要存储下来
    if(!(SimpleBigint(0,0)==tmp)){
        // cerr<<"thread^layer is too large, decrease layer or thread!"<<endl;
        // cerr<<"exp="<<exp<<",threads="<<threads<<",layers="<<layer<<endl;
        // return;
        krsnum.push_back(tmp);
        krsvalue.push_back(SimpleBigint(0,0));
        #ifdef _LAYERS_MODEXP_DEBUG
        cout<<"the krsnum is:";
        for(auto it=krsnum.begin();it!=krsnum.end();++it){
            cout<<(*it)<<" ";
        }
        cout<<endl;
        #endif // _LAYERS_MODEXP_DEBUG
    }

}
void LayersModexp::huafenKrsnum_max(unsigned mul){
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"begin:max-huafenKrsnum"<<" thraeds="<<SimpleBigint(1,mul)<<",layers="<<exp.numbersLength();
    cout<<endl;
    #endif // _LAYERS_MODEXP_DEBUG
    krsnum.clear();
    krsvalue.clear();
    SimpleBigint t(1,mul);//
    SimpleBigint tmp=exp;
    /////
    threads=1;
    layer=exp.numbersLength();
    /////
    for(unsigned i=1;i<layer;++i){
        SimpleBigint tmps=tmp.getlast(tmp.numbersLength()-mul);
        #ifdef _LAYERS_MODEXP_DEBUG
        cout<<"divide-max:"<<tmp<<","<<t<<endl;
        #endif
        SimpleBigint shengyu=tmp.getfirst(mul);
        #ifdef _LAYERS_MODEXP_DEBUG
        cout<<"mod-max:"<<shengyu<<endl;
        #endif
        krsnum.push_back(shengyu);
        krsvalue.push_back(SimpleBigint(0,0));
        tmp=tmps;
        //防止threads^layer过大，导致最终的商为0
        //因此在最终为0之前就组织继续划分，并更新layer值
        if(SimpleBigint(0,0)==tmp){
           #ifdef _LAYERS_MODEXP_DEBUG
            cout<<"INFO: layer changed: from "<<layer<<"to "<<i<<endl;
            cout<<"the krsnum is:";
            for(auto it=krsnum.begin();it!=krsnum.end();++it){
                cout<<(*it)<<" , ";
            }
            cout<<endl;
           #endif // _LAYERS_MODEXP_DEBUG

            layer=i;
            break;
            // cerr<<"thread^layer is too large, decrease layer or thread!"<<endl;
            // cerr<<"exp="<<exp<<",threads="<<threads<<",layers="<<layer<<endl;
            //return;
        }
        #ifdef _LAYERS_MODEXP_DEBUG
        cout<<"the tmp is:"<<tmp<<",shengyu is"<<shengyu;
        cout<<endl;
        #endif // _LAYERS_MODEXP_DEBUG
    }
    //得到最后的商（如果没有更新layer，就意味着商不是0，需要存储下来
    if(!(SimpleBigint(0,0)==tmp)){
        // cerr<<"thread^layer is too large, decrease layer or thread!"<<endl;
        // cerr<<"exp="<<exp<<",threads="<<threads<<",layers="<<layer<<endl;
        // return;
        krsnum.push_back(tmp);
        krsvalue.push_back(SimpleBigint(0,0));
        #ifdef _LAYERS_MODEXP_DEBUG
        cout<<"the krsnum is:";
        for(auto it=krsnum.begin();it!=krsnum.end();++it){
            cout<<(*it)<<" , ";
        }
        cout<<endl;
        #endif // _LAYERS_MODEXP_DEBUG
    }

}

void LayersModexp::layersmodexp_min(){
    //assign在外面做
    if(krsnum.size()!=layer){
        cerr<<"wrong in assignment, try again."<<endl;
        cmexp=SimpleBigint(0,0);
        return;
    }
    //找最大值
    SimpleBigint maxi(0,0);
    for(unsigned i=0;i<layer;++i){
        if(krsnum[i]>maxi)maxi=krsnum[i];
    }
/*    for(auto it=krsnum.begin();it!=krsnum.end();++it){
            if(maxi==*(it)){
                krsnum.erase(it);
                break;
            }
    }*/
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"the cmr="<<cmr<<endl;
    cout<<"the cm="<<cm<<endl;
    #endif
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"the maxi is"<<maxi<<endl;
    SimpleBigint sbp(0,0);
    #endif
    SimpleBigint resr=cmr;
    unsigned j=0;
    //将那些不需要模乘的提出来
    for(auto it=krsnum.begin();it!=krsnum.end();++j,++it){
        if(SimpleBigint(1,0)==*(it)){
            krsvalue[j]=resr;
            //krsnum.erase(it);
            #ifdef _LAYERS_MODEXP_DEBUG
            sbp+=SimpleBigint(1,0);cout<<"add1-";
            #endif
            //break;
        }
    }
    //进行计算：只计算最大指数需要的，其他的可以在过程中进行填充
    for(SimpleBigint i(2,0);maxi>=i;i+=SimpleBigint(1,0)){
        resr=mym.myMontgomeryMul(resr,cmr);
        #ifdef _LAYERS_MODEXP_DEBUG
        //cout<<"in exptmp="<<i<<", the resr is:"<<resr<<endl;
        #endif
        j=0;
        //寻找是不是有可以填充的
        for(auto it=krsnum.begin();it!=krsnum.end();++j,++it){
            if(i==*(it)){
                krsvalue[j]=resr;
                //krsnum.erase(it);
                #ifdef _LAYERS_MODEXP_DEBUG
                sbp+=SimpleBigint(1,0);cout<<"add1-";
                #endif
            }
        }
    }
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"all the exp="<<sbp<<endl;
    #endif
    //krsvalue[layer-1]=resr;
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"the krsnum is:";
    for(auto it=krsnum.begin();it!=krsnum.end();++it){
        cout<<(*it)<<" , ";
    }
    cout<<"the krsvalue is:";
    for(auto it=krsvalue.begin();it!=krsvalue.end();++it){
        cout<<(*it)<<" , ";
    }
    cout<<endl;
    #endif
    //现在开始最后的合并：
    SimpleBigint tmp(krsvalue[layer-1]);
    for(unsigned i=1;i<layer;++i){
        for(unsigned j=1;j<threads;++j){
            #ifdef _LAYERS_MODEXP_DEBUG
            //cout<<"the before-result of i="<<i<<",j="<<j<<",is:"<<tmp<<endl;
            #endif
            tmp=mym.myMontgomeryMul(tmp,krsvalue[layer-1]);
            #ifdef _LAYERS_MODEXP_DEBUG
            //cout<<"the hebing-result of i="<<i<<",j="<<j<<",is:"<<tmp<<endl;
            #endif
        }
        if(!(krsvalue[layer-1-i]==SimpleBigint(0,0)))
            tmp=mym.myMontgomeryMul(tmp,krsvalue[layer-1-i]);
        krsvalue[layer-1]=tmp;//更新
    }
    cmexp=mym.myMontgomeryMul(tmp,cmexp);
    noreduce=cmexp%mod;
    SimpleBigint sp(1,0);
    SimpleBigint otherres;
    otherres=mym.myMontgomeryMul(sp,cmexp);
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"the unreducted-result is:"<<cmexp<<endl;
    cout<<"other is:"<<otherres<<endl;
    #endif
    cmexp=mym.myMontgomeryReduction(cmexp);
    if(!(otherres==cmexp)){
        cerr<<"\n\n\n"<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>run quick! something error here! QAQ !!!\n\n"<<endl;
    }
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"the result is:"<<cmexp<<endl;
    #endif
}


void LayersModexp::layersmodexp_max(unsigned mul){
    //assign在外面做
    if(krsnum.size()!=layer){
        cerr<<"wrong in assignment, try again."<<endl;
        cmexp=SimpleBigint(0,0);
        return;
    }
    //找最大值
    // SimpleBigint maxi(0,0);
    // for(unsigned i=0;i<layer;++i){
    //     if(krsnum[i]>maxi)maxi=krsnum[i];
    // }
    // #ifdef _LAYERS_MODEXP_DEBUG
    // cout<<"the maxi is"<<maxi<<endl;
    //SimpleBigint sbp(0,0);
    // #endif
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"max-the cmr="<<cmr<<endl;
    cout<<"max-the cm="<<cm<<endl;
    #endif
    SimpleBigint resr=cmr;
    unsigned j=0;
    //将那些不需要模乘的提出来
    for(auto it=krsnum.begin();it!=krsnum.end();++j,++it){
        if(SimpleBigint(1,0)==*(it)){
            krsvalue[j]=resr;
            //krsnum.erase(it);
            // #ifdef _LAYERS_MODEXP_DEBUG
            // sbp=sbp+(*it);cout<<"add1-";
            // #endif
            //break;
        }
    }
    //进行计算：逐个调用更小的
    for(unsigned i=0;i<layer;++i){
        if(!((krsnum[i]==SimpleBigint(1,0))||(krsnum[i]==SimpleBigint(0,0)))){
            #ifdef _LAYERS_MODEXP_DEBUG
            cout<<"begin calculate one by one: "<<"("<<i+1<<","<<layer<<")"<<"cm="<<cmr<<",exp="<<krsnum[i]<<",mod="<<mod<<endl;
            #endif
            LayersModexp lme(cm,krsnum[i],mod);
            lme.setThreadandLayer(32,6);
            lme.huafenKrsnum_min();
            lme.layersmodexp_min();
            krsvalue[i]=lme.cmexp;//lme.mym.myMontgomeryMul((lme.cmexp),lme.mym.;
        //     SimpleBigint xtmp=lme.cmexp*lme.mym.R%mod;
        //     if(!(xtmp==krsvalue[i])){
        // cerr<<"\n\n\n"<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>run quick! something error here! QAQ !!!\n\n"<<endl;
        //     }

        }
    }
    // for(SimpleBigint i(2,0);maxi>=i;i+=SimpleBigint(1,0)){

    //     resr=mym.myMontgomeryMul(resr,cmr);
    //     j=0;
    //     //寻找是不是有可以填充的
    //     for(auto it=krsnum.begin();it!=krsnum.end();++j,++it){
    //         if(i==*(it)){
    //             krsvalue[j]=resr;
    //             krsnum.erase(it);
    //             #ifdef _LAYERS_MODEXP_DEBUG
    //             sbp=sbp+(*it);cout<<"add1-";
    //             #endif
    //             break;
    //         }
    //     }
    // }
    // #ifdef _LAYERS_MODEXP_DEBUG
    // cout<<"all the exp="<<sbp<<endl;
    // #endif
    //krsvalue[layer-1]=resr;
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"============================================REDUCTION BEGIN IN MAX===================================="<<endl;
    cout<<"max-the krsnum is:";
    for(auto it=krsnum.begin();it!=krsnum.end();++it){
        cout<<(*it)<<" , ";
    }
    cout<<"max-the krsvalue is:";
    for(auto it=krsvalue.begin();it!=krsvalue.end();++it){
        cout<<(*it)<<" , ";
    }
    cout<<endl;
    #endif
    //现在开始最后的合并：
    SimpleBigint tmp(0,0);
    SimpleBigint t(1,mul);
    SimpleBigint sp(1,0);
    for(unsigned i=1;i<layer;++i){
        #ifdef _LAYERS_MODEXP_DEBUG
        cout<<"max-begin reduce: cm="<<krsvalue[layer-1]<<",exp="<<t<<",mod="<<mod<<"corresponding krsnum="<<krsnum[layer-1-i]<<"krsvalue-mul"<<krsvalue[layer-1-i]<<endl;
        #endif
        LayersModexp lme(krsvalue[layer-1],t,mod);
        lme.setThreadandLayer(16,8);
        lme.huafenKrsnum_min();
        lme.layersmodexp_min();
        if(!(krsnum[layer-1-i]==SimpleBigint(0,0))){
            tmp=lme.cmexp;//noreduce%mod;
            tmp=tmp*krsvalue[layer-1-i]%mod;
            krsvalue[layer-1]=tmp;//更新
        }else{
            tmp=lme.cmexp;
            krsvalue[layer-1]=tmp;
        }
        #ifdef _LAYERS_MODEXP_DEBUG
        cout<<"max-tmp update: "<<tmp<<endl;
        #endif
    }
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"the max-cmexp is:"<<cmexp<<endl;
    #endif
    tmp=tmp*mym.R%mod;
    cmexp=mym.myMontgomeryMul(tmp,cmexp);
    noreduce=cmexp;
    SimpleBigint otherres=mym.myMontgomeryMul(sp,cmexp);
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"the max-unreducted-result is:"<<cmexp<<endl;
    #endif
    cmexp=mym.myMontgomeryReduction(cmexp);
    if(!(otherres==cmexp)){
        cerr<<"\n\n\n"<<"FINAL>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>run quick! something error here! QAQ !!!\n\n"<<endl;
    }
    #ifdef _LAYERS_MODEXP_DEBUG
    cout<<"the max-result is:"<<cmexp<<endl;
    #endif
}
