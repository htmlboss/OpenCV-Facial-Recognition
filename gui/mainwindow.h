#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QFile>
#include <QCloseEvent>
#include "gui/displaywidget.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
signals:
	void face_cascade_filename_signal(QString casacde_filename);

private slots:
	void change_face_cascade();

	//Menu
	void about();
	void aboutQT();
	void aboutOpenCV();
	void options_Camera();

protected:
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
	QMenuBar* m_menu;

	QAction* face_cascade_;

	QAction* m_aboutAct;
	QAction* m_aboutQTAct;
	QAction* m_aboutCVAct;
	QAction* m_optionscameraAct;

	QMenu* m_optionsMenu;
	QMenu* m_helpMenu;

	void createActions();
	void createMenus();
};

#endif // MAINWINDOW_H
