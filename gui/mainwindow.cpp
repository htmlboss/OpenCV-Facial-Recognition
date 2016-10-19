#include "gui/mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

	statusBar()->setSizeGripEnabled(false);

	//Set stylesheet
	QFile f("../../gui/style/Dark/style.qss");
	if(!f.exists()) {
		qDebug() << "Unable to set Dark stylesheet. File not found";
		statusBar()->showMessage(tr("Unable to set Dark stylesheet. File not found."));
	} else {
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		this->setStyleSheet(ts.readAll());
	}

	setWindowTitle(QApplication::applicationName() + " " + QApplication::applicationVersion());

	face_cascade_ = new QAction("Change Face Cascade", this);
	this->connect(face_cascade_, SIGNAL(triggered()), SLOT(change_face_cascade()));

	DisplayWidget* display = new DisplayWidget(this);
	display->connect(this, SIGNAL(face_cascade_filename_signal(QString)), SLOT(change_face_cascade_filename(QString)));

	setCentralWidget(display);
	setMouseTracking(true);

	m_menu = new QMenuBar(this);
	createActions();
	createMenus();

}

MainWindow::~MainWindow() {
}

void MainWindow::change_face_cascade() {
	QString filename = QFileDialog::getOpenFileName(this, tr("HaarCascade Filename"));

	Q_EMIT face_cascade_filename_signal(filename);
}

void MainWindow::about() {

}

void MainWindow::aboutQT() {
	QMessageBox::aboutQt(this, "About QT");
}

void MainWindow::aboutOpenCV() {
	QMessageBox* about = new QMessageBox(QMessageBox::NoIcon, "About OpenCV", "tasdf", QMessageBox::Ok);
	about->show();
}

void MainWindow::options_Camera() {

}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
	QMenu menu(this);
	menu.addAction(m_aboutAct);
	menu.exec(event->globalPos());

}

void MainWindow::createActions() {

	m_aboutAct = new QAction(tr("About"), m_menu);
	m_aboutAct->setStatusTip(tr("About"));
	connect(m_aboutAct, &QAction::triggered, this, &MainWindow::about);

	m_aboutCVAct = new QAction(tr("About OpenCV"), m_menu);
	m_aboutCVAct->setStatusTip(tr("About OpenCV"));
	connect(m_aboutCVAct, &QAction::triggered, this, &MainWindow::aboutOpenCV);

	m_aboutQTAct = new QAction(tr("About QT"), m_menu);
	m_aboutQTAct->setStatusTip(tr("About QT"));;
	connect(m_aboutQTAct, &QAction::triggered, this, &MainWindow::aboutQT);

}

void MainWindow::createMenus() {

	m_optionsMenu = m_menu->addMenu(tr("Tools"));
	m_optionsMenu->addAction(m_optionscameraAct);

	m_helpMenu = m_menu->addMenu(tr("Help"));
	m_helpMenu->addAction(m_aboutAct);
	m_helpMenu->addAction(m_aboutQTAct);
	m_helpMenu->addAction(m_aboutCVAct);
}
