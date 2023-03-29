#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "filter.h"
using namespace std;
using namespace cv;

//int imageSmoothing(Mat_<uchar> src, Mat_<uchar>& dst);
int detectByCany(Mat sourceImage, Mat& destinationImage);