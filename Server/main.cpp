#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <QThread>

// Для корректной работы freeaddrinfo в MinGW
// Подробнее: http://stackoverflow.com/a/20306451
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>


// Необходимо, чтобы линковка происходила с DLL-библиотекой
// Для работы с сокетам
#pragma comment(lib, "Ws2_32.lib")

using std::cerr;
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.run();
    w.show();

    return a.exec();
}
