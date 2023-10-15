#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <iostream>
using namespace std;

#define MAX_LEN_BUF 64

int main(int argc, char* argv[])
{
    setbuf(stdout, NULL);

    string adr;
    cout << "Enter network socket name in format \"server:port\": ";
    cin >> adr;
    string::size_type pos = adr.find(':');
    if (pos == string::npos) {
        cerr << "Invalid adress" << endl;
        return -1;
    }
    string server_name = adr.substr(0, pos);
    string port_name = adr.substr(pos + 1);
    int port = stoi(port_name);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    struct hostent* hosten;
    hosten = gethostbyname(server_name.c_str());
    serv_addr.sin_addr.s_addr = ((in_addr*)hosten->h_addr_list[0])->s_addr;
    serv_addr.sin_port = htons(port);

    char buf[MAX_LEN_BUF];
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (socket < 0) {
        cerr << "Error: socket() failed" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error: connect() failed: " << errno << endl;
        return -1;
    }

    if (send(sock, adr.c_str(), adr.size() + 1, 0) < 0) {
        cerr << "Error: sendto() failed" << endl;
        return -1;
    }

    char bufServer[BUFSIZ];
    int bytes = recv(sock, bufServer, sizeof(bufServer), 0);

    cout << "Server response: " << bufServer << endl;

    close(sock);
    return 0;
}