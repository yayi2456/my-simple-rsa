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

	//创建命令队列，enable性能测试
	command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

	//创建设备缓冲区
	//memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);
	//处理a1、a2
	int maxlength=a1.numbersLength()>a2.numbersLength()?a1.numbersLength():a2.numbersLength();
	void*a1mem=malloc(maxlength*sizeof(cl_uint));
	void*a2mem=malloc(maxlength*sizeof(cl_uint));
	memset(a1mem,0,maxlength*sizeof(cl_uint));
	memset(a2mem,0,maxlength*sizeof(cl_uint));
	memcpy(a1mem,&(a1.numbers[0]),sizeof(cl_uint)*a1.numbersLength());
	memcpy(a2mem,&(a2.numbers[0]),sizeof(cl_uint)*a2.numbersLength());
	cl_mem a1memobj=clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,maxlength*sizeof(cl_uint),a1mem,&ret);
	cl_mem a2memobj=clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,maxlength*sizeof(cl_uint),a2mem,&ret);
	cl_mem sumresobj=clCreateBuffer(context,CL_MEM_READ_WRITE,maxlength*sizeof(cl_uint),nullptr,&ret);
	cl_mem carryobj=clCreateBuffer(context,CL_MEM_READ_WRITE,maxlength*sizeof(cl_uint),nullptr,&ret);

	//创建kernel程序
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "badd", &ret);
	if(ret!=0){
        cout<<"clBuildProgram ret="<<ret<<endl;
        size_t len;
        char buffer[8 * 1024];
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
    }

	//设置kernel参数
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a1memobj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&a2memobj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&sumresobj);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&carryobj);
	if(ret!=0){
        cout<<"clBuildProgram ret="<<ret<<endl;
        printf("Error: Failed to set kernel Arg!\n");
    }

	//执行kernel
	//ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);

	size_t global_work_size[1] = {maxlength};
    cl_event enentPoint;
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, &enentPoint);
    clWaitForEvents(1,&enentPoint); //wait
    clReleaseEvent(enentPoint);
    //获取性能数据
    #ifdef _TEST_TIME_KERNEL_
    cl_ulong time_start, time_end;
    double total_time;
    clGetEventProfilingInfo(enentPoint, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(enentPoint, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    double kernel_interv=(time_end-time_start)/1000000.0;
    cout<<"Execution time in milliseconds of kernel ="<<kernel_interv<<" ms"<<endl;
    #endif
	//从membuffer获取执行结果
	void*sumres=malloc(sizeof(cl_uint)*(maxlength+2));
	void*carryres=malloc(sizeof(cl_uint)*(maxlength));

	ret = clEnqueueReadBuffer(command_queue, sumresobj, CL_TRUE, 0,
		maxlength * sizeof(cl_uint),sumres+sizeof(cl_uint), 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue, carryobj, CL_TRUE, 0,
		maxlength * sizeof(cl_uint),carryres, 0, NULL, NULL);
    memcpy(sumres+sizeof(cl_uint)*(maxlength+1),carryres+sizeof(cl_uint)*(maxlength-1),sizeof(cl_uint));
    memcpy(sumres,&maxlength,sizeof(cl_uint));
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

