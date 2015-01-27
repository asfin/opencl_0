cmake_minimum_required(VERSION 2.8.4)
project(opencl_0)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/ocl.cpp)

include_directories(/usr/lib64/OpenCL/global/include/)
#link_directories(/usr/lib64/OpenCL/vendors/intel/)
add_executable(opencl_0 ${SOURCE_FILES})
target_link_libraries(opencl_0 OpenCL ${OpenCV_LIBS})
