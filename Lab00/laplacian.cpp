#include "laplacian.h"
int around[3] = { -1, 0, 1 };

int detectByLaplace(Mat src, Mat des) {
	/*double ker[81] = {0, 1, 1, 2, 2, 2, 1, 1, 0,
					1, 2, 4, 5, 5, 5, 4, 2, 1,
					1, 4, 5, 3, 0, 3, 5, 4, 1,
					2, 5, 3, -12, -24, -12, 3, 5, 2, 
					2, 5, 0, -24, -40, -24, 0, 5, 2,
					2, 5, 3, -12, -24, -12, 3, 5, 2, 
					1, 4, 5, 3, 0, 3, 5, 4, 1, 
					1, 2, 4, 5, 5, 5, 4, 2, 1, 
					0, 1, 1, 2, 2, 2, 1, 1, 0};*/
	double ker[25] = { 0, 0, 1, 0, 0,
					0, 1, 2, 1, 0,
					1, 2, -16, 2, 1,
					0, 1, 2, 1, 0,
					0, 0, 1, 0, 0 };


	/*double* G = new double[blurKernelSize * blurKernelSize];
	gaussianKernel(G, blurKernelSize, blurKernelVar);
	int blurPadding = blurKernelSize / 2;
	Mat GKer(blurKernelSize, blurKernelSize, CV_64F,G);
	Mat blurred;
	copyMakeBorder(src, blurred, blurPadding, blurPadding, blurPadding, blurPadding, BORDER_CONSTANT);
	if (blurred.empty()) {
		return 0;
	}
	blurred = filter(blurred, GKer);*/

	Mat img_padded;
	copyMakeBorder(src, img_padded, 2, 2, 2, 2, BORDER_CONSTANT);
	if (img_padded.empty()) return 0;
	Mat kernel(5, 5, CV_64F, ker); 
	Mat edge = filter(img_padded, kernel);
	edge.copyTo(des);
	//zero crossing
	
	//des = Mat::zeros(edge.rows, edge.cols, CV_64F);
	/*if (edge.empty()) return 0;
	for (int i = 1; i < edge.rows - 1; i++) {
		for (int j = 1; j < edge.cols - 1; j++) {
			int neg = 0, pos = 0;
			for (int k = 0; k < 3; k++) {
				for (int m = 0; m < 3; m++) {
					double pixel_value = edge.at<double>(i + around[k], j + around[m]);
					if (pixel_value > 0) {
						pos++;
					}
					else if(pixel_value < 0)
					{
						neg++;
					}
				}
			}
			if (neg > 0 && pos > 0) {
				des.at<double>(i, j) = 1.0;
			}
		}
	}*/
	
	return 1;
}