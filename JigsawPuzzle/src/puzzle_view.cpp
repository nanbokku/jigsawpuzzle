#include "puzzle_view.h"
#include "CVUtils.h"
#include "Piece.h"

#include <nfd.h>

#include "imgui.h"
#include "imgui_impl_freeglut.h"
#include "imgui_impl_opengl2.h"


PuzzleView::PuzzleView(int argc, char** argv, PuzzleModel* model)
{
	instance_ = this;
	model_ = model;

	glutInit(&argc, argv);
	glutInitWindowSize(1200, 700);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutCreateWindow(argv[0]);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutDisplayFunc(display_callback);
	//glutReshapeFunc(resize);

	ImGui::CreateContext();

	ImGui_ImplFreeGLUT_Init();
	ImGui_ImplFreeGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	setStyle();
}

PuzzleView::~PuzzleView()
{
	delete instance_;
	delete model_;
}

void PuzzleView::draw()
{
	glutMainLoop();

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();
}

void PuzzleView::drawString(const char* str, const float charSize, const int lineWidth, const float x, const float y)
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

void PuzzleView::display()
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

void PuzzleView::gui()
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
					__raise onFileOpened(outPath);
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
	for (int i = 1; i < model_->textureNum(); i++) {
		ImGui::ImageButton((void*)(intptr_t)model_->texId_ptr(i), ImVec2(model_->piece(i).mat().cols, model_->piece(i).mat().rows));

		if (i % cols != 0) ImGui::SameLine();

		ImGuiDragDropFlags src_flags = 0;
		src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
		src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

		if (ImGui::BeginDragDropSource(src_flags)) {
			if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip)) {
				ImGui::Text("Moving");
				ImGui::Image((void*)(intptr_t)model_->texId_ptr(i), ImVec2(model_->piece(i).mat().cols, model_->piece(i).mat().rows));
			}
			ImGui::SetDragDropPayload("DND_LABEL", &i, sizeof(int));
			ImGui::EndDragDropSource();
		}
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(805 + ImGui::GetWindowContentRegionMin().x, 600));
	ImGui::Begin("Puzzle Frame");
	ImGui::Image((void*)(intptr_t)model_->texId_ptr(0), ImVec2(model_->piece(0).mat().cols, model_->piece(0).mat().rows));

	if (ImGui::BeginDragDropTarget()) {
		ImGuiDragDropFlags target_flags = 0;
		//target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
		target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_LABEL", target_flags)) {
			int label = *(const int*)payload->Data;
			ImVec2 pos = ImGui::GetMousePos();
			ImVec2 dest;

			CVUtils::convertTo(dest, model_->piece(label).centroid());
			dest = ImVec2(dest.x + frame_pos.x, dest.y + frame_pos.y);

			double distance = CVUtils::euclideanDist(pos, dest);
			if (distance < 50.0) {
				// drop near destination
				__raise onDropped(pos, dest, label);
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::End();
}

void PuzzleView::setStyle()
{
	ImGui::StyleColorsDark();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 0);
	style->Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 0);
}

void PuzzleView::display_callback()
{
	if (instance_ == nullptr) return;

	instance_->display();
}

void PuzzleView::update()
{
	printf("match\n");
}