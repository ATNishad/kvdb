#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>


#define DEFAULT_PORT "6969"
#define DEFAULT_BUFFER_LEN 512   //might wanna change buffer size later


int main(){
    WSADATA wsadata;
    int wsaResult;
    
    wsaResult = WSAStartup(MAKEWORD(2,2),&wsadata);
    if(wsaResult != 0){
        std::cout<<"wsa startup failed\n"<<wsaResult;
        return 1;
    }
    std::cout<<"wsa startup successful!\n";
    
    
    struct addrinfo *result =  NULL, *ptr = NULL, hints;
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

    SOCKET listenSocket = INVALID_SOCKET;
    listenSocket = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
    if(listenSocket == INVALID_SOCKET){
        std::cout<<"error at socket-listen function "<<WSAGetLastError()<<"\n";
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    std::cout<<"listen socket successful!\n";

    int bindResult = bind(listenSocket,result->ai_addr,(int)result->ai_addrlen);
       if(bindResult == SOCKET_ERROR){
        std::cout<<"error at bind function "<<WSAGetLastError()<<"\n";
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    std::cout<<"bind successful !\n";

    //todo -- accept multiple client connection 
    //checkout select or WSAPoll
    //--prefer async non blocking io over multithreading
    SOCKET clientSocket = INVALID_SOCKET;

    while(clientSocket == INVALID_SOCKET){
    clientSocket  = accept(listenSocket,NULL,NULL);
    if(clientSocket == INVALID_SOCKET){
        std::cout<<"error at connection function "<<WSAGetLastError();
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
}
    std::cout<<"accept successful !\n";

    //remove this when handling multiple clients
    closesocket(listenSocket);    


    char receiveBuffer[DEFAULT_BUFFER_LEN];
    int recvResult, sendResult;
    int recvbuflen = DEFAULT_BUFFER_LEN;

    do{
        recvResult = recv(clientSocket, receiveBuffer, recvbuflen, 0);
        if(recvResult > 0){
            std::cout<<"bytes received: "<<recvResult;

        sendResult = send(clientSocket,receiveBuffer,recvResult,0);
        if(sendResult == SOCKET_ERROR){
            std::cout<<"send function failed "<<WSAGetLastError();
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        std::cout<<"bytes sent: "<<sendResult<<'\n';
        }
        else if(recvResult == 0){
            std::cout<<"connection closing...\n";
        }
        else{
            std::cout<<"error at recv function "<<WSAGetLastError();
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

}while(recvResult > 0);

    recvResult = shutdown(clientSocket,SD_BOTH);
    if(recvResult == SOCKET_ERROR){
        std::cout<<"error at shutdown\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout<<"shutting down...\n";

}