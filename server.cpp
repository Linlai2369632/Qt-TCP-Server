#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

server::server()
{
    Server = new QTcpServer(this);
    // 客戶端發起連接，server發出信號
    if(Server->listen(QHostAddress::AnyIPv4, 8000)) {
        qDebug() << "正在監聽 port:8000";
    }
    else {
        QWidget *widget = new QWidget;
        QMessageBox::critical(widget,"QTCPServer",QString("Unable to start the server: %1.").arg(Server->errorString()));
        exit(EXIT_FAILURE);
    }

    connect(Server, &QTcpServer::newConnection, this, &server::newClientHandler);
}

void server::newClientHandler()
{
    qDebug() << "New Connection";
    // 建立 TCP 連接
    QTcpSocket *Socket = Server->nextPendingConnection();
//    Socket = Server->nextPendingConnection();

    clientAddress = Socket->peerAddress().toString();
    clientPort = QString::number(Socket->peerPort());
    qDebug() << "Client connected from:" << clientAddress << "Port = " << clientPort;
}

