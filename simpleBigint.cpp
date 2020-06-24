#include"simpleBigint.h"

SimpleBigint::SimpleBigint(string num){
    getfromString(num);
}

SimpleBigint::SimpleBigint(const SimpleBigint&bigint){
    numbers=bigint.numbers;
}
SimpleBigint::SimpleBigint(const uint64_t num,unsigned mulsize){


    uint32_t lowbits=num;
    uint32_t highbits=(num>>32);
    for(unsigned i=0;i<mulsize;++i){
        numbers.push_back(0);
    }
    numbers.push_back(lowbits);
    if(highbits)numbers.push_back(highbits);
}
SimpleBigint::SimpleBigint(void*num){
    uint32_t lengthme;
    memcpy(&lengthme,num,sizeof(uint32_t));
    //cout<<"SIMPLEBIGINT-length="<<lengthme<<endl;
    int position=sizeof(uint32_t);
    for(int i=0;i<(int)lengthme;++i){
        int tmp;
        memcpy(&tmp,num+position,sizeof(uint32_t));
        numbers.push_back(tmp);
        position+=sizeof(uint32_t);
        //cout<<"SIMPLEBIGINT-tmp"<<i<<"/"<<lengthme<<" ="<<tmp<<endl;
    }
}

void SimpleBigint::getfromString(string num){
    numbers.clear();
    int base=16;
    int elementsize=32,stepsize=elementsize/4;//8
    int numsize=num.length();
    //小端方式存储：numbers[0]是最低的8个字母
    int i=numsize-stepsize;
    for(;i>=0;i-=stepsize){
        uint32_t tmp=strtoul(num.substr(i,stepsize).c_str(),nullptr,base);
        numbers.push_back(tmp);
    }
    int j=numsize%stepsize;
    if(j==0)return;//处理结束
    else{
        uint32_t tmp=strtoul(num.substr(0,j).c_str(),nullptr,base);
        numbers.push_back(tmp);
    }

}
void SimpleBigint::getfromuint32(const uint32_t num,unsigned mulsize){
    numbers.clear();
    if(num==0){
        numbers.push_back(0);
        return;
    }
    for(unsigned i=0;i<mulsize;++i){
        numbers.push_back(0);
    }
    numbers.push_back(num);
}

