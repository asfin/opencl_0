#ifndef _OCL_H
#define _OCL_H
#define __NO_STD_VECTOR
#define __CL_ENABLE_EXCEPTIONS

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <CL/cl.hpp>
#include <memory>

class OclTask {
    std::string name;
    std::weak_ptr<cl::Context> wptr_context;
    cl::Device device;
    cl::CommandQueue *queue;
    cl::Kernel *kernel;
    cl::Program *program;
    cl::vector< cl::Buffer * > in_buffers;
    cl::vector< cl::Buffer * > out_buffers;

    void push_buffers();
    void pop_buffers();
    void CompileSource(std::string name, std::string source);
    void LoadSource(std::string name);
    bool already_transferred = false;
public:
    OclTask(std::shared_ptr<cl::Context> sptr_context, cl::Device device, std::string name);
    void TransferBuffers();
    void Run();
    void GetResult();
};

class OclRuntime {
    cl::vector< cl::Platform > platformList;
    cl::vector<cl::Device> devicesList;
    cl::Platform currentPlatform;
    cl::Device currentDevice;
//    cl::Context context;
    std::shared_ptr<cl::Context> sptr_context;
public:
    OclRuntime();
    std::shared_ptr<OclTask> CreateTask(std::string name);
};

#endif
