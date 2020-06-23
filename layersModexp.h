#ifndef _LAYERS_MOD_EXP_
#define _LAYERS_MOD_EXP_
#include"simpleBigint.h"
#include"simpleMontgomery.h"
#include<stdlib.h>
#include<algorithm>
#include<vector>

//LayersModexp �࣬
//��ɷֲ��ģ�ݼ���
class LayersModexp{
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
    LayersModexp(SimpleBigint c,SimpleBigint e,SimpleBigint m){
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
    LayersModexp()=default;
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
#endif // _LAYERS_MOD_EXP_

