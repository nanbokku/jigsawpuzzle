#pragma once

#include "piece.h"

#include <opencv2/opencv.hpp>

class PieceCreater
{
public:
	PieceCreater();
	~PieceCreater();

	std::vector<Piece> create(const char* frame_path, const char* img_path);
	void write(char* path);

private:
	std::vector<Piece> pieces_;
};