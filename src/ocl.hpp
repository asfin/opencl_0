#ifndef _OCL_H
#define _OCL_H
#define __NO_STD_VECTOR
#define __CL_ENABLE_EXCEPTIONS

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <CL/cl.hpp>

class OclRuntime {
    cl::vector< cl::Platform > platformList;
    cl::vector<cl::Device> devicesList;
    cl::Platform currentPlatform;
    cl::Device currentDevice;
    cl::Context context;
public:
    OclRuntime();
    ~OclRuntime();
    void run(OclTask *task);
};

class OclTask {
    cl::CommandQueue queue;
    cl::Kernel kernel;
    cl::vector< cl::Buffer * > in_buffers;
    cl::vector< cl::Buffer * > out_buffers;

    std::map<std::string, cl::Program*> _sources;
    std::map<std::string, cl::Kernel*> _kernels;

    void push_buffers();
    void pop_buffers();
public:
    OclTask(std::string src_name);
    void CompileSource(std::string name, std::string source);
    void AppendSources(std::string name);
};

#endif