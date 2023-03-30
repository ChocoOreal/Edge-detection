#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "filter.h"
#define WEAK 50
#define STRONG 255
#define EDGE 255
using namespace std;
using namespace cv;

//int imageSmoothing(Mat_<uchar> src, Mat_<uchar>& dst);
int detectByCany(Mat sourceImage, Mat& destinationImage);

struct my_point {
	int x, y;
};