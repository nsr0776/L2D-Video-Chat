#ifdef _TEST_RUN__

#ifndef TEST_ANIMATOR_HPP_
#define TEST_ANIMATOR_HPP_

#include <LApp/LAppDelegate.hpp>
#include "Communicator.hpp"
#include "detection.h"
#include <QApplication>
#include <QTimer>

class TestAnimator {
public:
	static int runTest(int argc, char *argv[]) {

		FacialLandmarkDetector::getInstance()->startDetector();

		// create the application instance
		auto lapp = LAppDelegate::GetInstance();

		QCoreApplication a(argc, argv);
		a.exec();
		
		lapp->Release();
		LAppDelegate::ReleaseInstance();

		return 114;
	}

private:

};

#endif

#endif