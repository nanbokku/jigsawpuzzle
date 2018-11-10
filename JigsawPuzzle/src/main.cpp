#include "PieceCreater.h"
#include <opencv2/opencv.hpp>
#include <GL/glut.h>

using namespace cv;
using namespace std;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char* argv[])
{
	PieceCreater creater;
	vector<Piece> pieces;

	//pieces = creater.create("media/rabbit.jpg");
	//creater.write("media");

	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	init();
	glutReshapeFunc(resize);
	glutMainLoop();

	return 0;
}