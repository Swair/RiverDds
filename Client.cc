#include <stdio.h>
#include "Socket.h"
#include "Common.h"
#include "LogWrite.h"



#define PROGRAM "TEST"
#define node "test"
#define topic "odom"


int main(int argc,char*argv[])
{   
    auto set = [](int sfd)->void{
        srand(time(0));
        int randnum = rand() % 10;
        Fson request;
        request.setStr("method", "SET");
        request.setStr("node", node);
        request.setStr("topic", topic);
        request.setInt("value", randnum);
        sendRequest(sfd, request);
        request.print();

        Fson response;
        recvRequest(sfd, response);
        response.getInt("code"); 
        response.print(); 
    };

    auto get = [](int sfd)->void{
        Fson request;
        request.setStr("method", "GET");
        request.setStr("node", node);
        request.setStr("topic", topic);
        request.print();
        sendRequest(sfd, request);

        Fson response;
        recvRequest(sfd, response);
        int code = response.getInt("code"); 
        response.print(); 
        if(code != 200)
            throw "-1";
        int ret = response.getInt("value");
        logWrite("ret = %d\n", ret);
    };


    nbSendToServer("127.0.0.1", "12345", set);
    nbSendToServer("127.0.0.1", "12345", get);

    return 0;
}


















