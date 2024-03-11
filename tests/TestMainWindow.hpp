#ifdef _TEST_RUN__

#ifndef TEST_MAIN_WINDOW_HPP_
#define TEST_MAIN_WINDOW_HPP_

#include "mainwindow.h"
#include "detection.h"
#include <QApplication>

class TestMainWindow {
public:
	static int runTest(int argc, char *argv[]) {
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		return a.exec();
	}
};

#endif

#endif