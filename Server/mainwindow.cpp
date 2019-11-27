#include "mainwindow.h"
#include "ui_mainwindow.h"
using std::cerr;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    QStringList labelsHeader;
    labelsHeader << "Mod" << "sock" << "time";
    ui->tableWidget->setHorizontalHeaderLabels(labelsHeader);
    startTimer(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::run()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << "\n";
//        return result;
    }

    struct addrinfo* addr = NULL; // структура, хранящая информацию
    // об IP-адресе  слущающего сокета

    // Шаблон для инициализации структуры адреса
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET; // AF_INET определяет, что будет
    // использоваться сеть для работы с сокетом
    hints.ai_socktype = SOCK_STREAM; // Задаем потоковый тип сокета
    hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
    hints.ai_flags = AI_PASSIVE; // Сокет будет биндиться на адрес,
    // чтобы принимать входящие соединения

    // Инициализируем структуру, хранящую адрес сокета - addr
    // Наш HTTP-сервер будет висеть на 8000-м порту локалхоста
    result = getaddrinfo("localhost", "8008", &hints, &addr);

    // Если инициализация структуры адреса завершилась с ошибкой,
    // выведем сообщением об этом и завершим выполнение программы
    if (result != 0) {
        cerr << "getaddrinfo failed: " << result << "\n";
        WSACleanup(); // выгрузка библиотеки Ws2_32.dll
//        return 1;
    }

    // Создание сокета
    int listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        cerr << "Error at socket: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        WSACleanup();
//        return 1;
    }

    // Привязываем сокет к IP-адресу
    result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    if (result == SOCKET_ERROR) {
        cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        closesocket(listen_socket);
        WSACleanup();
//        return 1;
    }

    // Инициализируем слушающий сокет
    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(listen_socket);
        WSACleanup();
//        return 1;
    }

    QThread *thread = new QThread;
    MyThread2 *my = new MyThread2;
    my->socketMy = listen_socket;
    my->flag_loop = true;
    my->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), my, SLOT(run()));
    QObject::connect(my, SIGNAL(signal_create_recv_for_new_client_socket(int)), this, SLOT(slot_create_recv_for_new_client_socket(int)));
    thread->start();
}

void MainWindow::slot_messageMy(QString str, int socket)
{
    qDebug() << str;
    bool comm_for_socket = false;
    for (int i=0; i<vecComments.size(); i++)
    {
        if (vecComments.at(i).second == socket)
        {
            comm_for_socket = true;
            QString tempOutput;
            if (str == "Deny"){
                tempOutput.push_back("<font color='red'>");
                tempOutput.push_back('"' + vecComments.at(i).first + '"' + ' ' + str + " by " + QString::fromStdString(to_string(socket)));
                tempOutput.push_back("</font>");
            }
            else{
                tempOutput.push_back('"' + vecComments.at(i).first + '"' + ' ' + str + " by " + QString::fromStdString(to_string(socket)));
            }
            ui->textBrowser->append(tempOutput);

            vecComments.removeAt(i);
            qDebug() << vecComments << "slot_messageMy";
            for (int i=0; i < ui->tableWidget->rowCount(); i++)
            {
                if (ui->tableWidget->item(i, 1)->text().toInt() == socket)
                {
                    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("")));
                    ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString("60")));
                    try_to_send_comment();
                }
            }
            break;
        }
    }
    if (comm_for_socket == false)
    {
        for (int i=0; i < ui->tableWidget->rowCount(); i++)
        {
            if (ui->tableWidget->item(i, 1)->text().toInt() == socket)
            {
                if (ui->tableWidget->item(i, 0) != nullptr && ui->tableWidget->item(i, 0)->text() == "send" && ui->tableWidget->item(i, 2)->text().toInt() == -1)
                {
                    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("")));
                    ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString("60")));
                    try_to_send_comment();
                    break;
                }

            }
        }
    }
    // если пришло сообщение от сокета на который не отправлены комментарии, а они есть, счетчик в 60
}

