#pragma once
#include <opencv2/opencv.hpp>
#include "filter.h"
using namespace cv;
int detectByLaplace(Mat src, Mat& des);