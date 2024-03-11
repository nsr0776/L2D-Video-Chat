/*
 * @Author: Luke_lu
 * @Date: 2019-12-14 17:23:14
 * @LastEditTime: 2019-12-14 22:35:35
 * @Description: Class of Facial recognition, landmark detection, and iris detect
 */

#ifndef DETECTION_H_
#define DETECTION_H_

#include <thread>
#include <vector>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <Network_QT.h>
#include <Setting.h>
#include <QObject>
#include <QDebug>


/**
 * @description: Obsolete data struct
 */
typedef struct RawFacePos {
	float angle;//+clockwise TODO
	bool inited = true;
	int faceLeftX;
	int faceRightX;
	int faceDownY, faceTopY;
	int noseMiddleMaxX, noseMiddleMinX, noseMiddleMaxY, noseMiddleMinY;
	int leftEyebrowY;
	int rightEyebrowY;
	int leftEyeMinX, leftEyeMaxX, leftEyeMinY, leftEyeMaxY;
	int rightEyeMinX, rightEyeMaxX, rightEyeMinY, rightEyeMaxY;
	int LipsInnerMaxY, LipsInnerMinY;
	cv::Vec3f leftIris;
	cv::Vec3f rightIris;
};

/**
 * @description: Single instance class of all detections
 */
class FacialLandmarkDetector:public QObject {
	Q_OBJECT;
private:
	void detection();
	FacialLandmarkDetector() {
		debug = Setting::getSetting()->debug;
		nuturalFace.inited = false;
		captureNuturalFaceFlag = false;
		detectThread = nullptr;
	};
	~FacialLandmarkDetector() {
		//detectThread->join();
	}
	FacialLandmarkDetector(FacialLandmarkDetector&) = delete;
	FacialLandmarkDetector operator=(FacialLandmarkDetector) = delete;
	QThread* detectThread;
	static FacialLandmarkDetector* facialLandmarkDetector;
	RawFacePos nuturalFace;
	bool captureNuturalFaceFlag;
public:
	bool CameraInitError;
	bool ModelLoadError;
	bool MultipleFaceWarning;
	bool debug;
	static FacialLandmarkDetector* getInstance() {
		if (facialLandmarkDetector == NULL) {
			facialLandmarkDetector = new FacialLandmarkDetector();
		}
		return facialLandmarkDetector;
	}

	/**
  * @description: Start the detection thread, throw exception if already runing
  * @Exception "Already Running"
  */ 
 void startDetector() {
		if (detectThread != nullptr && detectThread->isRunning()) {
			throw std::exception("Already Running");
		}
		else {
			CameraInitError = false;
			ModelLoadError = false;
			MultipleFaceWarning = false;
			//detectThread = new std::thread(&FacialLandmarkDetector::detection, this);
			detectThread = new QThread();
			facialLandmarkDetector->moveToThread(detectThread);
			detectThread->start();
			QObject::connect(facialLandmarkDetector, &FacialLandmarkDetector::startDetectionSignal, facialLandmarkDetector, &FacialLandmarkDetector::startDetectionSlot);
			emit startDetectionSignal();//Start detection
		}
	}


	/**
  * @description: Obsolete function
  */ 
 void captureNuturalFace() {
		captureNuturalFaceFlag = true;
		while (captureNuturalFaceFlag) {}
	}
signals:
	void NewDetection();
	void startDetectionSignal();

private slots:
	void startDetectionSlot() {
		detection();
	}

};

#endif