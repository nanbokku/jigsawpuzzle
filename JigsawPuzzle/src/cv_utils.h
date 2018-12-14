#pragma once
#include <opencv2/opencv.hpp>

#include "imgui.h"

class CVUtils
{
public:
	static void convertTo(ImVec2& vec2, const cv::Point& point);
	static double euclideanDist(const ImVec2& vec1, const ImVec2& vec2);
	static cv::Mat PinP(const cv::Mat &srcImg, const cv::Mat &smallImg, const int tx, const int ty);
	static void DrawTransPinP(cv::Mat &img_dst, const cv::Mat transImg, const cv::Mat baseImg, std::vector<cv::Point2f> tgtPt);
};