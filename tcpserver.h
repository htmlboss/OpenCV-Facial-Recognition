#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtNetwork>
#include <QTcpServer>
#include <QAbstractSocket>
#include <QObject>

class TCPserver : public QObject {
	Q_OBJECT
public:
	explicit TCPserver(QObject *parent = Q_NULLPTR);

private slots:
	void sessionOpened();
	void connected();

private:
	QTcpServer* m_tcpserver;
	QNetworkSession* m_networkSession;
};

#endif // TCPSERVER_H
