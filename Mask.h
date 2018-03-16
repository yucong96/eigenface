#ifndef __MASK_H__
#define __MASK_H__

#include "head.h"

class Mask
{
private:
	int maskRows, maskCols;
	int xf0, yf0, xf1, yf1;
	void rotate(Mat _src, Mat& dst, int x0, int y0, double alpha);		// src,dstΪ�Ҷ�ͼ��dstΪsrc��(x0,y0)˳ʱ����תalpha�Ƕȵ�ͼ��
	void move(Mat _src, Mat& dst, int x0, int y0, int dx, int dy);		// src,dstΪ�Ҷ�ͼ��dstΪsrc��������ƽ��(dx,dy)��ͼ��
	void scale(Mat _src, Mat& dst, int x0, int y0, double scale);		// src,dstΪ�Ҷ�ͼ��dstΪsrc��x,y����ȱ���Сscale����ͼ��
	void cut(Mat _src, Mat& dst);										// src,dstΪ�Ҷ�ͼ��dstΪsrc��maskRows,maskCols�ü����ͼ��
public:
	Mask(int maskRows = 80, int maskCols = 80, int xf0 = 20, int yf0 = 28, int xf1 = 60, int yf1 = 28);
	void process(Mat src, Mat& dst, int x0, int y0, int x1, int y1);	// ����ת����ƽ�ƣ��������
};

#endif