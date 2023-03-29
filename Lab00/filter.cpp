# include "filter.h"

Mat filter(Mat_<double> src, Mat_<double> kernel) {

	int r_src = src.rows;
	int c_src = src.cols;

	int r_ker = kernel.rows;
	int c_ker = kernel.cols;
	Mat result(r_src - r_ker + 1, c_src - c_ker + 1, CV_64F);

	int row = 0 ;
	for (int i = 0; i <= r_src - r_ker; i++) {
		int col = 0;
		for (int j = 0; j <= c_src - c_ker; j++) {
			double s = 0;
			
			for (int m = i; m < i + r_ker; m++) {
				for (int n = j; n < j + c_ker; n++) {
					//cout << "m, n: " << m << "," << n << '\t' << m - i << "," << n - j << "\t";
					s += src.at<double>(m, n) * kernel.at<double>(m - i, n - j);
				}
				//cout << "\n";
			}
			// Add s to the final result
			result.at<double>(row, col++) = s;
		}
		row++;
	}
	return result;
}