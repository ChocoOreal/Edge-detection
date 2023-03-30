#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include "filter.h"
#include "sobel.h"
#include "prewitt.h"
#include "laplacian.h"
#include "canny.h"
using namespace cv;
using namespace std;
Mat readImg(string fname) {
	Mat img = imread(fname);
	imshow("original image", img);
	waitKey(0);

	Mat imgray;
	cvtColor(img, imgray, COLOR_BGR2GRAY);
	imgray.convertTo(imgray, CV_64F, 1.0 / 255.0); //convert to double
	return imgray;
}

int main() {
	
	Mat img = readImg("..\\..\\..\\..\\Downloads\\lenna.png");
	
	Mat dst;
	detectByCany(img, dst);
	imshow("after filtering", dst);
	waitKey(0);
	return 0;
}