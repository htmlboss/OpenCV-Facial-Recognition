#include "tcpserver.h"
#include <QApplication>

TCPserver::TCPserver(QObject *parent) : m_tcpserver(Q_NULLPTR), m_networkSession(Q_NULLPTR) {

	QNetworkConfigurationManager manager;
	if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
	   // Get saved network configuration
	   QSettings settings(QSettings::UserScope, QApplication::applicationName());
	   settings.beginGroup(QLatin1String("QtNetwork"));
	   const QString id = settings.value(QLatin1String("DefaultNetworkConfig")).toString();
	   settings.endGroup();

	   // If the saved network configuration is not currently discovered use the system default
	   QNetworkConfiguration config = manager.configurationFromIdentifier(id);
	   if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
		   config = manager.defaultConfiguration();
	   }

		m_networkSession = new QNetworkSession(config, this);
		connect(m_networkSession, &QNetworkSession::opened, this, &TCPserver::sessionOpened);

		m_networkSession->open();
	}
	else {
		sessionOpened();
	}

	connect(m_tcpserver, &QTcpServer::newConnection, this, &TCPserver::connected);

}

void TCPserver::sessionOpened() {

	// Save the used configuration
	if (m_networkSession) {

		QNetworkConfiguration config = m_networkSession->configuration();
		QString id;
		if (config.type() == QNetworkConfiguration::UserChoice) {
			id = m_networkSession->sessionProperty(QLatin1String("UserChoiceConfig")).toString();
		}
		else {
			id = config.identifier();
		}

		QSettings settings(QSettings::UserScope, QApplication::applicationName());
		settings.beginGroup(QLatin1String("QtNetwork"));
		settings.setValue(QLatin1String("DefaultNetworkConfig"), id);
		settings.endGroup();
	}

	m_tcpserver = new QTcpServer(this);
	if (!m_tcpserver->listen(QHostAddress::Any, 1701)) {
		qDebug() << "Unable to start the server: " << m_tcpserver->errorString();
		return;
	}

	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
			ipAddress = ipAddressesList.at(i).toString();
			break;
		}
	}

	// if we did not find one, use IPv4 localhost
	if (ipAddress.isEmpty()) {
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	}

	qDebug() << "Server started on " << ipAddress << "port: " << 1701;
}

void TCPserver::connected() {

	QTcpSocket* client = m_tcpserver->nextPendingConnection();
	if (!client) {
		qDebug() << client->errorString();
		return;
	}
	connect(client, &QAbstractSocket::disconnected, client, &QObject::deleteLater);

	qDebug() << "Connected from " << client->peerAddress() << " port " << client->localPort();

	client->disconnectFromHost();
}
