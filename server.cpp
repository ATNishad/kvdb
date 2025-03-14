#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>


#define DEFAULT_PORT "6969"



int main(){
    WSADATA wsadata;
    int wsaResult;
    struct addrinfo *result =  NULL, *ptr = NULL, hints;
    
    wsaResult = WSAStartup(MAKEWORD(2,2),&wsadata);
    if(wsaResult != 0){
        std::cout<<"wsa startup failed\n"<<wsaResult;
        return 1;
    }
    std::cout<<"wsa startup successful!\n";
    

    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int addrinfoResult = getaddrinfo(NULL,DEFAULT_PORT,&hints,&result);
    if(addrinfoResult != 0){
        std::cout<<"getaddrinfo failed, "<<WSAGetLastError()<<"\n";
    }
    std::cout<<"getaddrinfo successful!\n";
    SOCKET listensocket = INVALID_SOCKET;
    listensocket = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
    if(listensocket == INVALID_SOCKET){
        std::cout<<"error at socket function "<<WSAGetLastError()<<"\n";
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    int bindResult = bind(listensocket,result->ai_addr,(int)result->ai_addrlen);
       if(bindResult == SOCKET_ERROR){
        std::cout<<"error at bind function "<<WSAGetLastError()<<"\n";
        freeaddrinfo(result);
        closesocket(listensocket);
        WSACleanup();
        return 1;
    }


}