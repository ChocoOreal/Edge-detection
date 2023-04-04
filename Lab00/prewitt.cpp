#include "prewitt.h"
int detectByPrewitt(Mat src, Mat dst, int direction) {
	double kerV[9] = { -1.0, 0.0, 1.0,
					-1.0, 0.0, 1.0,
					-1.0, 0.0, 1.0 };
	double kerH[9] = { 1.0, 1.0, 1.0,
					  0.0, 0.0, 0.0,
					 -1.0, -1.0, -1.0 };
	Mat img_padded, temp;
	copyMakeBorder(src, img_padded, 1, 1, 1, 1, BORDER_CONSTANT);
	if (img_padded.empty()) return 0;

	Mat kernelH, kernelV, edgeH, edgeV, mag;



	if (direction == 0 || direction == -1) {
		kernelH = Mat(3, 3, CV_64F, kerH);
		edgeH = filter(img_padded, kernelH);
		if (edgeH.empty()) return 0;
	}
	if (direction == 1 || direction == -1) {
		kernelV = Mat(3, 3, CV_64F, kerV);
		edgeV = filter(img_padded, kernelV);
		if (edgeV.empty()) return 0;

	}
	if (direction == -1) {
		
		magnitude(edgeH, edgeV, temp);

	}
	else if (direction == 0) temp = edgeH;
	else if (direction == 1) temp = edgeV;
	else
		cout << "direction must be in [-1, 0, 1]";
	//dst.convertTo(dst, CV_8U, 255.0f);
	temp.copyTo(dst);
	return 1;
}