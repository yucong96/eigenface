#include "image.h"

/*================================函数功能=====================================
ReadImage：读取.pgm结尾的图片，并将相关信息存储到矩阵 M 当中
pgm图片的格式如下：
格式_宽度_空格_高度_最大值_图片数据
_     ：表示空格
格式  ：ASCII码，可存储在字符串中，一般为"P5"
宽度  ：ASCII码，可存储在字符串中
高度  ：ASCII码，可存储在字符串中
最大值：ASCII码，可存储在字符串中，若为255，即每个像素占8位，一个字节，若为65536，即每个像素占16位，两个字节
一般情况下，最大值都为255（极少数例外）
图片数据：二进制码，假定最大值为255，则每个像素占8位，即一个字节，可用字符作为载体读取出来
==============================================================================*/
Mat my_imread(const char* ImagePath) {
	//打开文件 
	FILE *Image;
	Image = fopen(ImagePath, "rb");
	//读取图片的相关信息： 格式 宽度 高度 最大值 
	char Fomat[20], str_cols[20], str_rows[20], str_Max[20];
	fscanf(Image, "%s %s %s %s", Fomat, str_cols, str_rows, str_Max);
	printf("Read successfully\n");
	// 宽度和高度的信息，从原先的字符串数组，转换为整型的数值 
	int cols = atoi(str_cols);
	int rows = atoi(str_rows);
	int Max = atoi(str_Max);
	//创建读取数据的矩阵
	Mat M = init(rows, cols);
	//开始读取数据
	unsigned char pixel;
	pixel = fgetc(Image); //将多出来的空格读取掉
	//逐个像素读取，并将像素从一个字节的unsigned char转化为double，方便日后矩阵运算
	for (int i = 0;i < rows;++i) {
		for (int j = 0;j < cols;++j) {
			pixel = fgetc(Image);
			M->matrix[i][j] = (double)pixel;  
		}
	}
	fclose(Image);
	return M;
}
/*================================函数功能=====================================
WriteImage：读取矩阵 M ，并将相关信息存储到.pgm结尾的图片当中
pgm图片的格式如下：
格式_宽度_空格_高度_最大值_图片数据
_     ：表示空格
格式  ：ASCII码，一般为"P5"
宽度  ：ASCII码，等于矩阵的列数
高度  ：ASCII码，等于矩阵的行数
最大值：ASCII码，存储在字符串中，若为255，即每个像素占8位，一个字节，若为65536，即每个像素占16位，两个字节
一般情况下，最大值都为255（极少数例外），因此假定为255
图片数据：二进制码，可将矩阵元素转化为unsigned char型，变为1个字节，写入图像
==============================================================================*/
void my_imwrite(const char* ImagePath, Mat M) {
	FILE *Image;
	Image = fopen(ImagePath, "wb");
	//写入图片信息 
	fprintf(Image, "%s ", "P5");
	fprintf(Image, "%d ", M->cols);
	fprintf(Image, "%d ", M->rows);
	fprintf(Image, "%d ", 255);
	//写入图片数据
	for (int i = 0;i < M->rows;++i) {
		for (int j = 0;j < M->cols;++j) {
			fprintf(Image, "%c", (unsigned char)M->matrix[i][j]);
		}
	}
	printf("Write successfully\n");
	fclose(Image);
}
