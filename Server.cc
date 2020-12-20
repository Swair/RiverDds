#include <map>
#include "Server.h"
#include "LogWrite.h"
#include "Common.h"
#include "RiverDds.h"


class StateServer
{
    public:

        static StateServer& getInstance()
        {
            static StateServer stateServer;
            return stateServer;
        }

        void startRun(const std::string& ip, const std::string& port)
        {
            auto userHandler = [&](int cfd)->int {
                Fson request;
                Fson response;

                auto work = [&]()->void{
                    std::string method;
                    int res = 0;
                    try
                    {
                        method = request.getStr("method");
                    }
                    catch(int ret)
                    {
                        res = ret;
                    }

                    if(res == 0)
                    {
                        std::string nName = request.getStr("node");
                        std::string tName = request.getStr("topic");
                        Topic* topic = riverDds_.open(nName, tName);
                        if(0 == method.compare("SET"))
                        {
                            std::string value = request.getMem("value");
                            topic->mem.push(value);
                            request.print();
                            response.setInt("code", 200);
                        }
                        else if(0 == method.compare("GET"))
                        {
                            response.setInt("code", 200);
                            std::string value = topic->mem.front();
                            response.setMem("value", value);
                            response.print();
                            topic->mem.pop();
                        }
                    }
                    else
                    {
                        response.setInt("code", 400);
                    } 
                };

                int res = recvRequest(cfd, request);
                if(res <= 0)
                    return SOCKET_EXCEPTION;

                work();
                res = sendRequest(cfd, response);
                if(res < 0)
                {
                    logWrite("reply fail\n");
                }

                return SOCKET_CLOSE;
            };

            Reactor reactor(10);
            reactor.runEpollServer(ip.c_str(), port.c_str(), userHandler);
        }


    private:
        RiverDds riverDds_;
};



int main(int argc,char*argv[])
{
    std::string ip = "127.0.0.1";
    std::string port = "12345";
    StateServer::getInstance().startRun(ip, port);
    return 0;
}




