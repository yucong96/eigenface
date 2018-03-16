#ifndef __MASK_H__
#define __MASK_H__

#include "head.h"

class Mask
{
private:
	int maskRows, maskCols;
	int xf0, yf0, xf1, yf1;
	void rotate(Mat _src, Mat& dst, int x0, int y0, double alpha);		// src,dst为灰度图，dst为src绕(x0,y0)顺时针旋转alpha角度的图像
	void move(Mat _src, Mat& dst, int x0, int y0, int dx, int dy);		// src,dst为灰度图，dst为src向正方向平移(dx,dy)的图像
	void scale(Mat _src, Mat& dst, int x0, int y0, double scale);		// src,dst为灰度图，dst为src向x,y方向等比缩小scale倍的图像
	void cut(Mat _src, Mat& dst);										// src,dst为灰度图，dst为src按maskRows,maskCols裁剪后的图像
public:
	Mask(int maskRows = 80, int maskCols = 80, int xf0 = 20, int yf0 = 28, int xf1 = 60, int yf1 = 28);
	void process(Mat src, Mat& dst, int x0, int y0, int x1, int y1);	// 先旋转，再平移，最后缩放
};

#endif