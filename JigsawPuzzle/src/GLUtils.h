#pragma once

#include <opencv2/opencv.hpp>
#include <GL/glut.h>

using namespace cv;
using namespace std;

class GLUtils
{
public:
	static void GLUtils::ConvertMatToGL(const cv::Mat& src, GLuint* texID);
};