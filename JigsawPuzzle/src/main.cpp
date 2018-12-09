#include "puzzle_model.h"
#include "puzzle_controller.h"
#include "puzzle_view.h"

using namespace cv;
using namespace std;

vector<Piece> pieces;
char* filepath = "media/rabbit.jpg";


int main(int argc, char* argv[])
{
	PuzzleModel model = PuzzleModel(filepath);
	PuzzleView view = PuzzleView(&model);
	PuzzleController controller = PuzzleController(argc, argv, &model, &view);

	view.draw();
}