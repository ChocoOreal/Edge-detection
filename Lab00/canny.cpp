#include "canny.h"
#include "sobel.h"

int smoothImage(Mat_<double> src, Mat& dst) {
	double gaussianKernel[25] = { 2, 4, 5, 4, 2,
								4, 9, 12, 9, 4,
								5, 12, 15, 12, 5,
								4, 9, 12, 9, 4,
								2, 4, 5, 4, 2, };
	Mat img_padded;
	copyMakeBorder(src, img_padded, 2, 2, 2, 2, BORDER_CONSTANT);
	if (img_padded.empty()) return 0;
	Mat kernel(5, 5, CV_64F, gaussianKernel);
	dst = filter(img_padded, kernel);
	//dst.convertTo(dst, CV_8U);
	if (dst.empty()) return 0;
	/*edgeV.convertTo(edgeV, CV_8UC1, 1 / 256.0);
	edgeH.convertTo(edgeH, CV_8UC1, 1 / 256.0);*/
	dst /= 159.0f;
	return 1;

}

void computeGradient(Mat_<double> src, Mat& magnitude, Mat& deg) {
	Mat Ix, Iy;
	detectBySobel(src, Ix, 0);
	detectBySobel(src, Iy, 1);
	magnitude = Mat(src.rows, src.cols, CV_64F);
	deg = Mat(src.rows, src.cols, CV_64F);
	/*Ix.convertTo(Ix, CV_64F);
	Iy.convertTo(Iy, CV_64F);*/
	cartToPolar(Ix, Iy, magnitude, deg, true);
	magnitude.convertTo(magnitude, CV_8U, 255.0);
}

//magnitude is the gradient map compute by the computeGradient function.
void non_max_supression(Mat1b magnitude, Mat1d angle) {
	// Neu deg > 180 thi lay deg - 180
	for (int i = 1; i < magnitude.rows - 1; i++) {
		for (int j = 1; j < magnitude.cols - 1; j++) {
			double r = 1.0, q = 1.0;
			
			//angle is 45
			if (22.5 <= angle.at<double>(i, j) && angle.at<double>(i, j) < 67.5) {
				r = magnitude.at<uchar>(i - 1, j - 1);
				q = magnitude.at<uchar>(i + 1, j + 1);
			}
			//angle is 90
			else if (67.5 <= angle.at<double>(i, j) && angle.at<double>(i, j) < 112.5) {
				r = magnitude.at<uchar>(i - 1, j);
				q = magnitude.at<uchar>(i + 1, j);
			}
			//angle is 135
			else if (112.5 <= angle.at<double>(i, j) && angle.at<double>(i, j) < 157.5) {
				r = magnitude.at<uchar>(i - 1, j + 1);
				q = magnitude.at<uchar>(i + 1, j - 1);
			} // angle is 0
			else {
				r = magnitude.at<uchar>(i, j + 1);
				q = magnitude.at<uchar>(i, j - 1);
			}

			double max_ = (q > r) ? q : r;
			if (magnitude.at<uchar>(i, j) < max_) {
				magnitude.at<uchar>(i, j) = 0;
			}
		}
	}
}

void my_threshold(Mat1b magn, float low, float high, uchar weak, uchar strong) {
	double max_, min_; 
	minMaxLoc(magn, &min_, &max_);
	float highThreshold = high * max_;
	float lowThreshold = low * highThreshold;
	for (int i = 0; i < magn.rows; i++) {
		for (int j = 0; j < magn.cols; j++) {
			if (magn.at<uchar>(i, j) < lowThreshold) {
				magn.at<uchar>(i, j) = 0;
			}
			else if (magn.at<uchar>(i, j) > highThreshold) {
				magn.at<uchar>(i, j) = strong;
			}
			else {
				magn.at<uchar>(i, j) = weak;
			}
		}
	}
}


void hysteresis(Mat1b magn, uchar strong, uchar weak) {
	int around[3] = { -1, 0, 1 };
	
	for (int i = 1; i < magn.rows - 1; i++) {
		for (int j = 1; j < magn.cols - 1; j++) {
			if (magn.at<uchar>(i, j) == weak)
			{
				if (magn.at<uchar>(i - 1, j - 1) == strong ||
					magn.at<uchar>(i - 1, j) == strong ||
					magn.at<uchar>(i - 1, j + 1) == strong ||
					magn.at<uchar>(i, j - 1) == strong ||
					magn.at<uchar>(i, j + 1) == strong ||
					magn.at<uchar>(i + 1, j - 1) == strong ||
					magn.at<uchar>(i + 1, j) == strong ||
					magn.at<uchar>(i + 1, j + 1) == strong)
					magn.at<uchar>(i, j) = strong;
				else magn.at<uchar>(i, j) = 0;
			}
			
		}
	}
}

int detectByCany(Mat sourceImage, Mat& destinationImage) {
	Mat src, mag, angle;
	uchar weak = 50, strong = 255;
	smoothImage(sourceImage, src);
	computeGradient(src, mag, angle);
	non_max_supression(mag, angle);
	my_threshold(mag, 0.9f, 0.2f, weak, strong);
	hysteresis(mag, strong, weak);
	destinationImage = mag;
	return 0;
}
