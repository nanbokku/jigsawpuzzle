#include "CVUtils.h"

cv::Mat CVUtils::PinP(const cv::Mat & srcImg, const cv::Mat & smallImg, const int tx, const int ty)
{
	//�w�i�摜�̍쐬
	cv::Mat dstImg;
	srcImg.copyTo(dstImg);

	//�O�i�摜�̕ό`�s��
	cv::Mat mat = (cv::Mat_<double>(2, 3) << 1.0, 0.0, tx, 0.0, 1.0, ty);

	//�A�t�B���ϊ��̎��s
	cv::warpAffine(smallImg, dstImg, mat, dstImg.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);

	return dstImg;
}
