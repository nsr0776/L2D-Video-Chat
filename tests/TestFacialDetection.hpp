#ifdef _TEST_RUN__

#ifndef TEST_FACIAL_DETECTION_HPP_
#define TEST_FACIAL_DETECTION_HPP_

#include <nlohmann/json.hpp>
#include "detection.h"
#include "Network_QT.h"
#include <iostream>
#include <QTextStream>
#include <JsonConverter.hpp>
#include <LApp/LAppPal.hpp>
#include <LApp/LAppAllocator.hpp>
#include <QThread>
#include <QObject>
#include <QApplication>

class TestFacialDetection : public QObject {
	//Q_OBJECT;		// [IMPORTANT] For unknown reasons, if Q_OBJECT is added then the linker will complain

public:
	static void runTest(int argc, char* argv[]) {
		// Cubism Framework initialization
		LAppAllocator allocator;
		Csm::CubismFramework::StartUp(&allocator);
		Csm::CubismFramework::Initialize();

		auto detectorHandle = FacialLandmarkDetector::getInstance();
		detectorHandle->startDetector();
		
		TestFacialDetection instance;
		QThread thread;
		instance.moveToThread(&thread);
		thread.start();

		QCoreApplication a(argc, argv);
		a.exec();

		// Cubism Framework disposal
		Csm::CubismFramework::Dispose();
	}


private:
	TestFacialDetection() {
		QObject::connect(FacialLandmarkDetector::getInstance(), &FacialLandmarkDetector::NewDetection,
						this, &TestFacialDetection::printJson);
	}
	TestFacialDetection(const TestFacialDetection&) = delete;

	//void doRun() {
	//	if (_thread != nullptr && _thread->isRunning()) {
	//		qDebug().noquote() << "[TestFacialDetector] Thread is already running";
	//	}
	//	else {
	//		_thread = new QThread;

	//	}
	//}


private slots:
	void printJson() {
		auto j = Network_QT::getInstance()->getSendJson();

		if ((*j)["data"].is_null()) {
			qDebug().noquote() << "[printJson] data is null";
		}
		else {
			using namespace Csm;

			csmByte* buffer;	csmSizeInt size;
			Utils::CubismJson* cuJson = JsonConverter::convert(j, &buffer, &size);
			const char* text = cuJson->GetRoot().GetString().GetRawString();
			qDebug().noquote() << QString(text);
			LAppPal::ReleaseBytes(buffer);
			Utils::CubismJson::Delete(cuJson);
		}
	}
};

#endif

#endif