/*
 *  Simple OpenCL demo program
 *
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  gcc -o cldemo -std=gnu99 -Wall -I/usr/include/nvidia-current cldemo.c -lOpenCL
 *
 */

#include <CL/cl.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <sys/time.h>

#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


 static struct timeval start,stop,diff;




static int    time_substract(struct timeval *result, struct timeval *begin,struct timeval *end)

{

    if(begin->tv_sec > end->tv_sec)    return -1;

    if((begin->tv_sec == end->tv_sec) && (begin->tv_usec > end->tv_usec))    return -2;



    result->tv_sec    = (end->tv_sec - begin->tv_sec);

    result->tv_usec    = (end->tv_usec - begin->tv_usec);

    

    if(result->tv_usec < 0)

    {

        result->tv_sec--;

        result->tv_usec += 1000000;

    }



    return 0;

}

void pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
	fprintf(stderr, "OpenCL Error (via pfn_notify): %s\n", errinfo);
}


static int initflag = 0;



int NUM_DATA  = 0;

static cl_context context;
static cl_program program;

static cl_mem input_buffer;

static cl_kernel kernel;

static cl_command_queue queue;

static 	cl_mem output_buffer;

static	cl_device_id devices[100];
static 	cl_uint devices_n = 0;

cl_mem  outclImage;
cl_mem  inclImage;





#define CL_CHECK(_expr)                                                         \
   do {                                                                         \
     cl_int _err = _expr;                                                       \
     if (_err == CL_SUCCESS)                                                    \
       break;                                                                   \
     fprintf(stderr, "OpenCL Error: '%s' returned %d!\n", #_expr, (int)_err);   \
     abort();                                                                   \
   } while (0)

#define CL_CHECK_ERR(_expr)                                                     \
   ({                                                                           \
     cl_int _err = CL_INVALID_VALUE;                                            \
     typeof(_expr) _ret = _expr;                                                \
     if (_err != CL_SUCCESS) {                                                  \
       fprintf(stderr, "OpenCL Error: '%s' returned %d!\n", #_expr, (int)_err); \
       abort();                                                                 \
     }                                                                          \
     _ret;                                                                      \
   })




static int ksjcreateimage2d(int width,int height){
 // Create OpenCL image  
    cl_image_format clImageFormat;
    clImageFormat.image_channel_order = CL_A;
    clImageFormat.image_channel_data_type = CL_UNORM_INT8;

    cl_int errNum;

  	inclImage = clCreateImage2D(context,CL_MEM_READ_ONLY ,&clImageFormat,width,height,0,NULL,&errNum);

    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Error creating CL image object" << std::endl;
        return 0;
    }
}

static int heheksjcreateimage2d(size_t width,size_t height){
 // Create OpenCL image  
    cl_image_format clImageFormat;
    clImageFormat.image_channel_order = CL_RGB;
    clImageFormat.image_channel_data_type = CL_UNORM_INT8;

    cl_int errNum;

     outclImage = clCreateImage2D(context,CL_MEM_WRITE_ONLY,&clImageFormat,width,height,0,NULL,&errNum);

    // outclImage = clCreateImage(context,CL_MEM_WRITE_ONLY,&clImageFormat,width,height,0,NULL,&errNum);

    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Error creating CL image object" << std::endl;
        return 0;
    }

	printf("outclImage created \n");
}




static int cl_init(int width,int height,unsigned char* inbuf,unsigned char*outbuf)
{

	NUM_DATA  = width*height;

	cl_platform_id platforms[100];
	cl_uint platforms_n = 0;
	CL_CHECK(clGetPlatformIDs(100, platforms, &platforms_n));

	printf("=== %d OpenCL platform(s) found: ===\n", platforms_n);
	for (int i=0; i<platforms_n; i++)
	{
		char buffer[10240];
		printf("  -- %d --\n", i);
		CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 10240, buffer, NULL));
		printf("  PROFILE = %s\n", buffer);
		CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 10240, buffer, NULL));
		printf("  VERSION = %s\n", buffer);
		CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 10240, buffer, NULL));
		printf("  NAME = %s\n", buffer);
		CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 10240, buffer, NULL));
		printf("  VENDOR = %s\n", buffer);
		CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL));
		printf("  EXTENSIONS = %s\n", buffer);
	}

	if (platforms_n == 0)
		return 1;


	// CL_CHECK(clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 100, devices, &devices_n));
	CL_CHECK(clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 100, devices, &devices_n));
