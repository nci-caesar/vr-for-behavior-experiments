// BallFootageTracking.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <BallTracking.h>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		std::cout << "Input for tracking needed as command line arguments. Usage:" << std::endl;
		std::cout << "BallFootageTracking inpt_footage_file output_log_file" << std::endl;
	}
	else {
		std::cout << "Processing " << argv[1] << std::endl; 
		std::cout << "Writing output to " << argv[2] << std::endl;
	}

	cv::String input_filename = cv::String(argv[1]);
	cv::Mat frame;
	try {
		cv::VideoCapture cap(
			input_filename);

		if (!cap.isOpened()) {
			throw(std::exception("Error opening video stream or file"));
		}

		while (1) {
			cap >> frame;
			if (frame.empty())
				break;

			cv::imshow("Frame", frame);

			char c = (char)cv::waitKey(25);
			if (c == 27) break;
		}
		cap.release();
		cv::destroyAllWindows();
	}
	catch (const std::exception &exc)
	{
		// catch anything thrown within try block that derives from std::exception
		std::cerr << exc.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception!" << std::endl;
	}


	std::cerr << std::endl << "Press Enter to exit." << std::endl;
	std::cin.get();

	return 0;
}