SimpleBigint& SimpleBigint::operator=(const SimpleBigint& bigint){
    numbers=bigint.numbers;
    return *this;
}
SimpleBigint SimpleBigint::operator+(const SimpleBigint& bigint2)const{
    SimpleBigint ret(*this);
    ret+=bigint2;
    return ret;
}
SimpleBigint& SimpleBigint::operator+=(const SimpleBigint& bigint2){
    vector<uint32_t>::iterator it1=numbers.begin();
    vector<uint32_t>::const_iterator it2=bigint2.numbers.begin();
    uint64_t sum=0;
    uint32_t next=0;
    while(it1!=numbers.end()||it2!=bigint2.numbers.end()){
        if(it1!=numbers.end()){
            sum+=(uint64_t)(*it1);
        }
        else{
            numbers.push_back(0);
            it1=numbers.end()-1;
        }
        if(it2!=bigint2.numbers.end()){
            sum+=(uint64_t)((*it2));
            ++it2;
        }
        sum+=(uint64_t)(next);
        if(sum>>32){
            //yichu
            //cout<<"Sum="<<sum<<"sum>>32="<<(sum>>32)<<endl;
            next=(sum>>32);
        }
        else{
            next=0;
        }
        *it1=(uint32_t)sum;
        sum=0;
        ++it1;
        //cout<<sum<<endl;
    }
    if(next){
        numbers.push_back(1);
    }
    return *this;

}
SimpleBigint SimpleBigint::operator*(const SimpleBigint&bigint2)const{
    if(this->numbers.size()==1 && this->numbers[0]==0)return SimpleBigint(0,0);
    if(bigint2.numbers.size()==1 && bigint2.numbers[0]==0)return SimpleBigint(0,0);
    vector<uint32_t>::const_iterator it1;
    vector<uint32_t>::const_iterator it2;
    SimpleBigint ret;
    for(it1=numbers.begin();it1!=numbers.end();++it1){
        for(it2=bigint2.numbers.begin();it2!=bigint2.numbers.end();++it2){
            //需要向前移步多少个位置
            unsigned mulsize=((unsigned)(it1-numbers.begin())+(unsigned)(it2-bigint2.numbers.begin()));
            uint64_t tmp=(uint64_t)(*it1)*(*it2);
            SimpleBigint btmp(tmp,mulsize);
            ret+=btmp;
        }
    }
    ret.trimnumber();
    return ret;
}
SimpleBigint SimpleBigint::operator*(uint32_t x)const{
    if(this->numbers.size()==1 && this->numbers[0]==0)return SimpleBigint(0,0);
    if(x==0)return SimpleBigint(0,0);
    SimpleBigint ret;
    for(vector<uint32_t>::const_iterator it=numbers.begin();it!=numbers.end();++it){
        unsigned mulsize=(unsigned)(it-numbers.begin());
        uint64_t tmp=(uint64_t)(*it)*x;
        SimpleBigint btmp(tmp,mulsize);
        ret+=btmp;
    }
    ret.trimnumber();
    return ret;
}
SimpleBigint& SimpleBigint::operator*=(const SimpleBigint&bigint2){
    *this=(*this)*bigint2;
    return *this;
}
SimpleBigint SimpleBigint::operator-(const SimpleBigint& bigint2)const{
    SimpleBigint c=*this;
    c-=bigint2;
    return c;
}
//不考虑负数/0
//即：this>bigint2
SimpleBigint& SimpleBigint::operator-=(const SimpleBigint&bigint2){
    if(bigint2>=(*this)){
        *this=SimpleBigint();
        return *this;
    }
    vector<uint32_t>::iterator it1=numbers.begin();
    vector<uint32_t>::const_iterator it2=bigint2.numbers.begin();
    uint32_t next=0;
    while(it1!=numbers.end() || it2!=bigint2.numbers.end()){
        uint64_t tmp1=(*it1);
        uint64_t tmp2;
        if(it2!=bigint2.numbers.end()){
            tmp2=*it2;
            if(tmp1<tmp2+next ){
                uint64_t x=1;
                tmp1+=(x<<32);
                tmp1-=(tmp2+next);
                *it1=tmp1;
                next=1;
            }
            else{
                tmp1-=(tmp2+next);
                *it1=tmp1;
                next=0;
            }
            ++it2;
            ++it1;
            continue;
        }
        else{
            if(next){
                if(tmp1<next){
                    uint64_t x=1;
                    tmp1+=(x<<32);
                    tmp1-=(next);
                    *it1=tmp1;
                    next=1;
                }
                else{
                    tmp1-=(next);
                    *it1=tmp1;
                    next=0;
                }
            }
            ++it1;
        }
    }
    for(int i=numbers.size()-1;i>=0;--i){
        if(numbers[i]==0)numbers.pop_back();
        else{
            break;
        }
    }
    //cout<<"after -=,R="<<*this<<endl;
    return *this;
}
bool SimpleBigint::operator>(const SimpleBigint&bigint2)const{
    if(numbers.size()<bigint2.numbers.size())return false;
    else if(numbers.size()>bigint2.numbers.size())return true;
    for(int i=numbers.size()-1;i>=0;--i){
        if(numbers[i]>bigint2.numbers[i])return true;
        else if(numbers[i]<bigint2.numbers[i])return false;
        else continue;
    }
    return false;//equal
}
bool SimpleBigint::operator>=(const SimpleBigint&bigint2)const{
    if(numbers.size()<bigint2.numbers.size())return false;
    else if(numbers.size()>bigint2.numbers.size())return true;
    for(int i=numbers.size()-1;i>=0;--i){
        if(numbers[i]>bigint2.numbers[i])return true;
        else if(numbers[i]<bigint2.numbers[i])return false;
        else continue;
    }
    return true;//equal
}
bool SimpleBigint::operator<(const SimpleBigint&bigint2)const{
    //bigint2.trimnumber();
    if(numbers.size()>bigint2.numbers.size())return false;
    else if(numbers.size()<bigint2.numbers.size())return true;
    for(int i=numbers.size()-1;i>=0;--i){
        if(numbers[i]<bigint2.numbers[i])return true;
        else if(numbers[i]>bigint2.numbers[i])return false;
        else continue;
    }
    return false;//equal
}
bool SimpleBigint::operator==(const SimpleBigint&bigint2)const{
    if(numbers.size()!=bigint2.numbers.size())return false;
    vector<uint32_t>::const_iterator it1=numbers.begin();
    vector<uint32_t>::const_iterator it2=bigint2.numbers.begin();
    for(;it1!=numbers.end(),it2!=bigint2.numbers.end();++it1,++it2){
        if((*it1)!=(*it2))return false;
    }
    return true;
}

