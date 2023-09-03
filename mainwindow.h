#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QHostAddress>
//#include "server.h"
#include "threadHandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pbReady_clicked();

    void incommingConnection();

//    void readData();

    void on_pbRefresh_clicked();

    void on_pbClose_clicked();

    void showMessage(QByteArray ba);

    void destroySocket(QTcpSocket *s);

private:
    QTcpServer *server;

    QList<QTcpSocket*> clientSockets;

    QList<threadHandler*> threadHandlers;

    void sendToAllClients(QByteArray ba);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