2,
	printf("=== %d OpenCL device(s) found on platform:\n", platforms_n);
	for (int i=0; i<devices_n; i++)
	{
		char buffer[10240];
		cl_uint buf_uint;
		cl_ulong buf_ulong;
		printf("  -- %d --\n", i);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL));
		printf("  DEVICE_NAME = %s\n", buffer);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL));
		printf("  DEVICE_VENDOR = %s\n", buffer);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL));
		printf("  DEVICE_VERSION = %s\n", buffer);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL));
		printf("  DRIVER_VERSION = %s\n", buffer);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL));
		printf("  DEVICE_MAX_COMPUTE_UNITS = %u\n", (unsigned int)buf_uint);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL));
		printf("  DEVICE_MAX_CLOCK_FREQUENCY = %u\n", (unsigned int)buf_uint);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
		printf("  DEVICE_GLOBAL_MEM_SIZE = %llu\n", (unsigned long long)buf_ulong);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_IMAGE_SUPPORT, sizeof(buf_ulong), &buf_ulong, NULL));
		printf("  CL_DEVICE_IMAGE_SUPPORT = %llu\n", (unsigned long long)buf_ulong);


	}

	if (devices_n == 0)
		return 1;

	context = CL_CHECK_ERR(clCreateContext(NULL, 1, devices, &pfn_notify, NULL, &_err));
#if 0
	const char *program_source[] = {
		"__kernel void simple_demo(__global int *src, __global int *dst, int factor)\n",
		"{\n",
		"	int i = get_global_id(0);\n",
		"	printf(\"i=%d  \",i);\n",
		"	dst[i] = src[i] * factor;\n",
		"}\n"
	};

	const char *program_source[] = {
		"const sampler_t sampler = CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;\n" 
		"//__kernel void bgr2gray(__global unsigned char *src, __write_only image2d_t dst)\n" 
		"//__kernel void bgr2gray(__write_only image2d_t*src, __write_only image2d_t dst)\n" 
		"__kernel void bgr2gray(__global unsigned char *src)\n"  
		"//__kernel void bgr2gray(__global unsigned char *src, __global  image2d_t dst)\n"  
		"{\n"  
		"    int x = (int)get_global_id(0);\n"  
		"    int y = (int)get_global_id(1);\n"  
		" //   if (x >= get_image_width(dst) || y >= get_image_height(dst))\n"  
		" //       return; \n" 

		"  //  write_imagef(dst,(int2)(x,y),(float4)(255,0,0,1.0f));\n"  
		"}\n"  
		"__kernel void simple_demo(__global unsigned char *src, __global unsigned char *dst, int factor)\n",
		"{\n",
		"	int i = get_global_id(0);\n",
		"	dst[i] = src[i] * factor;\n",
		"}\n",

		"__kernel void binarization(__global unsigned char *src, __global unsigned char *dst, int factor)\n",
		"{\n",
		"	int i = get_global_id(0);\n",
		"	int j = get_global_id(1);\n ",
		"	if(src[i+j*1280]<127) { dst[i+j*1280]=0;\n"
		"	} else{dst[i+j*1280]=255;}\n",
		"}\n",
		"__kernel void bayer(__global unsigned char *src, __global unsigned char *dst, int factor)\n",
		"{\n",
		"	int i = get_global_id(0);\n",
		"	int j = get_global_id(1);\n",

		"if((i==100)&&(j==200))  printf(\"ij is meet\");\n",

		"}\n"
		"__kernel void test(__global unsigned char *src, __global unsigned char *dst, int factor)\n",
		"{\n",
		"	int i = get_global_id(0);\n",
		"	dst[i]=255;\n",
		"}\n"


	};

	program = CL_CHECK_ERR(clCreateProgramWithSource(context, sizeof(program_source)/sizeof(*program_source), program_source, NULL, &_err));
