#pragma once
#include <opencv2/opencv.hpp>
#include "filter.h"
using namespace cv;
using namespace std;
int detectByPrewitt(Mat src, Mat& dst, int direction);