#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>

#define PORT 8000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Server");

    ui->pbClose->setEnabled(false);
    ui->leConnectedAddress->setEnabled(false);
    ui->leConnectedPort->setEnabled(false);
}

MainWindow::~MainWindow()
{
    // 關閉伺服器連線
    server->close();
    server->deleteLater();
    // 釋放連接客戶端的socket
    for(QTcpSocket* clientSocket : server->findChildren<QTcpSocket*>()) {
        clientSocket->close();
        clientSocket->deleteLater();
    }

    delete ui;
}

void MainWindow::on_pbReady_clicked()
{
    QCoreApplication::processEvents();

    ui->pbClose->setEnabled(true);
    ui->teReceiveMessage->append("等待連線..");
    ui->pbReady->setEnabled(false);
    server = new QTcpServer;
    if(server->listen(QHostAddress::AnyIPv4, PORT)) {
        qDebug() << "正在監聽 port:" << PORT;
    }
    else {
        QMessageBox::critical(this, "QTCPServer", QString("Unable to start the server: %1.").arg(server->errorString()));
        exit(EXIT_FAILURE);
    }

    connect(server, &QTcpServer::newConnection, this, &MainWindow::incommingConnection);
}

void MainWindow::incommingConnection() {
    // 建立新的 socket 來處理新連線
    QTcpSocket *socket = server->nextPendingConnection();
    clientSockets.append(socket);
    // 顯示客戶端連接的 IP 位址和端口號在 UI 上
    ui->leConnectedAddress->setText(socket->peerAddress().toString());
    ui->leConnectedPort->setText(QString::number(socket->peerPort()));
    ui->teReceiveMessage->append("建立連線成功");
    // 連接 socket 的 readyRead 信號到 readData 槽函式，以便處理收到的數據
//    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    // 創建線程對象
    threadHandler *t = new threadHandler(socket);
    threadHandlers.append(t);
    // 開始線程
    t->start();
    // 處理接收完客戶端資料訊號
    connect(t, &threadHandler::dataReaded, this, &MainWindow::showMessage);
    // 處理斷線信號
    connect(t, &threadHandler::clientDisconnected, this, &MainWindow::destroySocket);
}

void MainWindow::showMessage(QByteArray ba) {
    ui->teReceiveMessage->append("-> " + QString(ba));
    sendToAllClients(ba);
}

void MainWindow::sendToAllClients(QByteArray ba) {
    for(QTcpSocket *clientSocket : clientSockets) {
        clientSocket->write(ba);
    }
}

void MainWindow::destroySocket(QTcpSocket *s) {
    // 找到並銷毀對應的 threadHandler
    for(int i = 0 ; i < threadHandlers.size() ; i++) {
        if(threadHandlers.at(i)->socket == s) {
            qDebug() << "Destroying threadHandler for disconnected client.";
            threadHandler *t = threadHandlers.takeAt(i);
            t->deleteLater();
            break;
        }
    }
    // 移除斷開連線的 clientSocket
    qDebug() << "Client disconnected. Socket removed and destroyed.";
    clientSockets.removeOne(s);
    s->deleteLater();

    ui->teReceiveMessage->append("客戶端已離線");
}

//void MainWindow::readData() {
//    // 使用讀取函式來處理收到的數據
////    QTcpSocket *socket = (QTcpSocket*)sender();
//    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
//    if (socket) {
//        QByteArray data = socket->readAll();
//        ui->teReceiveMessage->append(QString(data));
//    }
//}

void MainWindow::on_pbRefresh_clicked()
{
    ui->teReceiveMessage->clear();
}

void MainWindow::on_pbClose_clicked()
{
    // 關閉伺服器連線
    server->close();
    server->deleteLater();
    // 釋放連接客戶端的socket
    for(QTcpSocket* clientSocket : server->findChildren<QTcpSocket*>()) {
        clientSocket->close();
        clientSocket->deleteLater();
    }
    ui->teReceiveMessage->append("伺服器已關閉連線");
    ui->pbReady->setEnabled(true);
    ui->pbClose->setEnabled(false);
    ui->leConnectedAddress->clear();
    ui->leConnectedPort->clear();
    clientSockets.clear();
    threadHandlers.clear();
}
