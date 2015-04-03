#include "network.hpp"

Server Server::INSTANCE;

Client Client::INSTANCE;

/* Server / Cient Singletons */

Client::Client(void) {}
Client& Client::GetClient(void) {
   return INSTANCE;
}

Server::Server(void) {}
Server& Server::GetServer(void) {
   return INSTANCE;
}

/* Network Implementation */

Network::~Network(void) {
   if (_wsa) {
      closesocket(_socket);
      WSACleanup();
      delete _wsa;
   }
}

bool Network::Startup(void) {
   _wsa = new WSADATA;
   int iRet = WSAStartup(MAKEWORD(2, 2), _wsa);

   return iRet == 0;
}

bool Network::MakeSocket(int PROTOCOL, SOCKET* sock) {
   *sock = socket(PROTOCOL, SOCK_STREAM, IPPROTO_TCP);
   return *sock != INVALID_SOCKET;
}

int Network::LastError(void) {
   return WSAGetLastError();
}

bool Network::Send(string data) {
   return send(_socket , data.c_str() , data.length(), 0) >= 0;
}

bool Network::Receive(string& data) {
   int received;
   char buff[2000];
   if((received = recv(_socket, buff, 2000, 0)) == SOCKET_ERROR) {
      data = "";
      return false;
   }
    
   buff[received] = '\0';

   data = string(buff);
   return true;
}

/* Client Implementation */

Client::Client(void) {
   started = false;
}

bool Client::Connect(string address, int port) {
   if (!started) {
      if (!(Startup()))
         return false;

      if (!(MakeSocket(AF_INET, &_socket)))
         return false;

      started = true;
   }

   _server.sin_addr.s_addr = inet_addr(address.c_str());
   _server.sin_family = AF_INET;
   _server.sin_port = htons(port);

   return connect(_socket , (struct sockaddr *)&_server , sizeof(_server)) >= 0;
}

/* Server Implementation */

Server::~Server(void) {
   closesocket(_listensocket);
}

bool Server::AwaitConnection(int port) {
   int c;
   if (!(Startup()))
      return false;

   if (!(MakeSocket(PF_INET, &_listensocket)))
      return false;

   _server.sin_family = AF_INET;
   _server.sin_addr.s_addr = INADDR_ANY;
   _server.sin_port = htons(port);
   if(bind(_listensocket ,(struct sockaddr *)&_server, sizeof(_server)) == SOCKET_ERROR)
      return false;

   listen(_listensocket , 3);
   c = sizeof(struct sockaddr_in);
   _socket = accept(_listensocket , (struct sockaddr *)&_client, &c);

   if (_socket == INVALID_SOCKET)
      return false;

   return true;
}

