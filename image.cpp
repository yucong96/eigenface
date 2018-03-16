#include "image.h"

/*================================��������=====================================
ReadImage����ȡ.pgm��β��ͼƬ�����������Ϣ�洢������ M ����
pgmͼƬ�ĸ�ʽ���£�
��ʽ_���_�ո�_�߶�_���ֵ_ͼƬ����
_     ����ʾ�ո�
��ʽ  ��ASCII�룬�ɴ洢���ַ����У�һ��Ϊ"P5"
���  ��ASCII�룬�ɴ洢���ַ�����
�߶�  ��ASCII�룬�ɴ洢���ַ�����
���ֵ��ASCII�룬�ɴ洢���ַ����У���Ϊ255����ÿ������ռ8λ��һ���ֽڣ���Ϊ65536����ÿ������ռ16λ�������ֽ�
һ������£����ֵ��Ϊ255�����������⣩
ͼƬ���ݣ��������룬�ٶ����ֵΪ255����ÿ������ռ8λ����һ���ֽڣ������ַ���Ϊ�����ȡ����
==============================================================================*/
Mat my_imread(const char* ImagePath) {
	//���ļ� 
	FILE *Image;
	Image = fopen(ImagePath, "rb");
	//��ȡͼƬ�������Ϣ�� ��ʽ ��� �߶� ���ֵ 
	char Fomat[20], str_cols[20], str_rows[20], str_Max[20];
	fscanf(Image, "%s %s %s %s", Fomat, str_cols, str_rows, str_Max);
	printf("Read successfully\n");
	// ��Ⱥ͸߶ȵ���Ϣ����ԭ�ȵ��ַ������飬ת��Ϊ���͵���ֵ 
	int cols = atoi(str_cols);
	int rows = atoi(str_rows);
	int Max = atoi(str_Max);
	//������ȡ���ݵľ���
	Mat M = init(rows, cols);
	//��ʼ��ȡ����
	unsigned char pixel;
	pixel = fgetc(Image); //��������Ŀո��ȡ��
	//������ض�ȡ���������ش�һ���ֽڵ�unsigned charת��Ϊdouble�������պ��������
	for (int i = 0;i < rows;++i) {
		for (int j = 0;j < cols;++j) {
			pixel = fgetc(Image);
			M->matrix[i][j] = (double)pixel;  
		}
	}
	fclose(Image);
	return M;
}
/*================================��������=====================================
WriteImage����ȡ���� M �����������Ϣ�洢��.pgm��β��ͼƬ����
pgmͼƬ�ĸ�ʽ���£�
��ʽ_���_�ո�_�߶�_���ֵ_ͼƬ����
_     ����ʾ�ո�
��ʽ  ��ASCII�룬һ��Ϊ"P5"
���  ��ASCII�룬���ھ��������
�߶�  ��ASCII�룬���ھ��������
���ֵ��ASCII�룬�洢���ַ����У���Ϊ255����ÿ������ռ8λ��һ���ֽڣ���Ϊ65536����ÿ������ռ16λ�������ֽ�
һ������£����ֵ��Ϊ255�����������⣩����˼ٶ�Ϊ255
ͼƬ���ݣ��������룬�ɽ�����Ԫ��ת��Ϊunsigned char�ͣ���Ϊ1���ֽڣ�д��ͼ��
==============================================================================*/
void my_imwrite(const char* ImagePath, Mat M) {
	FILE *Image;
	Image = fopen(ImagePath, "wb");
	//д��ͼƬ��Ϣ 
	fprintf(Image, "%s ", "P5");
	fprintf(Image, "%d ", M->cols);
	fprintf(Image, "%d ", M->rows);
	fprintf(Image, "%d ", 255);
	//д��ͼƬ����
	for (int i = 0;i < M->rows;++i) {
		for (int j = 0;j < M->cols;++j) {
			fprintf(Image, "%c", (unsigned char)M->matrix[i][j]);
		}
	}
	printf("Write successfully\n");
	fclose(Image);
}