SimpleBigint SimpleBigint::operator/(const SimpleBigint&bigint2)const{
    if(bigint2.numbers.size()<=0)return SimpleBigint();
    SimpleBigint Q,R;
    Q.numbers.resize(numbers.size(),0);
    for(int i=(int)numbers.size()-1;i>=0;--i){
        R.numbers.insert(R.numbers.begin(),numbers[i]);
        if(*(R.numbers.end()-1)==0)R.numbers.pop_back();
        uint32_t mini=0,maxi=UINT32_MAX;
        uint32_t res=mini;
        while(maxi>mini){
            uint32_t reminder=(mini+maxi)%2,avg=(mini+maxi)/2;
            if(reminder)avg++;
            SimpleBigint prod=bigint2*avg;
            if(prod==R){
                res=avg;
                break;
            }
            else if(R>prod){
                mini=avg;
                res=mini;
            }
            else maxi=avg-1;
        }
        R-=(bigint2*res);
        Q.numbers[i]+=res;
    }
    return Q;
}
SimpleBigint&SimpleBigint::operator/=(const SimpleBigint&bigint2){
    *this=*this/bigint2;
    return *this;
}
SimpleBigint SimpleBigint::operator%(const SimpleBigint&bigint2)const{
    /*if(bigint2>*this)return *this;
    else if(bigint2==*this)return SimpleBigint();
    else{
        SimpleBigint tmp(*this);
        while((tmp-=bigint2)>bigint2){cout<<"processing...\n";}
        cout<<"done."<<endl;
        if(bigint2==tmp)return SimpleBigint();
        else return tmp;
    }*/
    #ifdef _DEBUG
    cout<<*this<<" mod "<<bigint2<<endl;
    #endif
    if(bigint2.numbers.size()<=0)return SimpleBigint();
    if(bigint2>*this)return *this;
    else if(bigint2==*this)return SimpleBigint();
    SimpleBigint R;
    for(int i=(int)numbers.size()-1;i>=0;--i){
        R.numbers.insert(R.numbers.begin(),numbers[i]);
        if(*(R.numbers.end()-1)==0)R.numbers.pop_back();
        uint64_t mini=0,maxi=UINT32_MAX;
        uint64_t res=mini;
        while(maxi>mini){
            uint64_t reminder=(mini+maxi)%2,avg=(mini+maxi)/2;
            if(reminder)avg++;
            SimpleBigint prod=bigint2*avg;
            //cout<<"maxi="<<maxi<<",avg="<<avg<<",mini="<<mini<<endl;
            if(prod==R){
                res=avg;
                //cout<<"res=avg="<<res<<",R==pord="<<R<<","<<prod<<endl;
                break;
            }
            else if(R>prod){
                mini=avg;
                res=mini;
                //cout<<"res=min="<<res<<",R>pord="<<R<<","<<prod<<endl;
            }
            else {
                maxi=avg-1;
                //cout<<"max="<<maxi<<",R<pord="<<R<<","<<prod<<endl;
            }
        }
        //cout<<"before -,R="<<R<<",i="<<i<<",number[i]="<<numbers[i]<<" res="<<res<<endl;
        //cout<<"bigint2*res="<<bigint2*res<<endl;
        //cout<<"r-bigint2*res="<<R-bigint2*res<<endl;
        R-=(bigint2*res);
        //cout<<"R="<<R<<",res="<<res<<endl;
    }
    return R;
}
SimpleBigint& SimpleBigint::operator%=(const SimpleBigint&bigint2){
    *this=*this%bigint2;
    return *this;
}
istream&operator>>(istream&in, SimpleBigint& bigint){
    string tmp;
    in>>tmp;
    bigint.getfromString(tmp);
    return in;
}
ostream&operator<<(ostream&out,const SimpleBigint& bigint){
    if(bigint.numbersLength()<=0)return out<<0;
    for(int i=bigint.numbersLength()-1;i>=0;--i){
        out<<hex<<fixed << setw(8) << setfill('0')<<bigint.numbers[i];
        cout<<" ";
    }
    return out;
}
int SimpleBigint::numbersLength()const{
    return numbers.size();
}
uint32_t SimpleBigint::getbyIndex(unsigned index)const{
    if(numbers.size()<=index || index<0){
            return 0;//越界
    }
    else{
        return numbers[index];
    }
}
SimpleBigint SimpleBigint::shiftDivide32(){
    //input: bits=32, R=(2^32)^(numbers.size()-1)
    //output: *this ,without the first vector element
    vector<uint32_t>::const_iterator itbegin=numbers.begin();
    numbers.erase(itbegin);
    return *this;
}
SimpleBigint SimpleBigint::getfirst(int nums){
    SimpleBigint ret;
    for(unsigned i=0;i<numbers.size() && i<nums;++i){
        ret.numbers.push_back(numbers[i]);
    }
    return ret;
}
SimpleBigint SimpleBigint::getlast(int nums){
    SimpleBigint ret;
    for(unsigned i=numbers.size()-nums;i<numbers.size();++i){
        ret.numbers.push_back(numbers[i]);
    }
    return ret;
}
SimpleBigint SimpleBigint::moveby2_divide(int num){
    SimpleBigint res=*this;
    int vectorelements=num/(int)32;
    int elementsinside=num-vectorelements*32;
    int y=0;
    for(auto it=res.numbers.begin();it!=res.numbers.end() && y<vectorelements;++y){
        if(*it!=0){
            //cerr<<"vectorelements="<<vectorelements<<endl;
            //cerr<<"hhh"<<*it<<endl;
            cerr<<"error!!!!!!!!!!!!!!!!:inside"<<endl;
            return res;
        }
        res.numbers.erase(it);
    }
    ///
/*    if((*res.numbers.begin())%(int)pow(2,elementsinside)!=0){
        cerr<<"error!!!!!!!!!!!!!!!!:"<<elementsinside<<endl;
    }*/
    unsigned maskallone=0xFFFFFFFF;
    unsigned maskres;
    //cout<<hex<<fixed << setw(8) << setfill('0')<<(maskallone>>32)<<endl;
 /*   if((res.numbers[0])%((uint32_t)pow(2,elementsinside))!=((res.numbers[0])&(maskallone>>(32-elementsinside)))){
        //cout<<"pow\n"<<hex<<fixed << setw(8) << setfill('0')<<(res.numbers[0])%((uint32_t)pow(2,elementsinside))<<endl<<((res.numbers[0])&(maskallone>>(32-elementsinside)))<<endl;
        //cout<<"pow\n"<<hex<<fixed << setw(8) << setfill('0')<<((uint32_t)pow(2,elementsinside))<<endl<<(maskallone>>(32-elementsinside))<<endl<<res.numbers[0]<<endl<<elementsinside<<endl;;
        cout<<"elemengsize="<<elementsinside<<endl<<"masknuo="<<maskres<<endl<<"mask="<<((res.numbers[0])&(maskallone>>(32-elementsinside)))<<endl;
        cout<<"xp="<<(32-elementsinside)<<endl;
    }*/
    ////=========
    if(elementsinside==0){
        maskres=0;
    }else{
        maskres=maskallone>>(32-elementsinside);
    }
    if(((res.numbers[0])&maskres)!=0){
        cerr<<"error!!!!!!!!!!!!!!!!:>>>"<<elementsinside<<endl;
    }
    ///

    uint32_t out=0;
    uint32_t mask=(0xffffffff>>(32-elementsinside));//
    for(auto it=res.numbers.end()-1;it!=res.numbers.begin()-1;--it){
        uint32_t  out2=(*it)%(int)pow(2,elementsinside);//cout<<"out:"<<out<<endl;
        //uint32_t out2=(*it)&mask;
        (*it)>>=elementsinside;//cout<<"(*it)"<<(*it)<<endl;
        //(*it)=(*it)|(out<<(32-elementsinside));
        (*it)+=out<<(32-elementsinside);//cout<<"(*it)"<<(*it)<<endl;
        out=out2;
    }
    /*for(auto it=res.numbers.end()-1;it!=res.numbers.begin()-1;--it){
        if((*it)!=0)break;
        res.numbers.erase(it);
    }*/
    res.trimnumber();
    return res;
}
void SimpleBigint::trimnumber(){
    int i=numbers.size()-1;
    for(;i>0;--i){
        if(numbers[i]==0)numbers.pop_back();
        else break;
    }
}
#ifndef _OPENMP_MOVE_MUL
SimpleBigint SimpleBigint::moveby2_mul(){
    numbers.push_back(0);
    uint32_t overflow=0;
    for(int i=0;i<numbers.size();++i){
        uint32_t mask=0x80000000;
        uint32_t thisoverflow=((numbers[i]&mask)==0)?0:1;
        numbers[i]<<=1;
        numbers[i]+=overflow;
        overflow=thisoverflow;
    }
    trimnumber();
    return *this;
}
#else
SimpleBigint SimpleBigint::moveby2_mul(){
    numbers.push_back(0);
    uint32_t overflow[numbers.size()];
    uint32_t mask=0x80000000;//num增加，时间增加
    #pragma omp parallel num_threads(1)
    {
    #pragma omp for
    for(int i=0;i<numbers.size();++i){
        overflow[i]=((numbers[i]&mask)==0)?0:1;
        numbers[i]<<=1;
    }
    #pragma omp for
    for(int i=1;i<numbers.size();++i){
        numbers[i]+=overflow[i-1];
    }
    }

    trimnumber();
    return *this;
}
#endif
