#include "PieceCreater.h"
#include "GLUtils.h"
#include "CVUtils.h"

#include <opencv2/opencv.hpp>
#include <GL/glut.h>
#include <nfd.h>
#include <random>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_freeglut.h"
#include "imgui_impl_opengl2.h"

using namespace cv;
using namespace std;

vector<Piece> pieces;
vector<GLuint> texIDs;
char* filepath = "media/rabbit.jpg";

void convertTo(ImVec2& vec2, const Point& point)
{
	vec2 = ImVec2(point.x, point.y);
}

double euclideanDist(const ImVec2& vec1, const ImVec2& vec2)
{
	auto x = vec1.x - vec2.x;
	auto y = vec1.y - vec2.y;

	return sqrt(x * x + y * y);
}

void gui()
{
	// 初めはファイルの選択
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Game")) {
			if (ImGui::MenuItem("New Game")) {
			}

			if (ImGui::MenuItem("Exit Game")) {
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options")) {
			if (ImGui::MenuItem("Open file", "CTRL+O")) {
				nfdchar_t *outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("png,jpg", NULL, &outPath);

				if (result == NFD_OKAY) {
					filepath = outPath;
					delete outPath;
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("How to")) {
				// how to jigsaw puzzle.
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	// パズル開始
	int move_from = -1;
	ImVec2 frame_pos = ImVec2(0, 20);
	frame_pos = ImVec2(frame_pos.x + ImGui::GetWindowContentRegionMin().x, frame_pos.y + ImGui::GetWindowContentRegionMin().y);
	const int cols = 3;

	ImGui::SetNextWindowPos(ImVec2(805 + ImGui::GetWindowContentRegionMin().x, 20));
	ImGui::SetNextWindowSize(ImVec2(350, 600));
	ImGui::Begin("Piece Box");
	for (int i = 1; i < texIDs.size(); i++) {
		ImGui::ImageButton((void*)(intptr_t)texIDs[i], ImVec2(pieces[i].mat().cols, pieces[i].mat().rows));

		if (i % cols != 0) ImGui::SameLine();

		ImGuiDragDropFlags src_flags = 0;
		src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
		src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

		if (ImGui::BeginDragDropSource(src_flags)) {
			if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip)) {
				ImGui::Text("Moving");
				ImGui::Image((void*)(intptr_t)texIDs[i], ImVec2(pieces[i].mat().cols, pieces[i].mat().rows));
			}
			ImGui::SetDragDropPayload("DND_LABEL", &i, sizeof(int));
			ImGui::EndDragDropSource();
		}
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(805 + ImGui::GetWindowContentRegionMin().x, 600));
	ImGui::Begin("Puzzle Frame");
	ImGui::Image((void*)(intptr_t)texIDs[0], ImVec2(pieces[0].mat().cols, pieces[0].mat().rows));

	if (ImGui::BeginDragDropTarget()) {
		ImGuiDragDropFlags target_flags = 0;
		//target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
		target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_LABEL", target_flags)) {
			int label = *(const int*)payload->Data;
			ImVec2 pos = ImGui::GetMousePos();
			ImVec2 dest;

			convertTo(dest, pieces[label].centroid());
			dest = ImVec2(dest.x + frame_pos.x, dest.y + frame_pos.y);

			// drop near destination
			if (euclideanDist(pos, dest) < 50.0) {
				move_from = label;
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (move_from != -1) {
		Piece piece = pieces[move_from];
		Mat new_frame = CVUtils::PinP(pieces[0].mat(), piece.mat(), piece.position().x, piece.position().y);
		pieces[0].mat(new_frame);
		GLUtils::overwriteTexture(pieces[0].mat(), &texIDs[0]);

		pieces.erase(pieces.begin() + move_from);
		texIDs.erase(texIDs.begin() + move_from);
	}
	ImGui::End();
}

void drawString(const char* str, const float charSize, const int lineWidth, const float x, const float y)
{
	glPushMatrix();
	glTranslatef(x, y, -1.0f);

	glPushMatrix();
	glPushAttrib(GL_LINE_BIT);
	glScaled(0.001 * charSize, 0.001 * charSize, 0.01);
	glLineWidth(lineWidth);
	while (*str) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
		++str;
	}
	glPopAttrib();
	glPopMatrix();

	glPopMatrix();
}

void display()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();

	gui();

	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);
	drawString("JIGSAW PUZZLE", 1.5, 10, -0.75f, 0.0f);

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();

	glFlush();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

void init(vector<Piece> pieces)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// bind texture
	texIDs = vector<GLuint>(pieces.size());
	for (int i = 0; i < texIDs.size(); i++) {
		GLUtils::convertMatToGL(pieces[i].mat(), &texIDs[i]);
	}
}

void setStyle()
{
	ImGui::StyleColorsDark();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 0);
	style->Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 0);
}

#include "puzzle_model.h"
#include "puzzle_controller.h"
#include "puzzle_view.h"

int main(int argc, char* argv[])
{
	PieceCreater creater;

	pieces = creater.create(filepath);
	creater.write("media/pieces");

	mt19937 rand;
	shuffle(pieces.begin() + 1, pieces.end(), rand);

	PuzzleModel model = PuzzleModel(filepath, pieces);
	PuzzleView view = PuzzleView(argc, argv, &model);
	PuzzleController controller = PuzzleController(&model, &view);

	view.draw();
}

//int main(int argc, char* argv[])
//{
//	PieceCreater creater;
//	//vector<Piece> pieces;
//
//	pieces = creater.create(filepath);
//	creater.write("media/pieces");
//
//	// shuffle pieces exclude first element
//	mt19937 rand;
//	shuffle(pieces.begin() + 1, pieces.end(), rand);
//
//	glutInit(&argc, argv);
//	glutInitWindowSize(1200, 700);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
//	glutCreateWindow(argv[0]);
//
//	glutDisplayFunc(display);
//	init(pieces);
//	//glutReshapeFunc(resize);
//
//	ImGui::CreateContext();
//
//	ImGui_ImplFreeGLUT_Init();
//	ImGui_ImplFreeGLUT_InstallFuncs();
//	ImGui_ImplOpenGL2_Init();
//
//	setStyle();
//
//	glutMainLoop();
//
//	ImGui_ImplOpenGL2_Shutdown();
//	ImGui_ImplFreeGLUT_Shutdown();
//	ImGui::DestroyContext();
//
//	return 0;
//}