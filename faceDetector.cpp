#include "faceDetector.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

FaceDetector::FaceDetector(QObject *parent) : QObject(parent), processAll_(true), m_tcpServer(new TCPserver(parent)) {

	timer_ = new QBasicTimer();
	facecascade_filename_ = "C:/Users/Nabil Miri/Documents/Visual Studio 2015/Projects/opencv-test/resources/haarcascade_frontalface_default.xml";
	eyecascade_filename_ = "C:/Users/Nabil Miri/Documents/Visual Studio 2015/Projects/opencv-test/resources/haarcascade_eye.xml";

	loadFiles(facecascade_filename_.toStdString().c_str(), eyecascade_filename_.toStdString().c_str());
	loadCSV("C:/Users/Nabil Miri/Documents/Visual Studio 2015/Projects/opencv-test/faces.txt", m_CSVimages, m_CSVlabels, m_CSVnames);

	m_faceModel = cv::face::createFisherFaceRecognizer();
	m_faceModel->train(m_CSVimages, m_CSVlabels);
}

FaceDetector::~FaceDetector() {
}

void FaceDetector::loadFiles(const cv::String& faceCascadeFilename, const cv::String& eyeCascadeFilename) {
	// TODO: Add in a try catch statement here
	if( !faceCascade.load( faceCascadeFilename ) ) {
		qDebug() << "Error Loading" << faceCascadeFilename.c_str();
	}
	if( !eyeCascade.load( eyeCascadeFilename ) ) {
		qDebug() << "Error Loading" << eyeCascadeFilename.c_str();
	}
}

void FaceDetector::loadCSV(const cv::String& filePath, std::vector<cv::Mat>& images, std::vector<int>& labels, std::vector<std::string>& names) {
	//Load CSV
	std::ifstream file(filePath.c_str());
	if(!file) {
		qDebug() << "Error: " << std::strerror(errno);
	}

	std::string line, path, classlabel, name;
	while(std::getline(file, line)) {
		std::stringstream liness(line);
		std::getline(liness, path, ';');
		std::getline(liness, classlabel, ';');
		std::getline(liness, name);

		QFileInfo check_file(path.c_str());
		//Check if parsed filepath is valid
		if (check_file.exists() && check_file.isFile()) {
			//Read-in images in greyscale
			if(!path.empty() && !classlabel.empty() && !name.empty()) {
				images.push_back(cv::imread(path.c_str(), CV_LOAD_IMAGE_GRAYSCALE));
				labels.push_back(atoi(classlabel.c_str()));
				names.push_back(name);
			}
		} //End check_file
		else {
			qDebug() << "Failed: " << path.c_str();
		}
	} //end of while

	//Check for empty images in storage
	for (int i = 0; i < images.size(); ++i) {
		if (!images[i].data) {
			qDebug() << "Bad image: " << i;
		}
	}
}

void FaceDetector::processFrame(const cv::Mat &frame) {
	if (processAll_)
	process(frame);
	else
	queue(frame);
}

void FaceDetector::setProcessAll(bool all) {
	processAll_ = all;
}

void FaceDetector::process(cv::Mat frame) {
	cv::Mat grey_image;
	cv::cvtColor(frame, grey_image, CV_BGR2GRAY);
	cv::equalizeHist(grey_image, grey_image);
	std::vector<cv::Rect> faces;

	// Calculate the camera size and set the size to 1/8 of screen height
	/*
	 * Params: const Mat& image, vector<Rect>& objects, double scaleFactor=1.1, int minNeighbors=3, int flags=0, Size minSize=Size(), Size maxSize=Size()
	 *
	 * scaleFactor – Parameter specifying how much the image size is reduced at each image scale. Basically the scale factor is used
	 * to create your scale pyramid. More explanation can be found here. In short, as described here, your model has a fixed size
	 * defined during training, which is visible in the xml. This means that this size of face is detected in the image if present.
	 * However, by rescaling the input image, you can resize a larger face to a smaller one, making it detectable by the algorithm.
	 * 1.05 is a good possible value for this, which means you use a small step for resizing, i.e. reduce size by 5%, you increase
	 * the chance of a matching size with the model for detection is found. This also means that the algorithm works slower since
	 * it is more thorough. You may increase it to as much as 1.4 for faster detection, with the risk of missing some faces altogether.
	 *
	 * minNeighbors – Parameter specifying how many neighbors each candidate rectangle should have to retain it. This parameter will
	 * affect the quality of the detected faces. Higher value results in less detections but with higher quality. 3~6 is a good value for it.
	 *
	 * minSize – Minimum possible object size. Objects smaller than that are ignored. This parameter determine how small size you
	 * want to detect. You decide it! Usually, [30, 30] is a good start for face detection.
	 */

	faceCascade.detectMultiScale(grey_image, faces, 1.1, 5, CV_HAAR_SCALE_IMAGE, cv::Size(frame.cols/4, frame.rows/4)); // Minimum size of obj

	//-- Draw rectangles around faces
	for(int i = 0; i < faces.size(); ++i) {
		cv::Mat image_ROI = frame(faces[i]);
		cv::Mat face_resized;

		//Resize the face to cv::Size() and convert BACK to gray
		cv::resize(image_ROI, face_resized, cv::Size(200, 200), 0.0, 0.0, cv::INTER_CUBIC);
		cv::cvtColor(face_resized, face_resized, CV_BGR2GRAY);

		//Make prediction
		int prediction = m_faceModel->predict(face_resized);

		//Draw Rectangle around face
		cv::rectangle(frame, faces[i], CV_RGB(255, 0, 255), 1);

		//Create annotation and prepare label data
		std::string text = cv::format("Prediction: %d", prediction);
		float posX = std::max(faces[i].tl().x - 10, 0);
		float posY = std::max(faces[i].tl().y - 10, 0);

		cv::putText(frame, text, cv::Point(posX, posY), cv::FONT_HERSHEY_PLAIN, 1.0f, CV_RGB(0, 255, 0), 1.25f, cv::LINE_AA, false);
	}
	cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
	const QImage image(static_cast<const unsigned char*>(frame.data), frame.cols, frame.rows, frame.step, QImage::Format_RGB888, &matDeleter, new cv::Mat(frame));
	image.rgbSwapped();
	Q_ASSERT(image.constBits() == frame.data);

	//Send processed image
	Q_EMIT image_signal(image);
}

void FaceDetector::timerEvent(QTimerEvent *ev) {
	if (ev->timerId() != timer_->timerId()) {
		return;
	}
	process(frame_);
	frame_.release();
	timer_->stop();
}

void FaceDetector::queue(const cv::Mat &frame) {
	if (!frame.empty())
	qDebug() << "Converter dropped frame !";
	frame_ = frame;
	if (!timer_->isActive())
	timer_->start(0, this);
}

void FaceDetector::matDeleter(void *mat) {
	delete static_cast<cv::Mat*>(mat);
}

void FaceDetector::facecascade_filename(QString filename) {
	cv::String temp = filename.toStdString().c_str();
	if( !faceCascade.load( temp ) ) {
		std::cout << "Error Loading" << filename.toStdString() << std::endl;
	}
	facecascade_filename_ = filename;
	// FIXME: Incorrect Implementation
	loadFiles(filename.toStdString().c_str(), filename.toStdString().c_str());
}
