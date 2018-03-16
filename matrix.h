#pragma once

typedef struct {
	int rows;
	int cols;
	float** matrix;
}MatStruct, *Mat;

Mat init(int rows, int cols); //��ʼ��,����Ԫ��Ϊ0
void zeros(Mat M); // ȫ����ֵΪ0
Mat transpose(Mat A); // ת��
Mat multiply(Mat M1, Mat M2);
Mat multiply(Mat M, float D);
Mat divide(Mat M, float D);
Mat add(Mat M1, Mat M2);
Mat AddD(Mat M, float D);
Mat subtract(Mat M1, Mat M2);
Mat inverse(Mat Mori);

Mat colRange(Mat M, int A, int B); // ȡ�ض�����
Mat rowRange(Mat M, int A, int B); // ȡ�ض�����


Mat clone(Mat M); // ����
void clear(Mat M);

//SVD�ֽ�������ֵ�����صĽ������ Mat[2],�ֱ������������ľ����һ������ֵ 
Mat* eigen(Mat M);

void swap(float* pa, float* pb);