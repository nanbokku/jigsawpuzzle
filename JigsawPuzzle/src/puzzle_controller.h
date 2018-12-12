#pragma once

#include "puzzle_view.h"
#include "puzzle_model.h"

#include "imgui.h"

[event_receiver(native)]
class PuzzleController
{
public:
	PuzzleController(int argc, char** argv, PuzzleModel* model, PuzzleView* view);
	PuzzleController(const PuzzleController& controller);
	~PuzzleController();
	PuzzleController& operator=(const PuzzleController& controller);

	void onInitialized();
	void onExited();
	void onDropped(const void* mouse_pos, const void* frame_pos, int label);
	void onFileOpened(const char* name);

private:
	void hookEvents(PuzzleView* p);
	void unhookEvents(PuzzleView* p);

	PuzzleModel* model_ = NULL;
	PuzzleView* view_ = NULL;
};