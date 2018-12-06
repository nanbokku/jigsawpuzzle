#include "puzzle_controller.h"
#include "CVUtils.h"

#include "imgui.h"

PuzzleController::PuzzleController(int argc, char** argv, PuzzleModel* model, PuzzleView* view)
{
	model_ = model;
	view_ = view;

	view_->initialize(argc, argv);
	model_->initialize();

	hookEvents(view_);
}

PuzzleController::~PuzzleController()
{
	unhookEvents(view_);

	delete model_;
	delete view_;
}

void PuzzleController::hookEvents(PuzzleView* p)
{
	__hook(&PuzzleView::onDropped, p, &PuzzleController::onDropped);
	__hook(&PuzzleView::onFileOpened, p, &PuzzleController::onFileOpened);
}

void PuzzleController::unhookEvents(PuzzleView* p) {
	__unhook(&PuzzleView::onDropped, p, &PuzzleController::onDropped);
	__unhook(&PuzzleView::onFileOpened, p, &PuzzleController::onFileOpened);
}

void PuzzleController::onDropped(const void* mouse_pos, const void* frame_pos, int label)
{
	ImVec2 mouse = *(ImVec2*)mouse_pos;
	ImVec2 frame = *(ImVec2*)frame_pos;
	ImVec2 dest;

	CVUtils::convertTo(dest, model_->piece(label).centroid());
	dest = ImVec2(dest.x + frame.x, dest.y + frame.y);
	double distance = CVUtils::euclideanDist(mouse, dest);

	if (distance >= 50.0) return;

	model_->put(label);
}

void PuzzleController::onFileOpened(const char* name)
{
}