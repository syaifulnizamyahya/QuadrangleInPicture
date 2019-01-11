#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	// setup background image
	String backgroundFilename("background.jpg"); 
	Mat background = imread(backgroundFilename, CV_LOAD_IMAGE_COLOR);
	imshow("background", background);

	// setup video capture
	VideoCapture cap("incredibles-2-trailer-1_h480p.mov");
	if (!cap.isOpened())
	{
		cout << "Error opening";
		return -1;
	}

	while (true)
	{
		// the video we want to put in background
		Mat video;
		cap >> video;
		if (video.empty())
		{
			break;
		}
		imshow("Video", video);

		// video source
		vector<Point> left_image;      
		left_image.push_back(Point(0, 0));
		left_image.push_back(Point(0, video.rows));
		left_image.push_back(Point(video.cols, video.rows));
		left_image.push_back(Point(video.cols, 0));

		// corresponding video coordinate destination
		vector<Point> right_image;
		right_image.push_back(Point(100, 100));
		right_image.push_back(Point(200, 500));
		right_image.push_back(Point(900, 650));
		right_image.push_back(Point(700, 200));

		// once we get 4 corresponding points in both images calculate homography matrix
		Mat H = findHomography(left_image, right_image, 0);
		Mat warpedImage;
		warpPerspective(video, warpedImage, H, background.size());
		imshow("warpedImage", warpedImage);

		// create a mask at background
		int num = 4;
		const Point *pt4 = &(right_image[0]);
		fillPoly(background, &pt4, &num, 1, Scalar(0, 0, 0), 8);
		imshow("background masked", background);

		// sum masked background and warpedImage
		// black pixel will contribute nothing to the final image
		// this will leaves only colored pixel in the final image
		Mat output = background + warpedImage;
		namedWindow("output", WINDOW_AUTOSIZE);
		imshow("output", output);

		char c = (char)waitKey(1);
		if (c == 27)
			break;
	}

	cap.release();

	destroyAllWindows();

	return 0;
}