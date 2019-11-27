#include "tr.h"
#include "QString"
using namespace std;
void MyThread::run()
{
    const int max_client_buffer_size = 1024;
    char buf[max_client_buffer_size];
    while (flag_loop)
    {
        int result = recv(socketMy, buf, max_client_buffer_size, 0);
        if (result > 0)
        {   QString str;
            for (int i=0; i<1024; i++)
            {
                if (buf[i] == '\0')
                    break;
                else
                    str.push_back(buf[i]);
            }
            emit signal_messageMy(str);
        }
    }
}

