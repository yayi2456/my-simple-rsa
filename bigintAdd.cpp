#include"bigintAdd.h"



SimpleBigint bigintAdd(SimpleBigint a1,SimpleBigint a2)
{
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;

	FILE *fp;
	char fileName[] = "./bigintAdd.cl";
	char *source_str;
	size_t source_size;

	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
		cerr<<"Failed to load kernel"<<endl;
		return SimpleBigint(0,0);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	//发现并初始化可用平台
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	//发现并初始化设备
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	//创建上下文环境
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	//创建命令队列
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	//创建设备缓冲区
	//memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);
	//处理a1、a2
	int maxlength=a1.numbersLength()>a2.numbersLength()?a1.numbersLength():a2.numbersLength();
	void*a1mem=malloc(maxlength*sizeof(uint32_t));
	void*a2mem=malloc(maxlength*sizeof(uint32_t));
	memset(a1mem,0,maxlength*sizeof(uint32_t));
	memset(a2mem,0,maxlength*sizeof(uint32_t));
	memcpy(a1mem,&(a1.numbers[0]),sizeof(uint32_t)*a1.numbersLength());
	memcpy(a2mem,&(a2.numbers[0]),sizeof(uint32_t)*a2.numbersLength());
	cl_mem a1memobj=clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,maxlength*sizeof(uint32_t),a1mem,&ret);
	cl_mem a2memobj=clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,maxlength*sizeof(uint32_t),a2mem,&ret);
	cl_mem sumresobj=clCreateBuffer(context,CL_MEM_READ_WRITE,maxlength*sizeof(uint32_t),nullptr,&ret);
	cl_mem carryobj=clCreateBuffer(context,CL_MEM_READ_WRITE,maxlength*sizeof(uint32_t),nullptr,&ret);
	//创建kernel程序
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "badd", &ret);

	//设置kernel参数
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a1memobj);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a2memobj);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&sumresobj);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&carryobj);

	//执行kernel
	//ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
	size_t global_work_size[1] = {maxlength};
    cl_event enentPoint;
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, &enentPoint);
    clWaitForEvents(1,&enentPoint); //wait
    clReleaseEvent(enentPoint);

	//从membuffer获取执行结果
	void*sumres=malloc(sizeof(uint32_t)*(maxlength+2));
	void*carryres=malloc(sizeof(uint32_t)*(maxlength));

	ret = clEnqueueReadBuffer(command_queue, sumresobj, CL_TRUE, 0,
		maxlength * sizeof(uint32_t),sumres+sizeof(uint32_t), 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue, carryobj, CL_TRUE, 0,
		maxlength * sizeof(uint32_t),carryres, 0, NULL, NULL);
    memcpy(sumres+sizeof(uint32_t)*(maxlength+1),carryres+sizeof(uint32_t)*(maxlength-1),sizeof(uint32_t));
    memcpy(sumres,&maxlength,sizeof(uint32_t));
    SimpleBigint finaladdres=SimpleBigint(sumres);
    finaladdres.trimnumber();
    //puts(string);

	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a1memobj);
	ret = clReleaseMemObject(a2memobj);
	ret = clReleaseMemObject(sumresobj);
	ret = clReleaseMemObject(carryobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);
	free(a1mem);
	free(a2mem);
	free(sumres);
	free(carryres);

	//getchar();
	return  finaladdres;

}

