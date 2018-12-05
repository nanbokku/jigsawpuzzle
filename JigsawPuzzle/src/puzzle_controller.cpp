#include "puzzle_controller.h"
#include "CVUtils.h"

PuzzleController::PuzzleController(PuzzleModel* model, PuzzleView* view)
{
	model_ = model;

	hookEvents(view);
}

PuzzleController::~PuzzleController()
{
	delete model_;
}

void PuzzleController::hookEvents(PuzzleView* p)
{
	__hook(&PuzzleView::onDropped, p, &PuzzleController::onDropped);
	__hook(&PuzzleView::onFileOpened, p, &PuzzleController::onFileOpened);
}

void PuzzleController::onDropped(const ImVec2& pos, const ImVec2& dest, const int label) {
	model_->put(label);
}

void PuzzleController::onFileOpened(const char* name) 
{
}