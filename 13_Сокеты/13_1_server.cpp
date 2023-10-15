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

    string filename(argv[0]);
    string::size_type lastDot = filename.rfind('.');
    if (lastDot == string::npos) {
        lastDot = filename.size();
    }
    socketName = filename.substr(0, lastDot) + ".soc";

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        cerr << "socket call error" << endl;
        return errno;
    }
    struct sockaddr_un sock_addr;
    sock_addr.sun_family = AF_UNIX;

    strncpy(sock_addr.sun_path, socketName.c_str(), socketName.size());
    if (bind(sfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) {
        cerr << "bind call error" << endl;
        cleanup_socket(sfd);
        return errno;
    }

    if (signal(SIGINT, sighandler) == SIG_ERR) {
        cerr << "signal call error" << endl;
        cleanup_socket(sfd);
        return errno;
    }

    cout << "Socket name: " << socketName << endl;

    char msg[BUF_SIZE];
    while (true) {
        size_t bytes = recv(sfd, msg, sizeof(msg) - 1, 0);
        if (bytes == -1) {
            cerr << "recv call error" << endl;
            cleanup_socket(sfd);
            return errno;
        }
        msg[bytes] = '\0';
        cout << "Got message: " << msg << endl;
    }
}