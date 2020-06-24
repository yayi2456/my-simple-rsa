__kernel void badd(__global uint32_t*add1,__global uint32_t*add2,__global uint32_t*sumres,__global uint32_t*carryres){
    int myid=get_global_id(0);
    uint64_t addres;
    addres=add1[myid]+add2[myid];
    uint64_t maskme=0xFFFFFFFF;
    sumres[myid]=addres&maskme;
    carryres[myid]=addres>>32;
    while(myid!=0 && carryres[myid-1]!=0){
        addres=addres+carryres[myid-1];
        sumres[myid]=addres&maskme;
        carryres[myid]=addres>>32;
    }
}
