#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDebug>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <QThread>
#include "tr.h"
#include "tr2.h"
#include <vector>
// Для корректной работы freeaddrinfo в MinGW
// Подробнее: http://stackoverflow.com/a/20306451
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>


// Необходимо, чтобы линковка происходила с DLL-библиотекой
// Для работы с сокетам
#pragma comment(lib, "Ws2_32.lib")

class ThreadSock
{
public:
       QThread *thread;
       MyThread *my;
       int socket;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::vector<int> client_sockets;
    QVector <ThreadSock> vecThreadSock;
    QVector <QPair<QString, int>> vecComments;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void run();
    void send_comment_to_socket(int arg);
    void try_to_send_comment();
    void timerEvent(QTimerEvent *event) override;
public slots:
    void slot_messageMy(QString str, int socket);
    void slot_create_recv_for_new_client_socket(int arg);
    void slot_connection_close(int arg);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
