#include "canny.h"
#include "sobel.h"
#include <stack>
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
	kernel /= 159.0;
	dst = filter(img_padded, kernel);
	//dst.convertTo(dst, CV_8U);
	if (dst.empty()) return 0;
	//dst /= 159.0f;
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

void my_threshold(Mat1b magn, float low, float high) {
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
				magn.at<uchar>(i, j) = STRONG;
			}
			else {
				magn.at<uchar>(i, j) = WEAK;
			}
		}
	}
}


void hysteresis(Mat1b magn) {
	
	for (int i = 1; i < magn.rows - 1; i++) {
		for (int j = 1; j < magn.cols - 1; j++) {
			if (magn.at<uchar>(i, j) == WEAK)
			{
				if (magn.at<uchar>(i - 1, j - 1) == STRONG ||
					magn.at<uchar>(i - 1, j) == STRONG ||
					magn.at<uchar>(i - 1, j + 1) == STRONG ||
					magn.at<uchar>(i, j - 1) == STRONG ||
					magn.at<uchar>(i, j + 1) == STRONG ||
					magn.at<uchar>(i + 1, j - 1) == STRONG ||
					magn.at<uchar>(i + 1, j) == STRONG ||
					magn.at<uchar>(i + 1, j + 1) == STRONG)
					magn.at<uchar>(i, j) = STRONG;
				else magn.at<uchar>(i, j) = 0;
			}
			
		}
	}
}

void followEdges(Mat1b magn, Mat1b edge, int x_ind, int y_ind) {
	int x[8] = { 1,1,0,-1,-1,-1,0,1 },
		y[8] = { 0,1,1,1,0,-1,-1,-1 };

	stack<my_point> s;
	s.push(my_point{ x_ind, y_ind });


	while (!s.empty()) {
		my_point current = s.top();
		s.pop();

		// since the current point belongs to edge, we check out the neighbors
		for (int i = 0; i < 8; i++) {
			
			int tx_ind = current.x + x[i], ty_ind = current.y + y[i];
			

			// check the bound of neighbors
			if (tx_ind >= 0 && tx_ind < magn.rows && ty_ind >= 0 && ty_ind < magn.cols &&
				edge.at<uchar>(tx_ind, ty_ind) == 0) 
				// edge[i, j] = 0 means we havent check out location i, j


				if (magn.at<uchar>(tx_ind, ty_ind) == WEAK ||
					magn.at<uchar>(tx_ind, ty_ind) == STRONG) {
					// if the current considered neighbor has the potential to be an edge or 
					// it is a true edge, we mark it as edge

					magn.at<uchar>(tx_ind, ty_ind) = STRONG;
					edge.at<uchar>(tx_ind, ty_ind) = EDGE;

					// push it to stack so that we will later consider its neighbors
					s.push(my_point{ tx_ind, ty_ind });
				}
		}
	}
	
}

void applyHysteresis(Mat1b magn, Mat& edge) {
	edge = Mat::zeros(magn.rows, magn.cols, CV_8U);
	for (int i = 0; i < magn.rows; i++) {
		for (int j = 0; j < magn.cols; j++) {


			//Find the first point belongs to edge and it hasnt been added to edge
			if (magn.at<uchar>(i, j) == STRONG && edge.at<uchar>(i, j) == 0) {
				edge.at<uchar>(i, j) = EDGE;
				followEdges(magn, edge, i, j);
			}
		}
	}
}

int detectByCany(Mat sourceImage, Mat& destinationImage) {
	Mat src, mag, angle, edge;
	smoothImage(sourceImage, src);
	computeGradient(src, mag, angle);
	non_max_supression(mag, angle);
	my_threshold(mag, 0.35f, 0.5f);
	applyHysteresis(mag, edge);
	destinationImage = edge;
	return 0;
}
