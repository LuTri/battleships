#include "network.hpp"
#include <exception>

Network* Network::INSTANCE = 0;

bool Network::TYPE_SERVER = false;

/* Server / Cient Singletons */

Client* Client::GetClient(void) {
   if (INSTANCE == 0) {
      INSTANCE = new Client();
   }
   if (TYPE_SERVER)
      throw std::logic_error("Already started as Server!");
   return (Client*)INSTANCE;
}

Server::Server(void) {
   TYPE_SERVER = true;
}

Server* Server::GetServer(void) {
   if (INSTANCE == 0) {
      INSTANCE = new Server();
   }
   if (TYPE_SERVER == false)
      throw std::logic_error("Already started as Client!");
   return (Server*)INSTANCE;
}

/* Network Implementation */

Network::Network(void) {
#ifndef LINUX
   _wsa = 0;
#endif
}

#ifndef LINUX
Network::~Network(void) {
   if (_wsa) {
      closesocket(_socket);
      WSACleanup();
      delete _wsa;
   }
}

#else
Network::~Network(void) {
   close(_socket);
}
#endif

bool Network::Startup(void) {
#ifndef LINUX
   _wsa = new WSADATA;
   int iRet = WSAStartup(MAKEWORD(2, 2), _wsa);

   return iRet == 0;
#else
   return true;
#endif
}

#ifndef LINUX
bool Network::MakeSocket(int PROTOCOL, SOCKET* sock) {
#else
bool Network::MakeSocket(int PROTOCOL, int* sock) {
#endif
   *sock = socket(PROTOCOL, SOCK_STREAM, IPPROTO_TCP);
   return *sock != INVALID_SOCKET;
}

int Network::LastError(void) {
#ifndef LINUX
   return WSAGetLastError();
#endif
	return 0;
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
   TYPE_SERVER = false;
}

Client::~Client(void) {
   INSTANCE = 0;
}

bool Client::Connect(string address, int port) {
   if (!started) {
#ifndef LINUX
      if (!(Startup()))
         return false;
#endif

      if (!(MakeSocket(AF_INET, &_socket)))
         return false;


      _server.sin_addr.s_addr = inet_addr(address.c_str());
      _server.sin_family = AF_INET;
      _server.sin_port = htons(port);

      started = true;
   }
   return connect(_socket , (struct sockaddr *)&_server , sizeof(_server)) >= 0;
}

/* Server Implementation */

Server::~Server(void) {
#ifndef LINUX
   closesocket(_listensocket);
#else
   close(_listensocket);
#endif
   INSTANCE = 0;
}

bool Server::AwaitConnection(int port) {
#ifndef LINUX
   int c;
   if (!(Startup()))
      return false;
#else
	socklen_t c;
#endif

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

