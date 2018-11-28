#include "PieceCreater.h"

using namespace cv;
using namespace std;

vector<Piece> PieceCreater::create(char* img_path)
{
	Mat frame = imread("media/jigsaw_small.png", IMREAD_GRAYSCALE);
	Mat src = imread(img_path, IMREAD_COLOR);

	resize(src, src, Size(frame.cols, frame.rows));

	Mat labelImg(frame.size(), CV_32S);
	Mat stats, centroids;

	// divide image into some pieces
	int label_count = connectedComponentsWithStats(frame, labelImg, stats, centroids, 8);

	vector<Mat> piece_imgs;
	vector<Piece> pieces;
	for (int label = 0; label < label_count; ++label) {
		int* params1 = stats.ptr<int>(label);
		int left = params1[ConnectedComponentsTypes::CC_STAT_LEFT];
		int top = params1[ConnectedComponentsTypes::CC_STAT_TOP];
		int width = params1[ConnectedComponentsTypes::CC_STAT_WIDTH];
		int height = params1[ConnectedComponentsTypes::CC_STAT_HEIGHT];
		double* params2 = centroids.ptr<double>(label);
		int x = static_cast<int>(params2[0]);
		int y = static_cast<int>(params2[1]);

		// mat for pieces initialize
		piece_imgs.push_back(Mat(Size(width, height), CV_8UC4, Scalar(0, 0, 0, 0)));

		Piece piece(Point(left, top), Point(x,y), Size(width, height));
		pieces.push_back(piece);
	}


	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			int label = labelImg.at<int>(y, x);
			Point position = pieces[label].position();
			Vec3b color = src.at<Vec3b>(y, x);
			Vec4b& pixel = piece_imgs[label].at<Vec4b>(y - position.y, x - position.x);

			// set 255 in alpha channel
			pixel[0] = color[0];
			pixel[1] = color[1];
			pixel[2] = color[2];
			pixel[3] = 255;
		}
	}

	for (int label = 0; label < label_count; label++) {
		if (label == 1) continue; // exclude frame
		pieces[label].piece(piece_imgs[label]);
	}

	return pieces_ = vector<Piece>(pieces);
}

void PieceCreater::write(char* path)
{
	int index = 0;
	for (auto piece : pieces_) {
		auto name = string(path) + "/piece" + to_string(index++) + ".raw";
		Mat mat = piece.piece();

		piece.path(name);

		ofstream stream(name.c_str(), ios::out | ios::binary);
		stream.write((char*)mat.data, mat.total() * mat.elemSize());
		stream.close();
		//imwrite(name, piece.piece());
	}
}