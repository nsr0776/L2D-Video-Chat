// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

	This example program shows how to find frontal human faces in an image and
	estimate their pose.  The pose takes the form of 68 landmarks.  These are
	points on the face such as the corners of the mouth, along the eyebrows, on
	the eyes, and so forth.


	This example is essentially just a version of the face_landmark_detection_ex.cpp
	example modified to use OpenCV's VideoCapture object to read from a camera instead
	of files.


	Finally, note that the face detector is fastest when compiled with at least
	SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
	chip then you should enable at least SSE2 instructions.  If you are using
	cmake to compile this program you can enable them by using one of the
	following commands when you create the build project:
		cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
		cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
		cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
	This will set the appropriate compiler options for GCC, clang, Visual
	Studio, or the Intel compiler.  If you are using another compiler then you
	need to consult your compiler's manual to determine how to enable these
	instructions.  Note that AVX is the fastest but requires a CPU from at least
	2011.  SSE4 is the next fastest and is supported by most current machines.
*/

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <detection.h>
#include <cmath>


using namespace dlib;
using namespace nlohmann;

FacialLandmarkDetector* FacialLandmarkDetector::facialLandmarkDetector = nullptr;



/*
Helper funtions
From https://github.com/HTTdesu/SimpleFacerig
*/
double meter(double A, double B, double C, double x, double y)
{
	double diversion = A * x + B * y + C;
	return diversion / sqrt(pow(A, 2) + pow(B, 2));
}

double eyebrow_move(dlib::point& p1, dlib::point& p2, double slope, double last, double rate)
{
	double bias = p1.y() - slope * p1.x();
	double distance = sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
	double diversion = meter(slope, -1, bias, p2.x(), p2.y());
	double result = (diversion / distance - 0.45) * 6;
	result = last * (1 - rate) + result * rate;
	return result;
}

double eye_open(dlib::point& p1, dlib::point& p2, dlib::point& v11, dlib::point& v12, dlib::point& v21, dlib::point& v22, double last, double rate)
{
	double distance = sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
	double slope = (double)(p2.y() - p1.y()) / (double)(p2.x() - p1.x());
	double bias = p2.y() - slope * p2.x();
	double d1 = abs(meter(slope, -1, bias, v11.x(), v11.y()));
	double d2 = abs(meter(slope, -1, bias, v12.x(), v12.y()));
	double diversion = d1 > d2 ? d1 : d2;
	d1 = abs(meter(slope, -1, bias, v21.x(), v21.y()));
	d2 = abs(meter(slope, -1, bias, v22.x(), v22.y()));
	diversion += d1 > d2 ? d1 : d2;
	double ratio = (diversion / distance - 0.18) * 8;
	ratio = ((int)(ratio * 10.0)) / 10.0;
	ratio = last * (1 - rate) + ratio * rate;
	return ratio;
}
/*
Helper funtions end
*/


/**
 * @description: Main detection loop
 * Interpetation of facial landmark point from https://github.com/HTTdesu/SimpleFacerig
 * Modified
 */
