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

	//���ֲ���ʼ������ƽ̨
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	//���ֲ���ʼ���豸
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	//���������Ļ���
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	//�����������
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	//�����豸������
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);
	//����kernel����
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "hello", &ret);

	//����kernel����
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

	//ִ��kernel
	ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);

	//��membuffer��ȡִ�н��
	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
		MEM_SIZE * sizeof(char),string, 0, NULL, NULL);

	//��ʾ���
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

