#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "filter.h"
#include "sobel.h"
#include <stack>
#include <cmath>
#define WEAK 50
#define STRONG 255
#define EDGE 255
using namespace std;
using namespace cv;

// smooth image with 5 x 5 gaussian filter
// src is the original grayscale image, note that src's datatype must be in double
// dst is the destination for the result, it will be in CV_64F
int smoothImage(Mat_<double> src, Mat& dst);

// compute gradient on both direction using sobel
// src is the smoothed image, note that src's datatype must be in double
// magnitude is the destination for the result, it will be in CV_8U and contain magnitude of gradient
// magnitude is the destination for the result, it will be in CV_64F and contain angle of each pixel
void computeGradient(Mat_<double> src, Mat& magnitude, Mat& deg);

// detect edges by canny
// src is the grayscale image, note that src's must be double
// dst is the destination for the result, it will be in CV_64F
int detectByCany(Mat sourceImage, Mat destinationImage, float smoothScale, float smoothSize, float low = 0.3f, float high = 0.1f);

//magnitude is the gradient map compute by the computeGradient function
//angle is the angle map compute by the computeGradient function
void nonMaxSupression(Mat1d magnitude, Mat& newMag, Mat1d angle);


// threshold based on gradient magnitude
// there are 3 levels of thresholds: irrelevant, weak and strong
void myThreshold(Mat1d magn, Mat& threshold, float low, float high);

// a helper function that traces every neighbors of the current pixel to see if 
// weak pixels can be part of the edge
void followEdges(Mat1b magn, Mat1b edge, int x_ind, int y_ind);

//hysteresis function, mark every weak pixels adjacent to a strong pixel as part of the edge
void applyHysteresis(Mat1b magn, Mat edge);

// a custom point
struct my_point {
	int x, y;
};