#ifndef __EIGENFACE_H__
#define __EIGENFACE_H__

#include "head.h"

#include <vector>

class EigenFace
{
private:
	int n;
	int maskRows, maskCols;
	int col;
	int eigenNum;
	int mode;

	Mat Data;				// col*n
	Mat Aver;				// col*1
	Mat A;					// col*n
	Mat At;					// n*col
	Mat At_A;				// n*n
	Mat eigenvalue;			// n*n
	Mat At_A_eigenvector;	// n*n
	Mat eigenvector;		// col*n
	Mat eigenvectort;		// n*col
	Mat eigenvectormax;		// 1*eigenNum
	Mat eigenvectormin;		// 1*eigenNum
	Mat result;				// eigenNum*n

	void computeAverage();
	void computeA();
	void computeEigen();
	void process();
	void showEigen();

	void printMat(Mat src, string path);
	Mat getMat(string path, int rows = -1, int cols = -1);
	Mat convert(Mat src);
public:
	EigenFace(vector<Mat> images, int eigenNum, int mode);
	Mat detect(Mat src, Mat& nearest);
	Mat rebuild(Mat eigenRes);
};

#endif