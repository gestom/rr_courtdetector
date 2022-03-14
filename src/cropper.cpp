#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <dynamic_reconfigure/server.h>
#include <rr_msgs/BeePosition.h>
#include <std_msgs/Int32.h>

using namespace cv;
using namespace std;

ros::Subscriber positionSub;
image_transport::Subscriber imageSub;
image_transport::Publisher imagePup;

Mat inputImage;
Mat outputImage;

int x,y;
bool detected;
int lastID;

/* dynamic reconfigure of surf threshold and showing images */
//void callback(rr_courtdetector::featureExtractionConfig &config, uint32_t level)
//{
//}

/* Extract features from image recieved from camera */
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	cv_bridge::CvImagePtr cv_ptr;
	try
	{
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception& e)
	{
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}
	inputImage=cv_ptr->image;
}

void cropPointCallback(const rr_msgs::BeePosition::ConstPtr& msg)
{
	x = msg->u;
	y = msg->v;
	detected = msg->visible;
}

int main(int argc, char** argv)
{ 
	ros::init(argc, argv, "cropper_node");
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_(nh_);

	/* Initiate dynamic reconfiguration */
	//dynamic_reconfigure::Server<stroll_bearnav::featureExtractionConfig> server;
	//dynamic_reconfigure::Server<stroll_bearnav::featureExtractionConfig>::CallbackType f = boost::bind(&callback, _1, _2);
	//server.setCallback(f);

	imageSub = it_.subscribe( "/image", 1,imageCallback);
	positionSub = nh_.subscribe("/queenposition", 1, cropPointCallback);

	ros::spin();
	return 0;
}
