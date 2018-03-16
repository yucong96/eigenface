#include "Mask.h"

void Mask::rotate(Mat _src, Mat& dst, int x0, int y0, double alpha)	// src,dst为灰度图，dst为src绕(x0,y0)顺时针旋转alpha角度的图像
{
	Mat src = clone(_src);

	double matrix[2][2] = { { cos(alpha), -sin(alpha) }, { sin(alpha), cos(alpha) } };

	for (int i = 0; i < dst->rows; i++)
		for (int j = 0; j < dst->cols; j++)
		{
			double srcj = matrix[0][0] * (j - x0) + matrix[0][1] * (i - y0) + x0;
			double srci = matrix[1][0] * (j - x0) + matrix[1][1] * (i - y0) + y0;
			if (srci >= 0 && srci <= src->rows - 1 && srcj >= 0 && srcj <= src->cols - 1)
			{
				double rateX = srcj - (int)srcj;
				double rateY = srci - (int)srci;
				if (srci == src->rows - 1 || srcj == src->cols - 1)
					dst->matrix[i][j] = src->matrix[(int)srci][(int)srcj];
				else
					dst->matrix[i][j] = 0.5*((1 - rateX) * src->matrix[(int)srci][(int)srcj] + rateX * src->matrix[(int)srci][(int)srcj + 1])
						+ 0.5*((1 - rateY) * src->matrix[(int)srci][(int)srcj] + rateY * src->matrix[(int)srci + 1][(int)srcj]);
			}
			else dst->matrix[i][j] = 0;
		}
}

void Mask::move(Mat _src, Mat& dst, int x0, int y0, int dx, int dy)	// src,dst为灰度图，dst为src向正方向平移(dx,dy)的图像
{
	Mat src = clone(_src);

	for (int i = 0; i < dst->rows; i++)
		for (int j = 0; j < dst->cols; j++)
		{
			int srci = i - dy;
			int srcj = j - dx;
			if (srci >= 0 && srci <= src->rows - 1 && srcj >= 0 && srcj <= src->cols - 1)
			{
				dst->matrix[i][j] = src->matrix[srci][srcj];
			}
			else dst->matrix[i][j] = 0;
		}
}

void Mask::scale(Mat _src, Mat& dst, int x0, int y0, double scale)	// src,dst为灰度图，dst为src向x,y方向等比缩小scale倍的图像
{
	Mat src = clone(_src);

	for (int i = 0; i < dst->rows; i++)
		for (int j = 0; j < dst->cols; j++)
		{
			double srci = (i - y0)*scale + y0;
			double srcj = (j - x0)*scale + x0;
			if (srci >= 0 && srci <= src->rows - 1 && srcj >= 0 && srcj <= src->cols - 1)
			{
				double rateX = srcj - (int)srcj;
				double rateY = srci - (int)srci;
				if (srci == src->rows - 1 || srcj == src->cols - 1)
					dst->matrix[i][j] = src->matrix[(int)srci][(int)srcj];
				else
					dst->matrix[i][j] = 0.5*((1 - rateX) * src->matrix[(int)srci][(int)srcj] + rateX * src->matrix[(int)srci][(int)srcj + 1])
					+ 0.5*((1 - rateY) * src->matrix[(int)srci][(int)srcj] + rateY * src->matrix[(int)srci + 1][(int)srcj]);
			}
			else dst->matrix[i][j] = 0;
		}
}

void Mask::cut(Mat _src, Mat& dst)	// src,dst为灰度图，dst为src按maskRows,maskCols裁剪后的图像
{
	Mat src = clone(_src);
	dst = init(maskRows, maskCols);

	for (int i = 0; i < dst->rows; i++) {
		for (int j = 0; j < dst->cols; j++)
		{
			int srci = i;
			int srcj = j;
			if (srci >= 0 && srci <= src->rows - 1 && srcj >= 0 && srcj <= src->cols - 1)
			{
				dst->matrix[i][j] = src->matrix[srci][srcj];
			}
			else dst->matrix[i][j] = 0;
		}
	}
}

Mask::Mask(int maskRows, int maskCols, int xf0, int yf0, int xf1, int yf1)
{
	this->maskRows = maskRows;
	this->maskCols = maskCols;
	this->xf0 = xf0;
	this->yf0 = yf0;
	this->xf1 = xf1;
	this->yf1 = yf1;
}

void Mask::process(Mat src, Mat& dst, int x0, int y0, int x1, int y1)	// 先旋转，再缩放，最后平移
{
	// 复制图像
	Mat temp = init(src->rows*2, src->cols*2);
	for (int i = 0; i < temp->rows; i++)
		for (int j = 0; j < temp->cols; j++)
		{
			if (i >= src->rows / 2 && i < src->rows / 2 + src->rows && j >= src->cols / 2 && j < src->cols / 2 + src->cols)
				temp->matrix[i][j] = src->matrix[i - src->rows / 2][j - src->cols / 2];
			else temp->matrix[i][j] = 255;
		}
	x0 += src->cols / 2;
	y0 += src->rows / 2;
	x1 += src->cols / 2;
	y1 += src->rows / 2;
	// 旋转
	double alpha = atan(1.0*(y1 - y0) / (x1 - x0));
	rotate(temp, temp, x0, y0, alpha);
	x0 = x0; y0 = y0; x1 = x0 + sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)); y1 = y0;
	// 缩放
	double smallScale = 1.0*(x1 - x0) / (xf1 - xf0);
	scale(temp, temp, x0, y0, smallScale);
	x0 = x0; y0 = y0; x1 = (x1 - x0) / smallScale + x0; y1 = (y1 - y0) / smallScale + y0;
	// 平移
	int dx = xf0 - x0;
	int dy = yf0 - y0;
	move(temp, temp, x0, y0, dx, dy);
	x0 = x0 + dx; y0 = y0 + dy; x1 = x1 + dx; y1 = y1 + dy;
	// 裁剪
	cut(temp, dst);
}