#pragma once

#include "puzzle_view.h"
#include "puzzle_model.h"
#include <GL/glut.h>

[event_receiver(native)]
class PuzzleController
{
public:
	PuzzleController(PuzzleModel* model, PuzzleView* view);
	~PuzzleController();

	void onDropped(const ImVec2& pos, const ImVec2& dest, const int label);
	void onFileOpened(const char* name);

private:
	void hookEvents(PuzzleView* p);

	PuzzleModel* model_;
};