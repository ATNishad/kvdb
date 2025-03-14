#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include<iostream>
#include<winsock.h>



int main(){
    WSADATA wsadata;
    int result;
    result = WSAStartup(MAKEWORD(2,2),&wsadata);
    if(result != 0){
        std::cout<<"wsa startup failed"<<result;
        return 1;
    }
}