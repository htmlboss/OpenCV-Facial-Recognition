#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/core/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/face.hpp>
#include <QObject>
#include <QLabel>
#include <QVector2D>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QDir>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QResource>
#include <QScopedPointer>
#include <QImage>
#include <QFileInfo>

#include <vector>

#include "tcpserver.h"

class FaceDetector : public QObject {
	Q_OBJECT

public:
	FaceDetector(QObject *parent = Q_NULLPTR);
	~FaceDetector();

	void setProcessAll(bool all);

signals:
	void image_signal(const QImage&);

public slots:
	void processFrame(const cv::Mat& frame);
	void facecascade_filename(QString filename);

private:
	QString facecascade_filename_;
	QString eyecascade_filename_;
	QBasicTimer* timer_;
	std::vector<cv::Mat> m_CSVimages;
	std::vector<int> m_CSVlabels;
	std::vector<std::string> m_CSVnames;
	cv::Mat frame_;
	bool processAll_;
	cv::CascadeClassifier faceCascade;
	cv::CascadeClassifier eyeCascade;
	cv::Ptr<cv::face::FaceRecognizer> m_faceModel;

	void process(cv::Mat frame);
	void loadFiles(const cv::String& faceCascadeFilename, const cv::String& eyesCascadeFilename);
	static void loadCSV(const cv::String& filePath, std::vector<cv::Mat>& images, std::vector<int>& labels, std::vector<std::string> &names);

	void queue(const cv::Mat & frame);
	void timerEvent(QTimerEvent* ev);
	static void matDeleter(void* mat);

	TCPserver* m_tcpServer;
};

#endif // FACEDETECTOR_H
