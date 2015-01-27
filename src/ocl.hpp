#ifndef _OCL_H
#define _OCL_H
#define __NO_STD_VECTOR
#define __CL_ENABLE_EXCEPTIONS

#include <fstream>
#include <sstream>
#include <iostream>
#include <CL/cl.hpp>

class Ocl {
    cl::vector< cl::Platform > *platformList;
    cl::vector< cl::Device > *devicesList;
    cl::Platform *currentPlatform;
    cl::Device *currentDevice;
    cl::Context *context;
    cl::CommandQueue *queue;
    cl::Kernel *kernel;
    cl::vector< cl::Buffer * > in_buffers;
    cl::vector< cl::Buffer * > out_buffers;
    void push_buffers();
    void pop_buffers();
public:
    Ocl();
    ~Ocl();
    void run();
};

#endif