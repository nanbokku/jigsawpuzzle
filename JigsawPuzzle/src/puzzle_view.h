#pragma once

#include "Observer.h"
#include "Piece.h"
#include "puzzle_model.h"

#include <GL/glut.h>
#include <vector>

[event_source(native)]
class PuzzleView : public Observer
{
public:
	PuzzleView(int argc, char** argv, PuzzleModel* model);
	~PuzzleView();
	void draw();
	void update() override;

	__event void onDropped(const void* mouse_pos, const void* frame_pos, int label);
	__event void onFileOpened(const char* name);

private:
	void drawString(const char* str, const float charSize, const int lineWidth, const float x, const float y);
	void gui();
	void display();
	static void display_callback();
	void setStyle();

	PuzzleModel* model_;

	static PuzzleView* instance;	// for callback
};