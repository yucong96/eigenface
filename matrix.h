#pragma once

typedef struct {
	int rows;
	int cols;
	float** matrix;
}MatStruct, *Mat;

Mat init(int rows, int cols); //初始化,所有元素为0
void zeros(Mat M); // 全部赋值为0
Mat transpose(Mat A); // 转置
Mat multiply(Mat M1, Mat M2);
Mat multiply(Mat M, float D);
Mat divide(Mat M, float D);
Mat add(Mat M1, Mat M2);
Mat AddD(Mat M, float D);
Mat subtract(Mat M1, Mat M2);
Mat inverse(Mat Mori);

Mat colRange(Mat M, int A, int B); // 取特定的列
Mat rowRange(Mat M, int A, int B); // 取特定的行


Mat clone(Mat M); // 拷贝
void clear(Mat M);

//SVD分解求特征值，返回的结果存入 Mat[2],分别是特征向量的矩阵和一行特征值 
Mat* eigen(Mat M);

void swap(float* pa, float* pb);