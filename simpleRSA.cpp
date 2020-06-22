#include"simpleRSA.h"
//#define _SIMPLE_RSA_DEBUG
/*
openssl使用方式：https://chuyao.github.io/2017/09/07/openssl-1-rsa-key/
更多生成数据与说明参见文件others/openssl_rsadata.txt
*/
bool SimpleRSA::generateKey(unsigned length){
    /*目前尚无密钥生成代码，
    使用openssl生成的密钥代替*/
    //cout<<"generating keys with length= 0X"<<length<<endl;
    switch(length){
        case 512:
            p.getfromString("e504448b8805c3a4c2918a2c72be4fb16b70438bf7abe7ddfe51c7fe0be8942b");
            q.getfromString("e0232658091f89f44c98921635049932a2ad526615c1a33f65170bfe6194ac87");
            e.getfromString("10001");
            d.getfromString("6d342c8d44cb8b0f59de091e901fddaa7be520471312f015df10b76199ee617de04dd676f53f31cc8f26a1c3e17029686fa81dd4ff2860aa8583408d8144204d");
            n.getfromString("c8832ddcd9e6477890bf173fe81e911551ed14add9edfdd6fb9b8c5ef10f5db6eb3716590dcee0a6367bf960cb6fc2ac904f879f9065758106df053c7f0f06ad");
            break;
        case 1024:
            p.getfromString("e86d43353136ce042a607928b9c5ec44b432d2d04afdcec8c1770ad3c86c14711d4bc54fa0241d4c42b3d52878dbe3d5b4ac71059a47340025ec6b74344890bd");
            q.getfromString("d8145f745b7e65ca461e16c4ecf96d4b831abd7111662011d83c47eb532474d71bc1f18dbcc3eddf0c72c2c6b24fbaa87fd227acb1337e7947e73c4cb7526f81");
            e.getfromString("10001");
            d.getfromString("5269b675e709fe71b31f19d3134bbb8bc08fa7e6c26e1e8b3d86f1d66968eb055e8980524bbfe162547010d5dc4c33b6ae497c05a046f3b12004498a4117630e02d9719e671ed734b898fc5142e95e024c4db885930cef9e5870a1740daa5d655769bb775584e73a83b83720e9e00c15c65c1feba62fde80d840bd49603ab601");
            n.getfromString("c42eafe85232e08efd6e85d3c483d7efd24854318708846cc1d72c56ce08479edcd4da6761f1a288475c77c8b3d12c1c8bf6d59e3334198da49b994ae7fa759c2c4f343fcb4c8cb00f1ddb1c20c04509c796f4aedb7aaca1bfce534d2ea633dcc43078ad1cbf8bbc520ca62269ce32142fbb91ef2c804ac3a8546b0b46dce23d");
            break;
        case 2048:
            n.getfromString("bc68197735acf01057e289ae59274361f31cf3950b82de8e53388456ff8d939d5530ebb6d7bab287444b4ad12d35bff9480f93cf8b3fa793259a948e5d0f8e20faa6dfe75391bbc6bec04ac8c08d1370d552f3c46221cc64b15280b2799a04c40d3ddfa80e5ec0e08f2bde7c2ddc1ba65366e00f859608bedfa952491c90e9db5815935969c201798565921fc24e3563b039451eb7725ec08cbd6db45c94db8e554aabcb59af66863fee281a533a0180ca419fea457c495b96f65647d5a70ebb23681416530636313f0be7e41931563e51bced52aa1a47d643d3ad8c2caeea3ce5c02445e845746cc755af4688c4c25a5443ea8d53eea9b50ad2967b9325e53d");
            d.getfromString("5d2887f6f6d664f95fcb46d2ab888b7be22edd8e43dbd8d9bcf28415545517b942c766b5c8e3aaa4d1b2fcf2830d41b82bd66ffabe5aa22c52faa51be38dee74061bff382d8d30b0ebdebedb96c1eaebeebf0e837b16452f40b28faa301064ecdfb349649a7e6596639488751463f25402872aa0e227b67952b43158af44442cf412a1c78e7100103461e660aef4038f761c04a0fbe1b592944d40ff838f7d8b219efcdfbe400f8062f25fc3aa800f2e01d0b6ea41d8d9b9872f788a97603d61a1328b893b430d7f1bfca59e97a6ddb8a2c6a5d81f3a0e01d6fef7b996822f1c3714e37cb874f5485459aebda923a53793536baf4a0976c5a93b3050892d18c1");
            p.getfromString("ec5903beb16c733049405bc2a6b8acc0c5493b2a9dd6ff0337090c551bebdeca2f6f427b9f65f4c97b4925a9150d2d07837ed3bda8f213da462557c77f2b2b9dac2abd5c1d1d3f01723d723ab26a60d58017aa8768cb25de65950aba30e49769bf87007404ca908470516980884d92e4fca43e9d247f20ec14d41b744ccb2e51");
            q.getfromString("cc1297e1d185e04f70da7ae853bd64a5e812f5821d548bf830de3f81ad2ab26806459976d2e9aca2e868ea998bf1ee13c0db9d172420829b367030840d2e57b0528ef6be745978bada2ee1e5dfbb0eea256a423a1b89a97c661642e0c94d99e1dd92e73ee2b352774d2e5aef53e73267409453bea7869891789f3bd293ed712d");
            e.getfromString("10001");
            break;
        default:
            cerr<<"wrong length"<<endl;
            return false;
    }
    #ifdef _SIMPLE_RSA_DEBUG
    cout<<"keys generated successfully"<<endl;
    #endif
    return true;
}
SimpleBigint SimpleRSA::encryptme(SimpleBigint m){
    SimpleModExp sme(m,e,n);
    #ifdef _SIMPLE_RSA_DEBUG
    cout<<"doning encrypt...\nm="<<m<<"\ne="<<e<<"\nn="<<n<<endl;
    #endif
    sme.modexp();
    #ifdef _SIMPLE_RSA_DEBUG
    cout<<"encrypt done with \nC="<<sme.cmexp<<endl;
    #endif
    return sme.cmexp;
}
SimpleBigint SimpleRSA::simdencryptme(SimpleBigint m){
    SimpleModExp sme(m,e,n);
    #ifdef _SIMPLE_RSA_DEBUG
    cout<<"doning encrypt... \nm="<<m<<"\ne="<<e<<"\nn="<<n<<endl;
    #endif
    sme.simdmodexp();
    #ifdef _SIMPLE_RSA_DEBUG
    cout<<"encrypt done with \nC="<<sme.cmexp<<endl;
    #endif
    return sme.cmexp;
}
/*
SimpleBigint SimpleRSA::decryptme(SimpleBigint c){
    SimpleModExp sme(c,d,n);
    cout<<"doning decrypt...with \nc="<<c<<"\nd="<<d<<"\nn="<<n<<endl;
    SimpleBigint M=sme.modexp();
    cout<<"decrypt done with \nM="<<M<<endl;
    return M;
}

bool SimpleRSA::checkEqual(SimpleBigint m){
    SimpleBigint C=encryptme(m);
    SimpleBigint M=decryptme(C);
    return M==m;
}
*/
