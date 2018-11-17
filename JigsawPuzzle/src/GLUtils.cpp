#include "GLUtils.h"
#include <iostream>

void GLUtils::ConvertMatToGL(const cv::Mat& src, GLuint* texID)
{
	if (src.empty() == true)
		return;

	glDeleteTextures(1, texID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, texID);
	glBindTexture(GL_TEXTURE_2D, *texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (src.channels() == 4) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			src.cols, src.rows,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			src.ptr<unsigned char>()
		);

	}
	else if (src.channels() == 3) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			src.cols, src.rows,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			src.ptr<unsigned char>()
		);
	}
	else if (src.channels() == 1) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_LUMINANCE,
			src.cols, src.rows,
			0,
			GL_LUMINANCE,
			GL_UNSIGNED_BYTE,
			src.ptr<unsigned char>()
		);
	}
	else {
		cerr << "other channel !" << endl;
	}
}