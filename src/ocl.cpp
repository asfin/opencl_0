#include "ocl.hpp"

#define buflen 5

Ocl::Ocl() {

    std::ifstream ocl_src_f("ocl_src.cl");
    std::stringstream buf;
    buf << ocl_src_f.rdbuf();
    std::string ocl_src = buf.str();

    platformList = new cl::vector<cl::Platform>[10];
    devicesList = new cl::vector<cl::Device>[10];
    cl::Platform::get(platformList);

    std::cout << "Platforms count: " << platformList->size() << std::endl;
    if (!platformList->size()) exit(0);

    std::string info;
    for (auto i: *platformList) {
        i.getInfo((cl_platform_info) CL_PLATFORM_VENDOR, &info);
        std::cout << info << ": ";
        i.getInfo((cl_platform_info) CL_PLATFORM_NAME, &info);
        std::cout << info << std::endl;
    }

    currentPlatform = platformList[0];

    currentPlatform->getDevices(CL_DEVICE_TYPE_ALL, devicesList);
    std::cout << "Devices count: " << devicesList->size() << std::endl;
    currentDevice = devicesList[0];

    context = new cl::Context((cl::vector<cl::Device, 10> const &) devicesList);

    cl::Program::Sources cl_src;
    cl_src.push_back({ocl_src.c_str(), ocl_src.length()});
    cl::Program p(*context, cl_src);
    if (p.build(*devicesList) != CL_SUCCESS) {
        std::cerr << p.getBuildInfo< CL_PROGRAM_BUILD_LOG>(*currentDevice) << std::endl;
    }
    queue = new cl::CommandQueue((cl::Context const &) context, (cl::Device const &) currentDevice);

//    cl::KernelFunctor simple_add(cl::Kernel(p, "simple_add"), queue, cl::NullRange, cl::NDRange(buflen), cl::NullRange);
    kernel = new cl::Kernel(p, "simple_add");
}

void Ocl::push_buffers() {
    size_t elem_sz = sizeof(int);
    int *h_buf = new int[buflen]();

    cl::Buffer *_buf;
    _buf = new cl::Buffer(*context, CL_MEM_READ_WRITE, elem_sz*buflen);
    in_buffers.push_back(_buf);
    _buf = new cl::Buffer(*context, CL_MEM_READ_WRITE, elem_sz*buflen);
    in_buffers.push_back(_buf);
    _buf = new cl::Buffer(*context, CL_MEM_READ_WRITE, elem_sz*buflen);
    out_buffers.push_back(_buf);

    for (auto buf: in_buffers) {
        queue->enqueueWriteBuffer(*buf, CL_TRUE, 0, elem_sz*buflen, h_buf);
    }
}

void Ocl::pop_buffers() {
    size_t elem_sz = sizeof(int);
    int *out = new int[buflen]();

    cl::Buffer buf = *out_buffers[0];

    queue->enqueueReadBuffer(buf, CL_TRUE, 0, elem_sz*buflen, out);

    for (auto i = 0; i < buflen; i++) {
        std::cout << out[i] << ' ';
    }
    std::cout << std::endl;
}

void Ocl::run() {
    push_buffers();
    int i = 0;
    for (int j = 0; j < in_buffers.size(); j++, i++) {
        kernel->setArg(j, in_buffers[j]);
    }
    for (int j = 0; j < out_buffers.size(); j++) {
        auto buf = out_buffers[j];
        kernel->setArg(i+j, *buf);
    }
    queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(buflen), cl::NullRange);
    pop_buffers();
}

Ocl::~Ocl() {
    queue = nullptr;
    context = nullptr;
}
