#include <utility>
#include <iostream>

#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>

#include "ocl.hpp"
#include "factories.hpp"


using namespace std;


int main() {
    try {
        OclRuntime cl;
        shared_ptr<OclTask> task = cl.CreateTask("ocl_src");
        task->TransferBuffers();
        task->Run();
        task->GetResult();
    } catch (cl::Error e) {
        std::cerr << e.err() << std::endl;
        std::cerr << e.what() << std::endl;
    }

//    OclTask clTask("ocl_src", (std::shared_ptr()), (std::basic_string<char, char_traits < _CharT>, allocator < _CharT >> ()));
//    cl.run();

//    cv::Mat image;
//    image.create(300, 400, CV_8UC1);
//    raytrace(image);
//
//    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
//    cv::imshow("Image", image);
//
//    cv::waitKey(0);

    cout << "Finish" << endl;
    return 0;
}