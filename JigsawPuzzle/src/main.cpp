#include "puzzle_model.h"
#include "puzzle_controller.h"
#include "puzzle_view.h"

int main(int argc, char* argv[])
{
	PuzzleModel model = PuzzleModel(argv[1], argv[2]);
	PuzzleView view = PuzzleView(&model);
	PuzzleController controller = PuzzleController(argc, argv, &model, &view);

	view.draw();
}