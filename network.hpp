#ifndef _NETWORK_HPP
#define _NETWORK_HPP

#ifndef LINUX
#include <WinSock2.h>
#else

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include <string>

using namespace std;

class Network {
#ifndef LINUX
private:
   WSADATA *_wsa;
#endif

protected:

   Network(void);

   static Network* INSTANCE;
   struct sockaddr_in _server;

   static bool TYPE_SERVER;

#ifndef LINUX
   SOCKET _socket;
#else
	int _socket;
#endif

   bool Startup(void);
#ifndef LINUX
   bool MakeSocket(int PROTOCOL, SOCKET* sock);
#else
   bool MakeSocket(int PROTICOL, int* sock);
#endif

public:
   ~Network(void);

   int LastError(void);

   bool Send(string data);
   bool Receive(string& data);
};

class Client: public Network {
private:

   bool started;

   Client(void);
public:
   static Client* GetClient(void);
   bool Connect(string address, int port);

   ~Client(void);
};

class Server: public Network {
private:
   struct sockaddr_in _client;
#ifndef LINUX
   SOCKET _listensocket;
#else
   int _listensocket;
#endif

   Server(void);
public:
   static Server* GetServer(void);
   bool AwaitConnection(int port);

   ~Server(void);
};

#endif
