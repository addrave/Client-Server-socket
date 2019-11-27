#include "tr2.h"
#include "QString"
using namespace std;
void MyThread2::run()
{
    while (flag_loop)
    {
        int client_socket = accept(socketMy, NULL, NULL);   // socketMy = listen_socket
        std::cout << client_socket << " client socket \n";
        std::cout << socketMy << "\n listen socket \n";
        if (client_socket == INVALID_SOCKET) {
            cerr << "accept failed: " << WSAGetLastError() << "\n";
            closesocket(socketMy);
            WSACleanup();
        }
        emit signal_create_recv_for_new_client_socket(client_socket);
    }
}
