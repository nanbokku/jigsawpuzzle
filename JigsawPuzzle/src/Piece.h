#pragma once

#include <opencv2/opencv.hpp>

class Piece
{
public:
	Piece(int label, const cv::Mat& piece, const cv::Point& position, const cv::Size& size, std::string filepath)
	{
		label_ = label;
		piece_ = piece;
		position_ = position;
		size_ = size;
		filepath_ = filepath;
	}
	Piece(int label, const cv::Point& position, const cv::Size& size)
	{
		label_ = label;
		position_ = position;
		size_ = size;
	}

	cv::Mat piece() { return piece_; }
	void piece(cv::Mat p) { piece_ = p; }
	cv::Point position() { return position_; }
	void position(cv::Point p) { position_ = p; }
	cv::Size size() { return size_; }
	void size(cv::Size s) { size_ = s; }
	std::string path() { return filepath_; }
	void path(std::string p) { filepath_ = p; }

private:
	int label_;
	cv::Mat piece_;
	cv::Point position_;
	cv::Size size_;
	std::string filepath_;
};