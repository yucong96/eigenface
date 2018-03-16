#include "EigenFace.h"

#ifdef OPENCV_TEST
void cv_imshow(string path) {
	cv::Mat img = cv::imread(path);
	cv:imshow(path, img);
	cv::waitKey(100);
}
#endif

EigenFace::EigenFace(vector<Mat> images, int eigenNum, int mode)
{
	// 初始化参数
	n = images.size();
	maskRows = images[0]->rows;
	maskCols = images[0]->cols;
	col = maskRows*maskCols;
	this->eigenNum = eigenNum;
	this->mode = mode;
	
	Data = init(col, n);
	// 得到数据（按列拉伸）
	for (int i = 0; i < n; i++)
	{
		Mat image = images[i];
		for (int j = 0; j < maskCols; j++)
			for (int k = 0; k < maskRows; k++)
				Data->matrix[j*maskRows + k][i] = 1.0 * image->matrix[k][j];
	}

	computeAverage();	// 计算平均值

	if (mode == 0)
	{
		computeA();			// 计算差
		At = transpose(A);	// 转置得到At
		At_A = multiply(At, A);
		computeEigen();		// 计算特征向量
		process();			// 计算训练集描述子
	}
	else
	{
		eigenvector = getMat("out\\eigenvector.txt",col,eigenNum);	// 得到特征向量
		cout << "Get Eigenvector Finish" << endl;
		eigenvectort = transpose(eigenvector);
		result = getMat("out\\result.txt");							// 得到训练集描述子
	}
}
void EigenFace::computeAverage()
{
	// 求平均值
	Aver = init(col, 1);
	for (int i = 0; i < n; i++) {
		Aver = add(Aver, colRange(Data, i, i+1));
	}
	Aver = divide(Aver, n);
	if (mode==0) my_imwrite("out\\average.pgm", convert(Aver));
	cout << "Get Average Face." << endl;
#ifdef OPENCV_TEST
	// cv_imshow("out\\average.pgm");
#endif
}
void EigenFace::computeA()
{
	A = init(col, n);
	for (int i = 0; i < col; i++)
		for (int j = 0; j < n; j++)
			A->matrix[i][j] = Data->matrix[i][j] - Aver->matrix[i][0];
	cout << "Get Difference Matrix." << endl;
}
void EigenFace::computeEigen()
{
	Mat* eigen_things = eigen(At_A);			// At_A_eigenvector是标准正交基。md，特征向量是横着的
	At_A_eigenvector = eigen_things[0];
	eigenvalue = eigen_things[1];
	Mat At_A_eigenvectort = transpose(At_A_eigenvector);		// 将特征向量竖起来
	cout << "Compute Basic Eigenvector Finish." << endl;

	eigenvector = multiply(A, At_A_eigenvector);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < col; j++) {
			eigenvector->matrix[j][i] = eigenvector->matrix[j][i] / sqrt(eigenvalue->matrix[i][i]);		// 归一化到单位向量
		}
	}
	eigenvectort = transpose(eigenvector);
	// printMat(eigenvalue, "out\\eigenvalue.txt");
	printMat(eigenvector, "out\\eigenvector.txt");
	cout << "Compute Generated Eigenvector Finish." << endl;

	eigenvectormax = init(1, eigenNum);
	eigenvectormin = init(1, eigenNum);
	for (int i = 0; i < eigenNum; i++)
		for (int j = 0; j < col; j++)
		{
			if (eigenvector->matrix[j][i] > eigenvectormax->matrix[0][i])
				eigenvectormax->matrix[0][i] = eigenvector->matrix[j][i];
			if (eigenvector->matrix[j][i] < eigenvectormin->matrix[0][i])
				eigenvectormin->matrix[0][i] = eigenvector->matrix[j][i];
		}
	showEigen();
	cout << "Compute Max & Min Element Finish." << endl;
}
void EigenFace::process()
{
	// printMat(eigenvectort * eigenvector, "out\\identity.txt");
	result = multiply(rowRange(eigenvectort, 0, eigenNum), A);			// eigenNum*n
	printMat(result, "out\\result.txt");
}
void EigenFace::showEigen()
{
	Mat combineEigenFace = init(col, 1);
	for (int i = 0; i < eigenNum; i++)
	{
		Mat tempEigenvector = colRange(eigenvector, i, i + 1);
		float diff = eigenvectormax->matrix[0][i] - eigenvectormin->matrix[0][i];
		for (int j = 0; j < col; j++)
		{
			tempEigenvector->matrix[j][0] = (tempEigenvector->matrix[j][0] - eigenvectormin->matrix[0][i]) / diff * 255;
		}
		combineEigenFace = add(combineEigenFace, tempEigenvector);
		string name = "out\\eigenface\\eigenface_";
		name = name + _itoa(i, new char[8], 10) + ".pgm";
		Mat res = convert(tempEigenvector);
		my_imwrite(name.c_str(), res);
#ifdef OPENCV_TEST
		// cv_imshow(name);
#endif
	}
	combineEigenFace = divide(combineEigenFace, eigenNum);
	my_imwrite("out\\combineeigen.pgm", convert(combineEigenFace));
#ifdef OPENCV_TEST
	// cv_imshow("out\\combineeigen.pgm");
#endif
}

Mat EigenFace::detect(Mat src, Mat& nearest)
{
	Mat img = init(col, 1);
	// 按列拉伸
	for (int i = 0; i < maskCols; i++)
		for (int j = 0; j < maskRows; j++)
		{
			float temp = 1.0 * src->matrix[j][i];
			img->matrix[i*maskRows + j][0] = temp;
		}
	// 计算投影分量
	Mat imgRes = multiply(rowRange(eigenvectort, 0, eigenNum), subtract(img, Aver));		// eigenNum*1
	// 比较（欧氏距离）
	float min = 1e10;
	int mini = 0;
	for (int i = 0; i < n; i++)
	{
		Mat diff = subtract(imgRes, colRange(result, i, i + 1));
		Mat difft;
		difft = transpose(diff);
		Mat dist = multiply(difft, diff);
		if (dist->matrix[0][0] < min)
		{
			min = dist->matrix[0][0];
			mini = i;
		}
	}
	// 取出最近图
	nearest = colRange(Data, mini, mini + 1);
	nearest = convert(nearest);
	// 返回投影分量
	return imgRes;
}

Mat EigenFace::rebuild(Mat eigenRes)
{
	Mat res = add(multiply(colRange(eigenvector, 0, eigenNum), eigenRes), Aver);			// col*1
	res = convert(res);					// maskRows*maskCols
	return res;
}

Mat EigenFace::convert(Mat src)
{
	// 按列拉伸
	Mat res = init(maskRows, maskCols);
	for (int i = 0; i < maskCols; i++)
		for (int j = 0; j < maskRows; j++)
		{
			int temp = src->matrix[i*maskRows + j][0];
			res->matrix[j][i] = temp;
		}
	return res;
}

void EigenFace::printMat(Mat src, string path)
{
	ofstream stream(path);
	stream << src->rows << " " << src->cols << endl;
	for (int i = 0; i < src->cols; i++)
	{
		for (int j = 0; j < src->rows; j++)
			stream << src->matrix[j][i] << "\t";
		stream << endl;
	}
	stream.close();
}

Mat EigenFace::getMat(string path, int rows, int cols)
{
	ifstream stream(path);
	int filerows, filecols;
	stream >> filerows >> filecols;
	if (rows == -1) rows = filerows;
	if (cols == -1) cols = filecols;
	Mat dst = init(rows, cols);
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
			stream >> dst->matrix[j][i];
	}
	stream.close();
	return dst;
}