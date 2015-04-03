#ifndef _NETWORK_HPP
#define _NETWORK_HPP
#include <WinSock2.h>
#include <string>

using namespace std;

class Network {
private:
   WSADATA *_wsa;
protected:
   struct sockaddr_in _server;
   SOCKET _socket;

   ~Network(void);

   bool MakeSocket(int PROTOCOL, SOCKET* sock);
   bool Startup(void);

public:
   int LastError(void);

   bool Send(string data);
   bool Receive(string& data);
};

class Client: public Network {
private:

   bool started;

   Client(void);
   static Client INSTANCE;
public:
   static Client& GetClient(void);
   bool Connect(string address, int port);
};

class Server: public Network {
private:
   struct sockaddr_in _client;
   SOCKET _listensocket;

   Server(void);
   static Server INSTANCE;
public:
   static Server& GetServer(void);
   bool AwaitConnection(int port);

   ~Server(void);
};

#endif
