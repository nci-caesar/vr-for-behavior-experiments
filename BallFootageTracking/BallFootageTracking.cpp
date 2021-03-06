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

	spdlog::filename_t log_filename = spdlog::filename_t(argv[2]);

	cv::String input_filename = cv::String(argv[1]);
	cv::Mat frame, track_data; 

	Uint64 frame_count = 0;
	Uint64 time_count = 0;

	BallTracking ball_tracker(true, RotModel::MODE_CALIBRATION);
	try {
		/*
		Set up logging
		*/
		auto file_logger = spdlog::rotating_logger_mt("calibration", log_filename, 1048576 * 5, 1000);
		file_logger->info("Calibration tracking: [amplitude_rad, amplitude_tan, offset_tan, phase]");

		/*
		Video capture and processing
		*/
		cv::VideoCapture cap(
			input_filename);

		if (!cap.isOpened()) {
			throw(std::exception("Error opening video stream or file"));
		}
		while (1) {
			cap >> frame;
			if (frame.empty())
				break;

			if (frame.channels() > 1) {
				cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
			}

			auto start = std::chrono::system_clock::now();

			track_data = ball_tracker.update(frame);

			cv::imshow("Frame", frame);
			cv::imshow("Debug", ball_tracker.debugPlot);


			file_logger->info("{} {} {} {}", track_data.at<float>(0), track_data.at<float>(1), track_data.at<float>(2), track_data.at<float>(3));

			auto end = std::chrono::system_clock::now();
			auto elapsed =
				std::chrono::duration_cast<std::chrono::microseconds>(end - start);

			frame_count += 1;
			time_count += elapsed.count();


			char c = (char)cv::waitKey(1);
			if (c == 27) break;
		}
		cap.release();
		cv::destroyAllWindows();

		std::cout << "Average processing time, us: " << time_count/frame_count << '\n';
	}
	catch (const std::exception &exc)
	{
		std::cerr << exc.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception!" << std::endl;
	}
	std::cerr << std::endl << "Press Enter to exit." << std::endl;
	std::cin.get();

	return 0;
}
