#include "PieceCreater.h"
#include "GLUtils.h"
#include "CVUtils.h"
#include <opencv2/opencv.hpp>
#include <GL/glut.h>
#include "imgui.h"
#include "imgui_impl_freeglut.h"
#include "imgui_impl_opengl2.h"

using namespace cv;
using namespace std;

//bool show_demo_window = false;
//bool show_another_window = true;
//static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
vector<Piece> pieces;
vector<GLuint> texIDs;

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
	int move_from = -1;
	ImVec2 frame_pos = ImVec2(0, 0);
	frame_pos = ImVec2(frame_pos.x + ImGui::GetWindowContentRegionMin().x, frame_pos.y + ImGui::GetWindowContentRegionMin().y);

	ImGui::SetNextWindowPos(ImVec2(805 + ImGui::GetWindowContentRegionMin().x, 0));
	ImGui::SetNextWindowSize(ImVec2(300, 600));
	ImGui::Begin("Piece Box");
	for (int i = 2; i < texIDs.size(); i++) {
		ImGui::ImageButton((void*)(intptr_t)texIDs[i], ImVec2(pieces[i].piece().cols, pieces[i].piece().rows));
		//ImGui::Image((void*)(intptr_t)texIDs[i], ImVec2(pieces[i].piece().cols, pieces[i].piece().rows));

		ImGuiDragDropFlags src_flags = 0;
		src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
		src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

		if (ImGui::BeginDragDropSource(src_flags)) {
			if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
				ImGui::Text("Moving");
			ImGui::SetDragDropPayload("DND_LABEL", &i, sizeof(int));
			ImGui::EndDragDropSource();
		}
	}

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(805 + ImGui::GetWindowContentRegionMin().x, 600));
	ImGui::Begin("Puzzle Frame");
	ImGui::ImageButton((void*)(intptr_t)texIDs[0], ImVec2(pieces[0].piece().cols, pieces[0].piece().rows));
	//ImGui::Image((void*)(intptr_t)texIDs[0], ImVec2(pieces[0].piece().cols, pieces[0].piece().rows));

	if (ImGui::BeginDragDropTarget()) {
		ImGuiDragDropFlags target_flags = 0;
		target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
		target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_LABEL", target_flags)) {
			int label = *(const int*)payload->Data;
			ImVec2 pos = ImGui::GetMousePos();
			ImVec2 dest;

			convertTo(dest, pieces[label].position());
			dest = ImVec2(dest.x + frame_pos.x, dest.y + frame_pos.y);
			
			cout << pos.x << " " << pos.y << "  " << dest.x << " " << dest.y << endl;

			// drop near destination
			if (euclideanDist(pos, dest) < 100.0) {
				move_from = label;
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::End();

	if (move_from != -1) {
		//printf("[%05d] move %d->%d (copy %d..%d to %d..%d)\n", ImGui::GetFrameCount(), move_from, move_to, copy_src, copy_src + copy_count - 1, copy_dst, copy_dst + copy_count - 1);
		//ImGui::SetDragDropPayload("DND_LABEL", &move_to, sizeof(int)); // Update payload immediately so on the next frame if we move the mouse to an earlier item our index payload will be correct. This is odd and showcase how the DnD api isn't best presented in this example.

		Piece piece = pieces[move_from];
		Mat new_frame = CVUtils::PinP(pieces[0].piece(), piece.piece(), piece.position().x, piece.position().y);
		pieces[0].piece(new_frame);
		imshow("test", new_frame);
	}

	ImGui::End();

	//ImGui::Begin("Hello world.");
	////ImGui::Checkbox("Demo Window", &show_demo_window);
	//ImGui::Checkbox("Another Window", &show_another_window);

	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	//ImGui::ColorEdit3("clear color", (float*)&clear_color);

	//if (ImGui::Button("Button")) counter++;
	//ImGui::SameLine();
	//ImGui::Text("counter = %d", counter);

	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::End();

	//if (show_another_window) {
	//	ImGui::Begin("Another Window", &show_another_window);
	//	ImGui::Text("Hello from another window!");
	//	if (ImGui::Button("Close Me")) show_another_window = false;
	//	ImGui::End();
	//}
}

void display()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();

	gui();

	ImGui::Render();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

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

	texIDs = vector<GLuint>(pieces.size());
	for (int i = 0; i < texIDs.size(); i++) {
		GLUtils::ConvertMatToGL(pieces[i].piece(), &texIDs[i]);
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

int main(int argc, char* argv[])
{
	PieceCreater creater;
	//vector<Piece> pieces;

	pieces = creater.create("media/rabbit.jpg");
	creater.write("media/pieces");

	glutInitWindowSize(1000, 700);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
	init(pieces);
	//glutReshapeFunc(resize);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplFreeGLUT_Init();
	ImGui_ImplFreeGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	setStyle();

	glutMainLoop();

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}