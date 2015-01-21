#include <utility>
#define __NO_STD_VECTOR
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>
#include <sstream>

using namespace std;

int main() {

    ifstream ocl_src_f("ocl_src.cl");
    stringstream buf;
    buf << ocl_src_f.rdbuf();
    string ocl_src = buf.str();

    cl::vector< cl::Platform > platformList;
    cl::Platform::get(&platformList);

    cout << "Count: " << platformList.size() << endl;
    if (!platformList.size()) exit(0);

    string info;
    for (auto i: platformList) {
        i.getInfo((cl_platform_info) CL_PLATFORM_VENDOR, &info);
        cout << info << ": ";
        i.getInfo((cl_platform_info) CL_PLATFORM_NAME, &info);
        cout << info << endl;
    }

    cl::Platform currentPlatform = platformList[0];
    cl::vector< cl::Device > devices;
    currentPlatform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    cout << "Devices: " << devices.size() << endl;
    cl::Context context(devices);
    cl::Program::Sources cl_src;
    cl_src.push_back({ocl_src.c_str(), ocl_src.length()});
    cl::Program p(context, cl_src);
    if (p.build(devices) != CL_SUCCESS) {
        cerr << p.getBuildInfo< CL_PROGRAM_BUILD_LOG>(devices[0]) << endl;
    }

    size_t buflen = 1000000;
    int *h_a = new int[buflen]();
    int *h_b = new int[buflen]();

    cl::Buffer buf_a(context, CL_MEM_READ_WRITE, sizeof(int)*buflen);
    cl::Buffer buf_b(context, CL_MEM_READ_WRITE, sizeof(int)*buflen);
    cl::Buffer buf_c(context, CL_MEM_READ_WRITE, sizeof(int)*buflen);

    cl::CommandQueue queue(context, devices[0]);
    queue.enqueueWriteBuffer(buf_a, CL_TRUE, 0, sizeof(int)*buflen, h_a);
    queue.enqueueWriteBuffer(buf_b, CL_TRUE, 0, sizeof(int)*buflen, h_b);

    cl::KernelFunctor simple_add(cl::Kernel(p, "simple_add"), queue, cl::NullRange, cl::NDRange(buflen), cl::NullRange);
    simple_add(buf_a, buf_b, buf_c);
    int *out = new int[buflen];
    queue.enqueueReadBuffer(buf_c, CL_TRUE, 0, sizeof(int)*buflen, out);

//    for (auto i = 0; i < buflen; i++) {
//        cout << out[i] << ' ';
//    }
//    cout << endl;

    cout << "Finish" << endl;
    return 0;
}