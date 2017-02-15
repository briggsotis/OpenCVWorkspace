// BackgroundVideo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>


using namespace cv;
using namespace std;


int main()
{
	VideoCapture cap("../data/dog.mp4"); // open the default camera
	if (!cap.isOpened())
	{
		std::cout << "!!! Failed to open file: ./data/dog.mp4";
		return -1;
	}

	// For Loop declarations
	Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
	pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach
	Mat frame;

	namedWindow("Red Motion Detection", WINDOW_AUTOSIZE);

	for (;;)
	{
		if (!cap.read(frame))
			break;

		// Add the frame to the model generate a mask based on it
		pMOG2->apply(frame, fgMaskMOG2);

		// Erosion settings
		int erosion_size = 2;
		Mat element = getStructuringElement(MORPH_ERODE,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			Point(erosion_size, erosion_size));

		// Apply erosion then dilation on the image (opening)
		erode(fgMaskMOG2, fgMaskMOG2, element);
		dilate(fgMaskMOG2, fgMaskMOG2,element);

		// Set all masked pixels to be pure red
		Mat masked;
		frame.setTo(Scalar(0, 0, 255), fgMaskMOG2);

		imshow("Red Motion Detection", frame);
		char key = cvWaitKey(10);
		if (key == 27) // ESC
			break;
	}

	return 0;
}

