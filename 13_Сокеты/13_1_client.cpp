#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <cstdio>
#include <unistd.h>
#include <string>

using namespace std;

static int sfd;
static string socketName;

#define BUF_SIZE 256


void cleanup_socket(int sockfd)
{
    close(sockfd);
    unlink(socketName.c_str());
}

void sighandler(int signum)
{
    cleanup_socket(sfd);
    exit(0);
}

int main(int argc, char* argv[]) {

    cout << "Enter name of filesocket: ";
    cin >> socketName;

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        cerr << "socket call error" << endl;
        return errno;
    }
    struct sockaddr_un sock_addr;
    sock_addr.sun_family = AF_UNIX;

    strncpy(sock_addr.sun_path, socketName.c_str(), socketName.size());
    if (connect(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
    {
        perror("Can't set dest address");
        return errno;
    }

    if (send(sfd, socketName.c_str(), socketName.size(), 0) == -1)
    {
        cerr << "send call error" << endl;
        return errno;
    }
    else
    {
        cout << "Sending socket name" << endl;
    }

    close(sfd);

    return 0;
}