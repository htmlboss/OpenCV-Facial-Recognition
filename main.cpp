#include <QApplication>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda_types.hpp>
#include <opencv2/core/cuda.inl.hpp>
#include <QDebug>
#include <QMessageBox>
#include <QException>

#include "gui/mainwindow.h"

Q_DECLARE_METATYPE(cv::Mat)
/** @function main */
int main( int argc, char* argv[] ) {

	int result = -1;
	try {
		qRegisterMetaType<cv::Mat>();
		QApplication::setApplicationName("OpenCV-Node");
		QApplication::setApplicationVersion("1.0");
		QApplication app(argc, argv);
		MainWindow window;
		window.show();
		result = app.exec();
	}
	catch(QException &e) {
		qCritical() << QString("Exception: %1").arg(e.what());
		QMessageBox* box = new QMessageBox(QMessageBox::Critical, "Fatal Error", e.what());
		box->show();
	}
	catch (...) {
		qCritical() << "Unhandled Exception.";
	}

	return result;
}
