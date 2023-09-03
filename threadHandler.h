#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QByteArray>

class threadHandler : public QThread
{
    Q_OBJECT;
public:
    threadHandler(QTcpSocket *s);

    QTcpSocket *socket;

    void run();

private:

private slots:
    void readSocket();

signals:
    void dataReaded(QByteArray ba);

    void clientDisconnected(QTcpSocket *clientSocket);
};

#endif // THREAD_H
