#include "FileManager.h"

FileManager::FileManager(string dir, int personNum, int faceNum, int maskRows, int maskCols, int xf0, int yf0, int xf1, int yf1)
{
	this->dir = dir;
	this->personNum = personNum;
	this->faceNum = faceNum;
	mask = Mask(maskRows, maskCols, xf0, yf0, xf1, yf1);
}

void FileManager::processPhotos()
{
	for (int i = 1; i <= personNum; i++)
		for (int j = 1; j <= faceNum; j++)
		{
			// ∂¡»°Õº∆¨
			string picPath = dir + "\\s" + _itoa(i, new char[8], 10) + "\\" + _itoa(j, new char[8], 10) + ".pgm";
			Mat img = my_imread(picPath.c_str());
			
			// ∂¡»°Õ´ø◊Œª÷√
			string txtPath = dir + "\\s" + _itoa(i, new char[8], 10) + "\\" + _itoa(j, new char[8], 10) + ".txt";
			ifstream txtStream(txtPath);
			int x1,y1,x0,y0;
			txtStream >> x1 >> y1 >> x0 >> y0;
			txtStream.close();

			// ¥¶¿ÌÕº∆¨
			Mat dst;
			mask.process(img, dst, x0, y0, x1, y1);

			// ±£¥ÊÕº∆¨
			string dstPath = dir + "\\s" + _itoa(i, new char[8], 10) + "\\" + _itoa(j, new char[8], 10) + "_p.pgm";
			my_imwrite(dstPath.c_str(), dst);

			clear(img);
			clear(dst);
			cout << i << " person's " << j << " photo has been preprocessed." << endl;
		}
}

vector<Mat> FileManager::getPhotos()
{
	vector<Mat> vec;
	for (int i = 1; i <= personNum; i++)
		for (int j = 1; j <= faceNum; j++)
		{
			// ∂¡»°Õº∆¨
			string picPath = dir + "\\s" + _itoa(i, new char[8], 10) + "\\" + _itoa(j, new char[8], 10) + "_p.pgm";
			Mat img = my_imread(picPath.c_str());
			vec.push_back(img);

			cout << i << " person's " << j << " photo has been loaded." << endl;
		}
	return vec;
}