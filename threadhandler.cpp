#include "threadHandler.h"

threadHandler::threadHandler(QTcpSocket *s)
{
    socket = s;
}

void threadHandler::run()
{
    connect(socket, &QTcpSocket::readyRead, this, &threadHandler::readSocket);
    connect(socket, &QTcpSocket::disconnected, [this](){
        emit clientDisconnected(socket);
    });
}

void threadHandler::readSocket()
{
    QByteArray ba;
    ba.append(socket->readAll());
    if(!ba.isEmpty()) {
        // 發射信號
        emit dataReaded(ba);
    }
}
