#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
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

int main(int argc, char* argv[]) {
	
	
	
	const String keys =
		"{help h usage ? || print this message   }"
		"{@path|| path to image}"
		"{@method|| choose method for edge detection}"
		"{direction|b| h for horizontal, v for vertical}"
		"{smooth-scale sm|1.5| variance for gaussian kernel}"
		"{smooth-size ss|5| size of the gaussian kernel}"
		"{low-threshold lt|0.3| low threshold}"
		"{high-threshold ht|0.2| high threshold}"
		;
	CommandLineParser parser(argc, argv, keys);
	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}
	string path = parser.get<string>("@path");
	string method = parser.get<string>("@method");
	string direction = parser.get<string>("direction");
	Mat img = readImg(path);
	Mat dst(img.rows, img.cols, CV_64F);
	if (method == "sobel") {
		if (direction == "h") {
			detectBySobel(img, dst, 0);
		}
		else if (direction == "v") {
			detectBySobel(img, dst, 1);
		}
		else if (direction == "b") {
			detectBySobel(img, dst, -1);

		}
		
			
	}
	else if (method == "prewitt") {
		if (direction == "h") {
			detectByPrewitt(img, dst, 0);
		}
		else if (direction == "v") {
			detectByPrewitt(img, dst, 1);
		}
		else if (direction == "b") {
			detectByPrewitt(img, dst, -1);

		}


	}
	else if (method =="laplace") {
		detectByLaplace(img, dst);
	}
	else if (method == "canny") {
		float smoothScale = 1.5f;
		int smoothSize = 5;
		float low = 0.3f, high = 0.3f;
		if (parser.has("smooth-size")) smoothSize = parser.get<int>("smooth-size");
		if (parser.has("smooth-scale")) smoothScale = parser.get<int>("smooth-scale");
		if (parser.has("low-threshold")) low = parser.get<float>("low-threshold");
		if (parser.has("high-threshold")) high = parser.get<float>("high-threshold");
		dst = Mat::zeros(img.rows, img.cols, CV_8U);
		detectByCany(img, dst, smoothScale, smoothSize, low, high);
	}
	else if (method == "cannycv") {

		float low = 300.0f, high = 600.0f;
		if (parser.has("low-threshold")) low = parser.get<float>("low-threshold");
		if (parser.has("high-threshold")) high = parser.get<float>("high-threshold");
		img.convertTo(img, CV_8U, 255.0f);
		Canny(img, dst, low, high);
	}

	if (method == "all") {
		detectBySobel(img, dst, -1);
		imshow("Sobel", dst);
		waitKey(0);

		detectByPrewitt(img, dst, -1);
		imshow("Prewitt", dst);
		waitKey(0);

		detectByLaplace(img, dst);
		imshow("Laplacian", dst);
		waitKey(0);

		dst = Mat::zeros(img.rows, img.cols, CV_8U);
		detectByCany(img, dst, 1.5f, 5);
		imshow("Canny", dst);
		waitKey(0);
	}
	
	else {
		imshow("after filtering", dst);
		waitKey(0);
	}
	
	
	return 0;
}