#pragma once
#include <opencv2/opencv.hpp>
#include "filter.h"
using namespace cv;

// detect edges by laplacian
// src is the grayscale image, note that src's must be double
// dst is the destination for the result, it will be in CV_64F
int detectByLaplace(Mat src, Mat des);