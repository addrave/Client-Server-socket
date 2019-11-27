#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <windows.h>
#include <QThread>
// Для корректной работы freeaddrinfo в MinGW
// Подробнее: http://stackoverflow.com/a/20306451
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <QApplication>
// Необходимо, чтобы линковка происходила с DLL-библиотекой
// Для работы с сокетам
#pragma comment(lib, "Ws2_32.lib")

using std::cerr;
using namespace std;
static char buf2[1024];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    WSADATA wsaData; // служебная структура для хранение информации
    // о реализации Windows Sockets
    // старт использования библиотеки сокетов процессом
    // (подгружается Ws2_32.dll)
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << "\n";
        ;
    }

//    struct addrinfo* addr = NULL; // структура, хранящая информацию
    // об IP-адресе  слущающего сокета

    // Шаблон для инициализации структуры адреса
//    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET; // AF_INET определяет, что будет
    // использоваться сеть для работы с сокетом
    hints.ai_socktype = SOCK_STREAM; // Задаем потоковый тип сокета
    hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
//    hints.ai_flags = AI_PASSIVE; // Сокет будет биндиться на адрес,
    // чтобы принимать входящие соединения

    // Инициализируем структуру, хранящую адрес сокета - addr
    // Наш HTTP-сервер будет висеть на 8000-м порту локалхоста
    result = getaddrinfo("localhost", "8008", &hints, &addr);

    // Если инициализация структуры адреса завершилась с ошибкой,
    // выведем сообщением об этом и завершим выполнение программы
    if (result != 0) {
        cerr << "getaddrinfo failed: " << result << "\n";
        WSACleanup(); // выгрузка библиотеки Ws2_32.dll
        ;
    }

    // Создание сокета
     listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);

     QPalette palette = ui->label->palette();
     palette.setColor(QPalette::Window, Qt::white);
     ui->label->setAutoFillBackground(true);
     ui->label->setPalette(palette);

     ::connect(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);
     recv_msg();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recv_msg()
{
    if (my != nullptr)
    {
//        closesocket(old_socket);
        thread->terminate();
        thread->wait();
        delete my;
        delete thread;
        cout << "\n delete thread; \n";
    }
    cout << "\n thread = new QThread; \n";
    thread = new QThread;
    my = new MyThread;
    my->socketMy = listen_socket;
    my->flag_loop = true;
    my->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), my, SLOT(run()));
    QObject::connect(my, SIGNAL(signal_messageMy(QString)), this, SLOT(slot_messageMy(QString)));
    thread->start();
}

void MainWindow::slot_messageMy(QString str)
{
    ui->label->setText(str);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->label->clear();
    std::stringstream response2;
    response2 << "Accept" << '\0';
    result = send(listen_socket, response2.str().c_str(),
                  response2.str().length(), 0);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->label->clear();
    std::stringstream response2;
    response2 << "Deny" << '\0';
    result = send(listen_socket, response2.str().c_str(),
                  response2.str().length(), 0);
}
