#include "ocl.hpp"

#define buflen 5

OclRuntime::OclRuntime() {

    cl::Platform::get(&platformList);

    std::cout << "Platforms count: " << platformList.size() << std::endl;
    if (!platformList.size()) exit(0);

    std::string info;
    for (auto i: platformList) {
        i.getInfo((cl_platform_info) CL_PLATFORM_VENDOR, &info);
        std::cout << info << ": ";
        i.getInfo((cl_platform_info) CL_PLATFORM_NAME, &info);
        std::cout << info << std::endl;
    }

    currentPlatform = platformList[0];

    currentPlatform.getDevices(CL_DEVICE_TYPE_ALL, &devicesList);
    std::cout << "Devices count: " << devicesList.size() << std::endl;
    currentDevice = devicesList[0];

    sptr_context = std::make_shared<cl::Context>(devicesList);
}

std::shared_ptr<OclTask> OclRuntime::CreateTask(std::string name) {
    std::shared_ptr<OclTask> task = std::make_shared<OclTask>(sptr_context, currentDevice, name);
    return task;
}

void OclTask::push_buffers() {
    size_t elem_sz = sizeof(int);

    if (auto context = wptr_context.lock()) {

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
    } else {
        std::cerr << "Cannot lock wptr_context\n";
    }
}

void OclTask::pop_buffers() {
    size_t elem_sz = sizeof(int);
    int *out = new int[buflen]();

    cl::Buffer *buf = out_buffers[0];

    queue->enqueueReadBuffer(*buf, CL_TRUE, 0, elem_sz*buflen, out);

    for (auto i = 0; i < buflen; i++) {
        std::cout << out[i] << ' ';
    }
    std::cout << std::endl;
}

void OclTask::LoadSource(std::string name) {
    auto filename = "ocl_src/" + name + ".cl";
    std::ifstream ocl_src_f(filename);
    std::stringstream buf;
    buf << ocl_src_f.rdbuf();
    std::string ocl_src = buf.str().c_str();

    CompileSource(name, ocl_src);
}

void OclTask::CompileSource(std::string name, std::string source) {
    if (source.length() < 20) {
        std::cerr << "Source file too small. Check paths.\n";
        exit(1);
    }
    cl::Program::Sources cl_src;
    cl_src.push_back({source.c_str(), source.length()});
    if (auto context = wptr_context.lock()) {
        program = new cl::Program(*context, cl_src);
        cl::vector<cl::Device> dev;
        dev.push_back(device);
        if (program->build(dev) != CL_SUCCESS) {
            std::cerr << "ERROR: \n" << program->getBuildInfo< CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        }
        kernel = new cl::Kernel(*program, name.c_str());
    } else {
        std::cerr << "Cannot lock wptr_context\n";
    }
}

OclTask::OclTask(std::shared_ptr<cl::Context> sptr_context, cl::Device device, std::string name) {
    this->name = name;
    this->device = device;
    wptr_context = sptr_context;
    LoadSource(name);
    queue = new cl::CommandQueue(*sptr_context, device);
}

void OclTask::Run() {
    TransferBuffers();
    int i = 0;
    for (int j = 0; j < in_buffers.size(); j++, i++) {
        auto buf = in_buffers[j];
        kernel->setArg(j, *buf);
    }
    for (int j = 0; j < out_buffers.size(); j++) {
        auto buf = out_buffers[j];
        kernel->setArg(i+j, *buf);
    }
    queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(buflen), cl::NullRange);
//    queue->finish();
}

void OclTask::TransferBuffers() {
    if (already_transferred) {
        return;
    }
    push_buffers();
    already_transferred = true;

}

void OclTask::GetResult() {
    pop_buffers();

}
