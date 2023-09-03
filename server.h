#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTcpServer>
#include <QCoreApplication>
#include <QObject>
#include <QMessageBox>

class server : public QTcpServer
{
    Q_OBJECT
public:
    server();

    QString clientAddress;
    QString clientPort;
private:
    QTcpServer *Server;
    QTcpSocket *Socket;
//    void newClientHandler();
private slots:
    void newClientHandler();

};



#endif // SERVER_H