void MainWindow::slot_create_recv_for_new_client_socket(int arg)
{
    client_sockets.push_back(arg);
//    cout << "TEST MY LONGNAME SLOT";
    QThread *thread = new QThread;
    MyThread *my = new MyThread;
    my->socketMy = arg;

    ThreadSock threadSock;
    threadSock.thread = thread;
    threadSock.my = my;
    threadSock.socket = arg;
    vecThreadSock.push_back(threadSock);

    my->flag_loop = true;
    my->moveToThread(thread);
    int rowVal = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowVal);
    ui->tableWidget->setItem(rowVal, 0, new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(rowVal, 1, new QTableWidgetItem(QString::number(arg)));
    ui->tableWidget->setItem(rowVal, 2, new QTableWidgetItem(QString::fromStdString("60")));
    QObject::connect(thread, SIGNAL(started()), my, SLOT(run()));
    QObject::connect(my, SIGNAL(signal_messageMy(QString, int)), this, SLOT(slot_messageMy(QString, int)));
    QObject::connect(my, SIGNAL(signal_connection_close(int)), this, SLOT(slot_connection_close(int)));
    thread->start();
    try_to_send_comment();
}
void MainWindow::timerEvent(QTimerEvent *event)
{
    qDebug() << vecComments;
    for (int i=0; i < ui->tableWidget->rowCount(); i++)
    {
        int myTime = ui->tableWidget->item(i, 2)->text().toInt();
        if (myTime > -1)
        {
            if (ui->tableWidget->item(i, 0) != nullptr && ui->tableWidget->item(i, 0)->text() != "")
            {
                myTime = myTime - 1;
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(myTime)));
                if (myTime == -1)
                {
                    int socket = ui->tableWidget->item(i, 1)->text().toInt();
                    for (int j=0; j < vecComments.size(); j++)
                    {
                        if (vecComments.at(j).second == socket)
                        {
                            vecComments[j].second = 0;
                        }
                    }

                    try_to_send_comment();
                }
            }
        }
    }
}

void MainWindow::slot_connection_close(int arg)
{
    cout << "test";
    for (int i=0; i < ui->tableWidget->rowCount(); i++)
    {
        if (ui->tableWidget->item(i, 1)->text().toInt() == arg)
            ui->tableWidget->removeRow(i);
    }

    for (int i=0; i < vecThreadSock.size(); i++)
    {
        if (vecThreadSock.at(i).socket == arg)
        {
            closesocket(vecThreadSock.at(i).socket);
            vecThreadSock.at(i).thread->terminate();
            vecThreadSock.at(i).thread->wait();
            delete vecThreadSock.at(i).thread;
            delete vecThreadSock.at(i).my;
            vecThreadSock.remove(i);
            qDebug() << "delete Thread whith socket " << arg;
            break;
        }
    }
    for (int i=0; i < vecComments.size(); i++)
    {
        if (vecComments.at(i).second == arg)
            vecComments[i].second = 0;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QPair<QString, int> comments;
    comments.first = ui->lineEdit->text();
    comments.second = 0;
    vecComments.push_back(comments);
    qDebug() << vecComments;
    try_to_send_comment();
}

void MainWindow::send_comment_to_socket(int arg)
{
    bool flag = false;
    for (int i=0; i < vecComments.size(); i++)
    {
        if (vecComments.at(i).second == 0)
        {
            vecComments[i].second = arg;
            std::stringstream response;
            QString strCom = vecComments.at(i).first;
            response << strCom.toStdString();
            response << '\0';
            int result = send(arg, response.str().c_str(),
                          response.str().length(), 0);
            qDebug() << vecComments;
            flag = true;
            break;
        }
    }
    if (flag)
    {
        for (int i=0; i < ui->tableWidget->rowCount(); i++)
        {
            if (ui->tableWidget->item(i, 1)->text().toInt() == arg)
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("send")));
        }
    }
}

void MainWindow::try_to_send_comment()
{
    bool openMod = false; // проверяем свободные коментарии
    int mod;
    bool openComments = false; //проверяем свободных модераторов
    int comm;
    for (int i=0; i < vecComments.size(); i++)
    {
        if (vecComments.at(i).second == 0)
        {
            openComments = true;
            comm = i;
            break;
        }
    }
    for (int i=0; i < ui->tableWidget->rowCount(); i++)
    {
        if (ui->tableWidget->item(i, 0) != nullptr && ui->tableWidget->item(i, 0)->text() == "")
        {
            openMod = true;
            mod = i;
        }
    }
    if (openMod && openComments) // отправляем свободный коментарий свободному модератору
    {
        int socket = ui->tableWidget->item(mod, 1)->text().toInt();
        vecComments[comm].second = socket;

        std::stringstream response;
        QString strCom = vecComments.at(comm).first;
        response << strCom.toStdString();
        response << '\0';
        int result = send(socket, response.str().c_str(),
                      response.str().length(), 0);

        ui->tableWidget->setItem(mod, 0, new QTableWidgetItem(QString("send")));

        try_to_send_comment();
    }
}
