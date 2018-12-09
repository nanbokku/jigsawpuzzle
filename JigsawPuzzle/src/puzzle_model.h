#pragma once

#include "PieceCreater.h"
#include "Piece.h"
#include "Subject.h"

#include <opencv2/opencv.hpp>
#include <GL/glut.h>

class PuzzleModel : public Subject
{
public:
	PuzzleModel(const char* filename);
	~PuzzleModel();
	void initialize();
	void initialize(const char* filePath);

	bool isPlaying() { return is_playing_; }
	void isPlaying(bool playing) { is_playing_ = playing; }

	char* filename() { return filename_; }
	void filename(const char* n)
	{
		filename_ = (char*)malloc(sizeof(char) * sizeof(n));
		strncpy(filename_, n, strlen(n) + 1);
	}

	cv::Mat frame() { return frame_; }
	void frame(const cv::Mat& m) { frame_ = m; }

	int pieceNum() { return pieces_.size(); }
	Piece piece(int index) { return pieces_[index]; }
	Piece* piece_ptr(int index) { return &pieces_[index]; }
	void push(const Piece& p) { pieces_.push_back(p); }

	int textureNum() { return texIds_.size(); }
	GLuint texId(int index) { return texIds_[index]; }
	GLuint* texId_ptr(int index) { return &(texIds_[index]); }
	void push(const GLuint& id) { texIds_.push_back(id); }

	void fit(int label);


private:
	bool is_playing_ = false;
	char* filename_;
	cv::Mat frame_;
	std::vector<Piece> pieces_;
	std::vector<GLuint> texIds_;

	PieceCreater creater_;
	//GameState state_;
};