#ifndef __HEAD_H__
#define __HEAD_H__

#include <math.h>
#include <iostream>
#include <fstream>
#include "matrix.h"
#include "image.h"

using namespace std;

// #define OPENCV_TEST

#ifdef OPENCV_TEST
#include <opencv2\opencv.hpp>
const int show_width = 150;
const int show_height = 120;
const int show_num_in_a_row = 9;
#endif

#endif