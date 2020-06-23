#ifndef _THREAD_LAYERS_MOD_EXP_
#define _THREAD_LAYERS_MOD_EXP_
#include"simpleBigint.h"
#include"simpleMontgomery.h"
#include<stdlib.h>
#include<algorithm>
#include<vector>

//LayersModexp �࣬
//��ɷֲ��ģ�ݼ���
class ThreadLayersModexp{
public:
    SimpleBigint cm;//����
    SimpleBigint exp;//ָ��
    SimpleBigint mod;//ģ��
    SimpleBigint cmexp;//ģ��
    MyMontgomery mym;//�ɸ�����������
    SimpleBigint cmr;//cm���ɸ�������ʽ
    unsigned threads=4;//Ĭ��ÿ�λ���1/4
    unsigned layer=1;//���ִ���=layer-1����������֣�k=exp
    //krsnum ÿһ�λ�����ʣ���ǲ��ֵ�ֵ
    vector<SimpleBigint>krsnum;//����Ӧ����layer//˳����r_1,r_2...r_{layer-1},k
    //krsvalue ÿһ��������֮���ֵ
    vector<SimpleBigint>krsvalue;
    //���캯������ɹ�����initһ��
    SimpleBigint noreduce;//���ڽ����У���cmexpδ�������һ��reduce��ֵ����cmeexp*r mod n
    ThreadLayersModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c+m;
        while(cm.numbersLength()<=m.numbersLength()){
            cm=cm+m;
        }
        exp=e-SimpleBigint(1,0);
        mod=m;
        mym.initMontgomery(cm,cm,m);
        cmr=mym.myMontgomeryTo();
        cmexp=cmr;
        noreduce=cmr;
    }
    // }:
    //     cm(c+m),exp(e-SimpleBigint(1,0)),mod(m),mym(c+m,c+m,m)
    //     {cmr=mym.myMontgomeryTo();
    //     cmexp=cmr;}
    ThreadLayersModexp()=default;
    //huafenKrsnum �������в㼶���֣���¼ÿһ������ֵ�����һ����¼���յ�ָ����С
    void huafenKrsnum_min();//ָ����С��ʱ��ͨ����32bits����
    void huafenKrsnum_max(unsigned mul=0);//ָ���ϴ��ʱ��mul�����Ƶ�λ�������ʱ�����thread��������1��layers����exp.numberLength
    //initLayersModexp �������г�ʼ��
    void initLayersModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
        cm=c+m;
        while(cm.numbersLength()<=m.numbersLength()){
            cm=cm+m;
        }
        exp=e-SimpleBigint(1,0);
        mod=m;
        mym.initMontgomery(c+m,c+m,m);
        cmr=mym.myMontgomeryTo();
        cmexp=cmr;
        noreduce=cmr;
    }
    //ָ��������ÿ�㻮����������ͽ�thread����
    void setThreadandLayer(unsigned t,unsigned l){threads=t;layer=l;}
    //�����ϴμ���Ľ��
    void clearRes(){cmexp=cmr;}
    //����ģ��ֵcmexp
    void layersmodexp_min();//32bits���ڵļ���
    void layersmodexp_max(unsigned mul=0);//��Ӧ��huafen�����mul=0���򲻻�ʹ��layersModexp���м���
};
//ʹ��˳��
//1. ���캯����init
//2. setThreadandLayer
//3. huafenKrsnum
//4. layersmodexp
//5. ��cmexp���õ����

//MultiSimpleBignt ���ڶ��SimpleBigint���ݸ�child
class MultiSimpleBignt{
public:
    uint32_t numsofBigint=0;
    vector<SimpleBigint>simpleBigints;
    void*toVoidStar(){
        void*res;
        uint32_t totalsize=0;
        for(int i=0;i<simpleBigints.size();++i){
            totalsize+=simpleBigints[i].numbersLength();
        }
        res=malloc(sizeof(uint32_t)*(1+simpleBigints.size()+totalsize));
        memcpy(res,&numsofBigint,sizeof(uint32_t));
        uint32_t position=sizeof(uint32_t);
        for(int i=0;i<simpleBigints.size();++i){
            uint32_t simplebigintsize=simpleBigints[i].numbersLength();
            memcpy(res+position,&simplebigintsize,sizeof(uint32_t));
            position+=sizeof(uint32_t);
            memcpy(res+position,&(simpleBigints[i].numbers[0]),sizeof(uint32_t)*simplebigintsize);
            position+=(sizeof(uint32_t)*simplebigintsize);
        }
        return res;
    }
    MultiSimpleBignt()=default;
    MultiSimpleBignt(void*voidspace){
        simpleBigints.clear();
        memcpy(&numsofBigint,voidspace,sizeof(uint32_t));
        int position=sizeof(uint32_t);
        for(int i=0;i<numsofBigint;++i){
            SimpleBigint subSimpleBigint;
            subSimpleBigint=(SimpleBigint)(voidspace+position);
            //cout<<subSimpleBigint<<endl;
            position+=(sizeof(uint32_t)*(1+subSimpleBigint.numbersLength()));
            simpleBigints.push_back(subSimpleBigint);
        }
    }
    void AddOne(SimpleBigint simplebigint){
        simpleBigints.push_back(simplebigint);
        numsofBigint+=1;
    }
    friend ostream&operator<<(ostream&out,const MultiSimpleBignt&multibigint);
};


extern SimpleBigint cm1;
extern SimpleBigint mod1;
#endif // _THREAD_LAYERS_MOD_EXP_


