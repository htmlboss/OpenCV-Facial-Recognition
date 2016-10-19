#include "gui/displaywidget.h"

DisplayWidget::DisplayWidget(QWidget *parent) : QWidget(parent), m_font("Arial", 11, QFont::Bold) {

	QComboBox* cameraOptions = InitCameraComboBox();

	QVBoxLayout* verticalLayout = new QVBoxLayout();
	QPushButton* runButton = new QPushButton("Run", this);
	runButton->setFont(m_font);
	runButton->setMinimumWidth(250);
	runButton->setMaximumWidth(250);

	verticalLayout->setSpacing(15);
	verticalLayout->setMargin(10);
	verticalLayout->addWidget(cameraOptions);
	verticalLayout->addWidget(runButton);

	QHBoxLayout* layout = new QHBoxLayout();
	image_viewer_ = new ImageViewer(this);

	layout->setSpacing(15);
	layout->setMargin(10);
	layout->addWidget(image_viewer_);
	layout->addLayout(verticalLayout);

	QSpacerItem* spacer = new QSpacerItem(0, 25);
	QVBoxLayout* masterLayout = new QVBoxLayout();
	masterLayout->addSpacerItem(spacer);
	masterLayout->addLayout(layout);

	setLayout(masterLayout);

	camera_ = new Camera(cameraOptions->currentIndex());
	faceDector_ = new FaceDetector();

	//faceDector_->setProcessAll(false);

	faceDetectThread_.start();
	cameraThread_.start();

	camera_->moveToThread(&cameraThread_);
	faceDector_->moveToThread(&faceDetectThread_);

	// TODO: Add in slot to turn off camera_, or something
	image_viewer_->connect(faceDector_, SIGNAL(image_signal(QImage)), SLOT(set_image(QImage)));

	faceDector_->connect(camera_, SIGNAL(matReady(cv::Mat)), SLOT(processFrame(cv::Mat)));

	QObject::connect(runButton, SIGNAL(clicked()), camera_, SLOT(runSlot()));

	QObject::connect(cameraOptions, SIGNAL(currentIndexChanged(int)), camera_, SLOT(cameraIndexSlot(int)));

	QObject::connect(this, SIGNAL(videoFileNameSignal(QString)), camera_, SLOT(videoFileNameSlot(QString)));

	faceDector_->connect(this, SIGNAL(facecascade_name_signal(QString)), SLOT(facecascade_filename(QString)));
}

DisplayWidget::~DisplayWidget() {
	faceDector_->~FaceDetector();
	camera_->~Camera();
	faceDetectThread_.quit();
	cameraThread_.quit();
	faceDetectThread_.wait();
	cameraThread_.wait();
}

QComboBox* DisplayWidget::InitCameraComboBox() {

	QStringList cameraOptions;
	QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
	foreach (const QCameraInfo &cameraInfo, cameras) {
		cameraOptions << cameraInfo.description();
	}

	QComboBox* cameraComboBox = new QComboBox(this);
	cameraComboBox->addItems(cameraOptions);
	cameraComboBox->setFont(m_font);
	cameraComboBox->setEditable(true);
	cameraComboBox->lineEdit()->setReadOnly(true);
	cameraComboBox->lineEdit()->setAlignment(Qt::AlignCenter);

	for (int i = 0; i < cameraComboBox->count(); ++i) {
		cameraComboBox->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
	}
	cameraComboBox->setEditable(false);
	cameraComboBox->setMinimumWidth(250);
	cameraComboBox->setMaximumWidth(300);

	return cameraComboBox;
}

void DisplayWidget::change_face_cascade_filename(QString filename) {
	Q_EMIT facecascade_name_signal(filename);
}

void DisplayWidget::openFileDialog() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Video"));
	Q_EMIT videoFileNameSignal(filename);
}
