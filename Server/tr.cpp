#include "tr.h"
#include "QString"
#include "QDebug"
using namespace std;
void MyThread::run()
{
    const int max_client_buffer_size = 1024;
    char buf[max_client_buffer_size];
    while (flag_loop)
    {
        int result = recv(socketMy, buf, max_client_buffer_size, 0);
        cout << "\nresultt " << result << " socket=" << socketMy << buf << endl;
        if (result > 0)
        {   QString str;
            for (int i=0; i<1024; i++)
            {
                if (buf[i] == '\0')
                    break;
                else
                    str.push_back(buf[i]);
            }
            cout << str.toStdString();
            emit this->signal_messageMy(str, socketMy);
        }
        if (result == -1)
        {
            qDebug() << "connection close " << socketMy;
            flag_loop = false;
            emit this->signal_connection_close(socketMy);
        }
    }

}