#endif

		  std::ifstream file("bayer.cl");
		  std::stringstream content_stream;
		  content_stream << file.rdbuf();
		  std::string source =content_stream.str();

		  const char * source_str = source.c_str();
  		  const char ** buffer = &source_str;



	program = CL_CHECK_ERR(clCreateProgramWithSource(context, 1, buffer, NULL, &_err));



	if (clBuildProgram(program, 1, devices, "", NULL, NULL) != CL_SUCCESS) {
		char buffer[10240];
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
		fprintf(stderr, "CL Compilation failed:\n%s", buffer);
		abort();
	}
	CL_CHECK(clUnloadCompiler());

	







	input_buffer = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(char)*NUM_DATA, NULL, &_err));


	output_buffer = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_WRITE_ONLY, (sizeof(char)*NUM_DATA)*3, NULL, &_err));


	


	
	queue = CL_CHECK_ERR(clCreateCommandQueue(context, devices[0], 0, &_err));






	heheksjcreateimage2d(width,height);
	ksjcreateimage2d(width,height);
	

	printf("opencl init done \n");

}


static int cl_release()
{
	if(initflag==1)
{

	CL_CHECK(clReleaseMemObject(input_buffer));
	CL_CHECK(clReleaseMemObject(output_buffer));

	CL_CHECK(clReleaseKernel(kernel));
	CL_CHECK(clReleaseProgram(program));
	CL_CHECK(clReleaseContext(context));
	initflag = 0;	
	return 0;
}else
{

printf("opencl not inited can not release \n");
return 1;
}

}

cl_bool ImageSupport(cl_device_id device)
{
    cl_bool imageSupport = CL_FALSE;
    clGetDeviceInfo(device, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool),
        &imageSupport, NULL);
    return imageSupport;
}




int cl_entrance(char* kernelname,int width,int height,unsigned char* inbuf,unsigned char*outbuf)
{
	


	printf("width =  %d     height =  %d   \n",width,height);

	if(initflag==0) {
	cl_init(width,height,inbuf,outbuf);

    	memset(&start,0,sizeof(struct timeval));

    	memset(&stop,0,sizeof(struct timeval));

    	memset(&diff,0,sizeof(struct timeval));


	

	int factor = 2;

	kernel = CL_CHECK_ERR(clCreateKernel(program, kernelname, &_err));
	CL_CHECK(clSetKernelArg(kernel, 0, sizeof(input_buffer), &input_buffer));

	CL_CHECK(clSetKernelArg(kernel, 1, sizeof(output_buffer), &output_buffer));
	initflag=1;

		}
	//CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), &inclImage));

	//printf("size of sizeof(cl_mem) = %d \n",sizeof(cl_mem));
	//CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), &outclImage));

	//CL_CHECK(clSetKernelArg(kernel, 2, sizeof(factor), &factor));


    	gettimeofday(&start,0);

	CL_CHECK(clEnqueueWriteBuffer(queue, input_buffer, CL_TRUE, 0,NUM_DATA , inbuf, 0, NULL, NULL));



 	 size_t origin[] = {0,0,0};
 	 size_t region[] = {width, height, 1};

//	CL_CHECK(clEnqueueWriteImage(queue, inclImage, CL_TRUE, origin, region, 0, 0, (void*)inbuf, 0, NULL, NULL));





	cl_event kernel_completion;
	size_t dim=2;
	size_t global_work_size_d1[] = { NUM_DATA };

	size_t global_work_size[] = { width,height };
	size_t local_work_size[1] = { 1 };

	size_t global_offset[]={0,0};

	CL_CHECK(clEnqueueNDRangeKernel(queue, kernel, 2, 0, global_work_size, NULL, 0, NULL, &kernel_completion));
	//CL_CHECK(clEnqueueNDRangeKernel(queue, kernel, 1, 0, global_work_size_d1, NULL, 0, NULL, &kernel_completion));
	CL_CHECK(clWaitForEvents(1, &kernel_completion));
	CL_CHECK(clReleaseEvent(kernel_completion));
    gettimeofday(&stop,0);
    time_substract(&diff,&start,&stop);
    printf("cl real Total time : %d s,%d us\n",(int)diff.tv_sec,(int)diff.tv_usec);


	CL_CHECK(clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, NUM_DATA*3, outbuf, 0, NULL, NULL));

	//cl_release();

	return 0;
}

