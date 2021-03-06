// VRApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BallTracking.h"
#include "ImageAcquisition.h"
#include "Environment3D.h"
#include "ConcurrentQueue.h"



int vr_thread_function(int* control_flags, TrackingQueue* tracking_queue) {
	/*
	Game engine thread
	*/
	Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
	Urho3D::SharedPtr<Environment3D> application(new Environment3D(context));
	
	const char* startup_script = "VirtualEnvironments\\Stripe.as";
	//const char* startup_script = "C:\\Users\\vishniakou\\source\\repos\\Caesar.VR\\CaesarVR\\Release\\Data\\VirtualEnvironments\\Stripe.as";
	application->Configure((char*)startup_script, control_flags, tracking_queue);
	int r = application->Run();
	*control_flags = 1;
	return r;
}


int tracking_thread(int* control_flags, TrackingQueue* tracking_queue) {
	boost::property_tree::ptree tracking_settings;
	
	while (!*control_flags) {

	}
}


int main()
{
	TrackingQueue q;
	int control_flags = 0;
	std::thread vr_thread(vr_thread_function, &control_flags, &q);


	//bool toggle_screen = false;
	while (!control_flags) {
		char k = cv::waitKey(1);
		if (k) {
			//control_flags |= 32;
		}
		if (k == 'q' || k==27)
			break;
	}

	control_flags |= 1;

	cv::destroyAllWindows();
	//image_aquisition_thread.join();
	//file_thread.join(); 
	vr_thread.join();



    return 0;
}

