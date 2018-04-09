// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global Variables
const int intensity_slider_max = 255;

int main()
{
	VideoCapture cap;
	Mat kernel(10, 10, CV_8U, Scalar(1));
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.

	int intensity_slider = 150;

	namedWindow("Window", WINDOW_AUTOSIZE);

	createTrackbar("Intensity", "Window", &intensity_slider, intensity_slider_max, nullptr);

	if (!cap.open(1))
		return 0;
	for (;;)
	{
		Mat frame, grayscale, canny_output;
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;

		cap >> frame;
		if (frame.empty()) break; // end of video stream

		// Convert to grayscale
		cvtColor(frame, grayscale, CV_BGR2GRAY);
		
		// Remove noise
		morphologyEx(grayscale, grayscale, MORPH_OPEN, kernel);

		// Change grayscale intensity
		threshold(grayscale, grayscale, intensity_slider, intensity_slider_max, 3);

		// Find contours
		Canny(grayscale, canny_output, intensity_slider, intensity_slider * 2, 3);
		findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		// Draw contours
		Scalar color = Scalar(0, 0, 255);
		for (int i = 0; i < contours.size(); i++) {
			drawContours(frame, contours, i, color, 2, 8, hierarchy);
		}

		imshow("Window", frame);

		if (waitKey(10) == 27) break; // stop capturing by pressing ESC 
	}
	// the camera will be closed automatically upon exit
	//cap.close();
	return 0;
}

