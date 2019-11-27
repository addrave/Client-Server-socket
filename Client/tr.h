#include <QThread>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <thread>
#include <QString>

// Для корректной работы freeaddrinfo в MinGW
// Подробнее: http://stackoverflow.com/a/20306451
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>

class MyThread : public QObject
 {
    Q_OBJECT
public:
    SOCKET socketMy;
    bool flag_loop = false;
public slots:
    void run();
signals:
    signal_messageMy(QString str);
 };

