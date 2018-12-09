#pragma once

#include "Piece.h"
#include <opencv2/opencv.hpp>

class PieceCreater
{
public:
	PieceCreater();
	~PieceCreater();

	std::vector<Piece> create(char* img_path);
	void write(char* path);

private:
	std::vector<Piece> pieces_;
};