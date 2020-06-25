__kernel void badd(__global uint*add1,__global uint*add2,__global uint*sumres,__global uint*carryres){
    int myid=get_global_id(0);
    //printf("my id is %d",myid);
    ulong addres;
    addres=add1[myid]+add2[myid];
    //printf("the add res is %x",addres);
    ulong maskme=0xFFFFFFFF;
    sumres[myid]=addres&maskme;
    carryres[myid]=addres>>32;
    while(myid!=0 && carryres[myid-1]!=0){
        addres=addres+carryres[myid-1];
        sumres[myid]=addres&maskme;
        carryres[myid]=addres>>32;
    }
}
