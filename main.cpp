#include "head.h"
#include "EigenFace.h"
#include "FileManager.h"
#include "image.h"

string dir = "att_faces";						// 图片库路径
int personNum = 40, faceNum = 9;				// 人数，每人人脸数

int maskRows = 80, maskCols = 80;				// 40*40模板
int xf0 = 20, yf0 = 28, xf1 = 60, yf1 = 28;		// 模板瞳孔坐标

int eigenNum = 20;		// 特征向量数量
int mode = 0;

#ifdef OPENCV_TEST
void cv_imshow_main(string path) {
	cv::Mat img = cv::imread(path);
	cv:imshow(path, img);
	cv::waitKey(100);
}
#endif

int main()
{
	int temp;
	cout << "请输出特征向量数量（低于训练集图片数量，-1为默认值20）：" << endl;
	cin >> temp;
	if (temp > 0) eigenNum = temp;

	while (1)
	{
		cout << "请输出模式，0为训练模式，1为测试模式，-1退出：" << endl;
		cin >> mode;
		if (mode == -1) break;

		FileManager fileManager = FileManager(dir, personNum, faceNum);
		if (mode==0) fileManager.processPhotos();
		cout << "All Photo has been preprocessed." << endl;
		vector<Mat> vec = fileManager.getPhotos();
		cout << "All Photo has been loaded." << endl;

		EigenFace eigenFace(vec, eigenNum, mode);
		cout << "Training Set has been trained." << endl;
		cout << "训练完毕，可以在out文件中查看平均脸、特征值、训练集描述子和特征脸" << endl;

		if (mode == 1)
		{
			string temp;
			cout << "输入待检测人脸路径（\\需要写成\\\\，不允许出现空格，默认测试请输入-1）：" << endl;
			cin >> temp;
			if (temp == "-1")		// 默认测试
			{
				for (int i = 1; i <= 18; i++)
				{
					string testPath = "test\\10_p (";
					testPath = testPath + _itoa(i, new char[8], 10);
					testPath = testPath + ").pgm";
					Mat img = my_imread(testPath.c_str());
					Mat nearest;
					Mat result = eigenFace.detect(img, nearest);
					Mat rebuild = eigenFace.rebuild(result);

					string src = "test\\src";
					src = src + _itoa(i, new char[8], 10);
					src += ".pgm";
					my_imwrite(src.c_str(), img);
#ifdef OPENCV_TEST
					cv_imshow_main(src);
					cv::moveWindow(src, show_width * ((i - 1) % show_num_in_a_row), 0 + 3 * show_height * ((i - 1) / show_num_in_a_row));
#endif

					string near = "test\\near";
					near = near + _itoa(i, new char[8], 10);
					near += ".pgm";
					my_imwrite(near.c_str(), nearest);
#ifdef OPENCV_TEST
					cv_imshow_main(near);
					cv::moveWindow(near, show_width * ((i - 1) % show_num_in_a_row), show_height + 3 * show_height * ((i - 1) / show_num_in_a_row));
#endif

					string res = "test\\rebuild";
					res = res + _itoa(i, new char[8], 10);
					res += ".pgm";
					my_imwrite(res.c_str(), rebuild);
#ifdef OPENCV_TEST
					cv_imshow_main(res);
					cv::moveWindow(res, show_width * ((i - 1) % show_num_in_a_row), 2 * show_height + 3 * show_height * ((i - 1) / show_num_in_a_row));
#endif
				}
			}
			else
			{
				string testPath = temp;
				Mat img = my_imread(testPath.c_str());
				Mat nearest;
				Mat result = eigenFace.detect(img, nearest);
				Mat rebuild = eigenFace.rebuild(result);
			}
		}
		cout << "Finish" << endl;
	}
	system("pause");
	return 0;
}
