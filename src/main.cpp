

#define _TEST_RUN__
#include "../tests/TestMainWindow.hpp"
//#include "../tests/TestAnimator.hpp"
//#include "../tests/TestFacialDetection.hpp"
#undef _TEST_RUN__

#include "LApp/LAppAllocator.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
	
	// Do something here
	int ret = TestMainWindow::runTest(argc, argv);
	return ret;
}
