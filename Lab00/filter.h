#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
Mat filter(Mat_<double> src, Mat_<double> kernel);