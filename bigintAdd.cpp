#include"layersModexpOpenCL.h"




int testME()
{
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobj = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;

	char string[MEM_SIZE];

	FILE *fp;
	char fileName[] = "./layersModexpOpenCL.cl";
	char *source_str;
	size_t source_size;

	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
		cerr<<"Failed to load kernel"<<endl;
		return;
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
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);
	//创建kernel程序
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "hello", &ret);

	//设置kernel参数
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

	//执行kernel
	ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);

	//从membuffer获取执行结果
	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
		MEM_SIZE * sizeof(char),string, 0, NULL, NULL);

	//显示结果
	puts(string);

	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);

	getchar();
	return 0;

}

