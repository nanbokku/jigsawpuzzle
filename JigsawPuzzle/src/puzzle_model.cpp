#include "puzzle_model.h"

#include <random>
#include <algorithm>

#include <opencv2/opencv.hpp>

#include "cv_utils.h"
#include "gl_utils.h"

PuzzleModel::PuzzleModel(const char* filePath)
{
	this->filename(filePath);
}

PuzzleModel::PuzzleModel(const PuzzleModel& model) : Subject(model)
{
	this->filename(model.filename_);
}

PuzzleModel::~PuzzleModel()
{
	delete filename_;

	std::vector<Piece>().swap(pieces_);
	std::vector<GLuint>().swap(texIds_);
}

PuzzleModel& PuzzleModel::operator=(const PuzzleModel& model)
{
	if (this != &model) {
		Subject::operator=(model);
		this->filename(model.filename_);
	}

	return *this;
}

void PuzzleModel::initialize()
{
	auto pieces = creater_.create(filename());

	// shuffle puzzle pieces exclude first piece
	mt19937 rand;
	shuffle(pieces.begin() + 1, pieces.end(), rand);

	pieces_ = vector<Piece>(pieces);

	// bind textures
	texIds_ = std::vector<GLuint>(pieceNum());
	for (int i = 0; i < textureNum(); i++) {
		GLUtils::convertMatToGL(piece(i).mat(), texId_ptr(i));
	}
}

void PuzzleModel::initialize(const char* filePath)
{
	filename(filePath);

	// initialize puzzle game
	initialize();
}

void PuzzleModel::fit(int label)
{
	Piece p = piece(label);
	cv::Mat new_frame = CVUtils::PinP(piece(0).mat(), p.mat(), p.position().x, p.position().y);

	// update frame
	piece_ptr(0)->mat(new_frame);
	GLUtils::overwriteTexture(piece(0).mat(), texId_ptr(0));

	// remove a piece
	pieces_.erase(pieces_.begin() + label);
	texIds_.erase(texIds_.begin() + label);

	// notify observers
	notify();
}