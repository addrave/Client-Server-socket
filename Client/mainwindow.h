#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <thread>

// Для корректной работы freeaddrinfo в MinGW
// Подробнее: http://stackoverflow.com/a/20306451
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "tr.h"
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int old_socket;
    int result;
    SOCKET listen_socket;
    MyThread *my = NULL;
    QThread *thread = NULL;
    struct addrinfo* addr = NULL;
    struct addrinfo hints;


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void recv_msg();
private:
    Ui::MainWindow *ui;
private slots:
    void slot_messageMy(QString str);
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
};

#endif // MAINWINDOW_H
