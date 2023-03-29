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
	
	Mat img = readImg("..\\..\\..\\..\\Downloads\\new_tab.png");
	
	Mat dst;
	detectByCany(img, dst);
	imshow("after filtering", dst);
	waitKey(0);
	/*double x[9] = { 1, 5,-2, 3, 4, 5, 7, 9,10 };
	double y[9] = { 1, 4, -5, 7, -9,10, -5,2, 3 };
	Mat Ix(3, 3, CV_64F, x);
	Mat Iy(3, 3, CV_64F, y);
	Mat mag, angle;
	cartToPolar(Ix, Iy, mag, angle, true);
	cout << mag;
	cout << angle;*/
	return 0;
}