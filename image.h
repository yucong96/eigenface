#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream>
#include <fstream>
#include<sstream>
#include "matrix.h"
#include <string>
using namespace std;

Mat my_imread(const char* ImagePath);
void my_imwrite(const char* ImagePath, Mat M);

#endif // !_IMAGE_H

