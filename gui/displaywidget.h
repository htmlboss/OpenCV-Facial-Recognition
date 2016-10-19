#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QThread>
#include <QCameraInfo>

#include <iostream>
#include <stdio.h>

#include "gui/imageviewer.h"
#include "camera.h"
#include "faceDetector.h"
#include "tcpserver.h"

class DisplayWidget : public QWidget {
	Q_OBJECT
public:
	explicit DisplayWidget(QWidget *parent = 0);
	~DisplayWidget();

	QComboBox* InitCameraComboBox();

signals:
	void videoFileNameSignal(QString videoFileName);
	void facecascade_name_signal(QString filename);

public slots:
	void openFileDialog();
	void change_face_cascade_filename(QString filename);

private:
	FaceDetector* faceDector_;
	Camera* camera_;
	QThread faceDetectThread_;
	QThread cameraThread_;
	ImageViewer* image_viewer_;
	QFont m_font;
};

#endif // DISPLAYWIDGET_H
