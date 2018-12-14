#include "cv_utils.h"

void CVUtils::convertTo(ImVec2& vec2, const cv::Point& point)
{
	vec2 = ImVec2(point.x, point.y);
}

double CVUtils::euclideanDist(const ImVec2& vec1, const ImVec2& vec2)
{
	auto x = vec1.x - vec2.x;
	auto y = vec1.y - vec2.y;

	return sqrt(x * x + y * y);
}

void setAlpha(cv::Mat& srcMat)
{
	int width = srcMat.cols;
	int height = srcMat.rows;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cv::Vec4b &p = srcMat.at<cv::Vec4b>(i, j);
			// a値 = 0のとき 色反転
			// それ以外はすべて0(ギザギザを解消)
			if (p[3] == 255) {
				//p[0] = 255 - p[0];
				//p[1] = 255 - p[1];
				//p[2] = 255 - p[2];
				//p[3] = 255 - p[3];
			}
			else {
				p[0] = 0;
				p[1] = 0;
				p[2] = 0;
				p[3] = 0;
			}
		}
	}
}

cv::Mat CVUtils::PinP(const cv::Mat& src, const cv::Mat& overlay, const int tx, const int ty)
{
	cv::Mat img = src.clone();
	cv::Mat mask = overlay.clone();
	setAlpha(mask);
	
	cv::Mat roi(img, cv::Rect(tx, ty, overlay.cols, overlay.rows));
	overlay.copyTo(roi, mask);

	return img;
}