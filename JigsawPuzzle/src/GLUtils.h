#pragma once

#include <opencv2/opencv.hpp>
#include <GL/glut.h>

using namespace cv;
using namespace std;

class GLUtils
{
public:
	static void GLUtils::convertMatToGL(const cv::Mat& src, GLuint* texID);
	static void GLUtils::overwriteTexture(const cv::Mat& src, const GLuint& texID);
};