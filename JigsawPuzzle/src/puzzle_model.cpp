#include "puzzle_model.h"
#include "CVUtils.h"
#include "GLUtils.h"

PuzzleModel::PuzzleModel(char* filename, const std::vector<Piece>& pieces)
{
	filename_ = filename;
	pieces_ = vector<Piece>(pieces);

	// bind textures
	texIds_ = std::vector<GLuint>(pieceNum());
	for(int i=0; i<texIds_.size(); i++) {
		GLUtils::convertMatToGL(piece(i).mat(), texId_ptr(i));
	}
}

PuzzleModel::~PuzzleModel()
{
	std::vector<Piece>().swap(pieces_);
	std::vector<GLuint>().swap(texIds_);
}

void PuzzleModel::put(int label)
{
	Piece p = piece(label);
	cv::Mat new_frame = CVUtils::PinP(piece(0).mat(), p.mat(), p.position().x, p.position().y);

	// update frame
	piece(0).mat(new_frame);
	GLUtils::overwriteTexture(piece(0).mat(), texId_ptr(0));

	// remove a piece
	pieces_.erase(pieces_.begin() + label);
	texIds_.erase(texIds_.begin() + label);

	// notify observers
	notify();
}