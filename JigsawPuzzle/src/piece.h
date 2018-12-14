#pragma once

#include <opencv2/opencv.hpp>

class Piece
{
public:
	Piece(const cv::Mat& mat, const cv::Point& position, const cv::Point& centroid, const cv::Size& size, std::string filepath)
	{
		mat_ = mat;
		position_ = position;
		centroid_ = centroid;
		size_ = size;
		filepath_ = filepath;
	}
	Piece(const cv::Point& position, const cv::Point& centroid, const cv::Size& size)
	{
		position_ = position;
		centroid_ = centroid;
		size_ = size;
	}

	cv::Mat mat() { return mat_; }
	void mat(cv::Mat p) { mat_ = p; }
	cv::Point position() { return position_; }
	void position(cv::Point p) { position_ = p; }
	cv::Point centroid() { return centroid_; };
	void centroid(cv::Point p) { centroid_ = p; }
	cv::Size size() { return size_; }
	void size(cv::Size s) { size_ = s; }
	std::string path() { return filepath_; }
	void path(std::string p) { filepath_ = p; }

private:
	cv::Mat mat_;
	cv::Point position_, centroid_;
	cv::Size size_;
	std::string filepath_;
};