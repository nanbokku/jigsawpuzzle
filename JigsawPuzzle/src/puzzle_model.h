#pragma once

#include "Piece.h"
#include "Subject.h"

#include <opencv2/opencv.hpp>
#include <GL/glut.h>

class PuzzleModel : public Subject
{
public:
	PuzzleModel(char* filename, const std::vector<Piece>& pieces);
	~PuzzleModel();

	char* filename() { return filename_; }
	void filename(const char* n) { strcpy(filename_, n); }
	cv::Mat frame() { return frame_; }
	void frame(const cv::Mat& m) { frame_ = m; }
	int pieceNum() { return pieces_.size(); }
	Piece piece(int index) { return pieces_[index]; }
	void push(const Piece& p) { pieces_.push_back(p); }
	int textureNum() { return texIds_.size(); }
	GLuint texId(int index) { return texIds_[index]; }
	GLuint* texId_ptr(int index) { return &(texIds_[index]); }
	void push(const GLuint& id) { texIds_.push_back(id); }

	void put(int label);

private:
	char* filename_;
	cv::Mat frame_;
	std::vector<Piece> pieces_;
	std::vector<GLuint> texIds_;
	//GameState state_;
};