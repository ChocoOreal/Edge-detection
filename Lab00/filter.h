#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;


// A function that helps convolving src with kernel
// src and kernel must be of type CV_64F
Mat filter(Mat_<double> src, Mat_<double> kernel);

// return gaussian kernel
void gaussianKernel(double* kernel, int size, float sigma);