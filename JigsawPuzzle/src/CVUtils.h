#pragma once
#include <opencv2/opencv.hpp>

class CVUtils
{
public:
	static cv::Mat PinP(const cv::Mat &srcImg, const cv::Mat &smallImg, const int tx, const int ty);
};