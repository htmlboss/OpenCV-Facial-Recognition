#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDebug>

class ImageViewer : public QWidget {
	Q_OBJECT

public:
	explicit ImageViewer(QWidget *parent = 0) : QWidget(parent) {
		setAttribute(Qt::WA_OpaquePaintEvent);
	}

signals:

public slots:
	void set_image(const QImage & img);

private:
	QImage image_;

	void paintEvent(QPaintEvent *) {
		QPainter p(this);
		p.drawImage(0, 0, image_);
		image_ = QImage();
	}
};

#endif // IMAGEVIEWER_H
