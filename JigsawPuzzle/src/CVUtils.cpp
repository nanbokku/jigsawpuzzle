#include "CVUtils.h"

cv::Mat CVUtils::PinP(const cv::Mat & srcImg, const cv::Mat & smallImg, const int tx, const int ty)
{
	//背景画像の作成
	cv::Mat dstImg;
	srcImg.copyTo(dstImg);

	//前景画像の変形行列
	cv::Mat mat = (cv::Mat_<double>(2, 3) << 1.0, 0.0, tx, 0.0, 1.0, ty);

	//アフィン変換の実行
	cv::warpAffine(smallImg, dstImg, mat, dstImg.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);

	return dstImg;
}