void FacialLandmarkDetector::detection()
{
	dlib::frontal_face_detector detector;//facial regonition
	dlib::shape_predictor pose_model;//landmark detection
	cv::CascadeClassifier haarcascade_eye_tree_eyeglasses;//Iris detection
	// live2d parameters
	float x_rotate = 0.0f;
	float y_rotate = 0.0f;
	float z_rotate = 0.0f;

	float left_eye = 1.0f;
	float right_eye = 1.0f;

	float eyebrow_left = 0.0f;
	float eyebrow_right = 0.0f;

	float eyeBallX = 0.0f;
	float eyeBallY = 0.0f;

	float mouth_open = 0.0f;
	//Eyes positions
	int leftEyeMaxX = 0;
	int leftEyeMinX = 0;
	int leftEyeMinY = 0;
	int leftEyeMaxY = 0;
	int rightEyeMaxX = 0;
	int rightEyeMinX = 0;
	int rightEyeMinY = 0;
	int rightEyeMaxY = 0;
	try
	{
		cv::VideoCapture camera(0);
		if (!camera.isOpened())
		{
			std::cerr << "Unable to connect to camera" << std::endl;
			return;
		}

		//image_window win;

		// Load face detection and pose estimation models.
		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor pose_model;
		haarcascade_eye_tree_eyeglasses.load("..\\..\\res\\haarcascade_eye_tree_eyeglasses.xml");//OpenCV Model
		deserialize("..\\..\\res\\shape_predictor_68_face_landmarks.dat") >> pose_model; //dlib model

		// Grab and process frames until the main window is closed by the user.
		while (camera.isOpened())
		{
			cv::Mat buff, frame;
			camera.read(frame);
			//cv::GaussianBlur(temp, buff, cv::Size(5, 5), 5, 5);
			cv::flip(frame, frame, 1);

//The result of following Process is process facial landmark points and get positions
//Copy from https://github.com/HTTdesu/SimpleFacerig. Authorized by author
			dlib::cv_image<dlib::bgr_pixel> cimg(frame);
			std::vector<dlib::rectangle> faces = detector(cimg);//get faces
			std::vector<dlib::full_object_detection> shapes;

			for (unsigned long i = 0; i < faces.size(); ++i)
				shapes.push_back(pose_model(cimg, faces[i]));

			if (!shapes.empty()) {
				for (int i = 0; i < 68; i++) {
					cv::circle(frame, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 2, cv::Scalar(0, 0, 255), -1);
				}

				int div_x = shapes[0].part(16).x() - shapes[0].part(0).x();
				int div_y = shapes[0].part(16).y() - shapes[0].part(0).y();
				double center_x = shapes[0].part(0).x() + div_x / 2.0;
				double center_y = shapes[0].part(0).y() + div_y / 2.0;
				double slope = (double)(div_y) / (double)(div_x);
				double bias = center_y - slope * center_x;
				double x_proj = (slope * (shapes[0].part(30).y() - bias) + shapes[0].part(30).x()) / (1 + pow(slope, 2));
				double y_proj = slope * x_proj + bias;
				double diversion = sqrt(pow(x_proj - shapes[0].part(0).x(), 2) + pow(y_proj - shapes[0].part(0).y(), 2));
				double distance = sqrt(pow(shapes[0].part(16).x() - shapes[0].part(0).x(), 2) + pow(shapes[0].part(16).y() - shapes[0].part(0).y(), 2));

				double rate = 0.5;
				// Ax+By+C/sqrt(A^2+B^2)
				x_rotate = x_rotate * (1 - rate) + asin(diversion / distance - 0.5) * 3.14 * 40.0 * rate;

				// nose to eye around 1/6 head
				diversion = meter(slope, -1, bias, shapes[0].part(30).x(), shapes[0].part(30).y());
				diversion = diversion + 1.0 / 6 * distance;
				y_rotate = y_rotate * (1 - rate) + asin(diversion / distance) * 3.14 * 40.0 * rate;

				z_rotate = z_rotate * (1 - rate) + atan(slope) * 3.14 * 40 * rate;

				// eye
				left_eye = eye_open(shapes[0].part(36), shapes[0].part(39), shapes[0].part(37), shapes[0].part(38), shapes[0].part(40), shapes[0].part(41), left_eye, rate);
				right_eye = eye_open(shapes[0].part(42), shapes[0].part(45), shapes[0].part(43), shapes[0].part(44), shapes[0].part(46), shapes[0].part(47), right_eye, rate);

				// eyebrow
				eyebrow_left = eyebrow_move(shapes[0].part(17), shapes[0].part(19), slope, eyebrow_left, rate);
				eyebrow_right = eyebrow_move(shapes[0].part(26), shapes[0].part(24), slope, eyebrow_right, rate);

				// mouth
				diversion = sqrt(pow(shapes[0].part(62).x() - shapes[0].part(66).x(), 2) + pow(shapes[0].part(62).y() - shapes[0].part(66).y(), 2));
				distance = sqrt(pow(shapes[0].part(60).x() - shapes[0].part(64).x(), 2) + pow(shapes[0].part(60).y() - shapes[0].part(64).y(), 2));
				mouth_open = (diversion / distance - 0.15) * 2;

//process facial landmark points end

				dlib::full_object_detection& firstFace = shapes[0];//only proces one face, don't support multiple faces
				// Left eye range
				{

					int minX = INT_MAX, maxX = 0, minY = INT_MAX, maxY = 0;
					for (unsigned long i = 37; i <= 41; ++i) {
						minX = minX < firstFace.part(i).x() ? minX : firstFace.part(i).x();
						maxX = maxX > firstFace.part(i).x() ? maxX : firstFace.part(i).x();
						minY = minY < firstFace.part(i).y() ? minY : firstFace.part(i).y();
						maxY = maxY > firstFace.part(i).y() ? maxY : firstFace.part(i).y();
					}
					leftEyeMaxX = maxX;
					leftEyeMinX = minX;
					leftEyeMinY = minY;
					leftEyeMaxY = maxY;

				}

				// Right eye range
				{
					int minX = INT_MAX, maxX = 0, minY = INT_MAX, maxY = 0;
					for (unsigned long i = 43; i <= 47; ++i) {
						minX = minX < firstFace.part(i).x() ? minX : firstFace.part(i).x();
						maxX = maxX > firstFace.part(i).x() ? maxX : firstFace.part(i).x();
						minY = minY < firstFace.part(i).y() ? minY : firstFace.part(i).y();
						maxY = maxY > firstFace.part(i).y() ? maxY : firstFace.part(i).y();
					}
					rightEyeMaxX = maxX;
					rightEyeMinX = minX;
					rightEyeMinY = minY;
					rightEyeMaxY = maxY;
				}

				try {
					std::vector<cv::Rect> eyes;
					cv::Mat eyesRaw;
					camera.read(eyesRaw);
					cv::flip(eyesRaw, eyesRaw, 1);
					cv::cvtColor(eyesRaw, eyesRaw, cv::COLOR_BGR2GRAY);//灰度
					equalizeHist(eyesRaw, eyesRaw);//直方图均化
					haarcascade_eye_tree_eyeglasses.detectMultiScale(eyesRaw, eyes,
						1.1, 2, 0
						//|CV_HAAR_FIND_BIGGEST_OBJECT
						//|CV_HAAR_DO_ROUGH_SEARCH
						| cv::CASCADE_SCALE_IMAGE
						,
						cv::Size(30, 30));



					for (std::vector<cv::Rect>::const_iterator r = eyes.begin(); r != eyes.end(); r++)
					{
						cv::Point center;
						int radius;
						//center可以作为瞳孔的坐标
						center.x = cvRound(r->x + r->width * 0.5);
						center.y = cvRound(r->y + r->height * 0.5);
						//radius = (int)(cvRound(r->width + r->height)*0.25);
						radius = 2;
						//cv::imshow("eyesRaw", eyesRaw);
						int margin = 10;
						//Calaulate relative position of iris
						if (center.x < rightEyeMaxX + margin && center.x > rightEyeMinX - margin && center.y < rightEyeMaxY + margin && center.y > rightEyeMinY - margin) {
							eyeBallX = (center.x - ((rightEyeMaxX - rightEyeMinX) / 2.0f + rightEyeMinX)) / (rightEyeMaxX - rightEyeMinX) * 2;
							eyeBallY = (center.y - ((rightEyeMaxY - rightEyeMinY) / 2.0f + rightEyeMinY)) / (rightEyeMaxY - rightEyeMinY) * 2;
							cv::circle(frame, center, radius, cv::Scalar(255, 0, 0), 3, 8, 0);

						}
						else if (center.x < leftEyeMaxX + margin && center.x > leftEyeMinX - margin && center.y < leftEyeMaxY + margin && center.y > leftEyeMinY - margin) {
							eyeBallX = (center.x - ((leftEyeMaxX - leftEyeMinX) / 2.0f + leftEyeMinX)) / (leftEyeMaxX - leftEyeMinX) * 2;
							eyeBallY = (center.y - ((leftEyeMaxY - leftEyeMinY) / 2.0f + leftEyeMinY)) / (leftEyeMaxY - leftEyeMinY) * 2;
							cv::circle(frame, center, radius, cv::Scalar(255, 0, 0), 3, 8, 0);

						}
						else {
							cv::circle(frame, center, radius, cv::Scalar(0, 255, 0), 3, 8, 0);
						}

					}
				}
				catch (std::exception e) {
					qDebug() << e.what();//Usually catch fail to load model
				}

				//Idea from
				//版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
				//本文链接：https://blog.csdn.net/computerme/article/details/38142125

				//Finalize and put into Json
				{
					json data;
					data["head"]["ParamAngleZ"] = z_rotate;
					data["head"]["ParamAngleX"] = x_rotate;
					data["head"]["ParamAngleY"] = y_rotate;
					data["eye"]["ParamEyeLOpen"] = left_eye;
					data["eye"]["ParamEyeROpen"] = right_eye;
					data["eye"]["ParamEyeBallX"] = eyeBallX;
					data["eye"]["ParamEyeBallY"] = eyeBallY;
					data["brow"]["ParamBrowLY"] = eyebrow_left;
					data["brow"]["ParamBrowRY"] = eyebrow_right;
					data["ParamMouthOpenY"] = mouth_open;

					Network_QT::getInstance()->updateMotion(data);
					
					emit NewDetection();
				}

			}
			else
			{
				x_rotate = 0.0f;
				y_rotate = 0.0f;
				z_rotate = 0.0f;
				left_eye = 1.0f;
				right_eye = 1.0f;
				eyebrow_left = 0.0f;
				eyebrow_right = 0.0f;
				mouth_open = 0.0f;
				eyeBallX = 0.0f;
				eyeBallY = 0.0f;
			}

			//double t = (live2d::UtSystem::getUserTimeMSec() / 1000.0) * 2 * 3.14;
			//live2DModel->setParamFloat("PARAM_ANGLE_Z", (float)(30 * sin(t / 3.0)));
			
			//cv::waitKey(30);
			//Display it all on the screen  
			//
			// Display it all on the screen
			if (debug) {
				system("cls");
				qDebug() << "x_rotate" << x_rotate << "\n"
					<< "y_rotate" << y_rotate << "\n"
					<< "z_rotate" << z_rotate << "\n"
					<< "left_eye" << left_eye << "\n"
					<< "right_eye" << right_eye << "\n"
					<< "eyebrow_left" << eyebrow_left << "\n"
					<< "eyebrow_right" << eyebrow_right << "\n"
					<< "mouth_open" << mouth_open << "\n"
					<< "eyeBallX" << eyeBallX << "\n"
					<< "eyeBallY" << eyeBallY << "\n";	
			}
			if (Setting::getSetting()->ShowFR) {
				cv::imshow("Feature points", frame);
				cv::waitKey(30);
			}

		}
			

		
	}catch (serialization_error & e)
	{//When fail to load dlib model
		std::cerr << "Fail to load model" << std::endl;
		qDebug().noquote() << "Fail to load model";
		this->ModelLoadError = true;
		return;
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
}
