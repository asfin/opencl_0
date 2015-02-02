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
    OclRuntime cl;
    OclTask clTask("ocl_src");
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