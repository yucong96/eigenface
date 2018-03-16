#ifndef __FILEMANAGER_H__
#define __FILEMANAFER_H__

#include "head.h"
#include "Mask.h"

#include <vector>

class FileManager
{
private:
	string dir;
	int personNum, faceNum;
	Mask mask;
public:
	FileManager(string dir, int personNum, int faceNum, int maskRows = 80, int maskCols = 80, int xf0 = 20, int yf0 = 28, int xf1 = 60, int yf1 = 28);
	void processPhotos();
	vector<Mat> getPhotos();
};

#endif