#ifndef TDS_SERVER_H
#define TDS_SERVER_H
#include "tds_connection.h"
namespace TDS
{
    class TCPServer : public NetConnection
    {
    public:
        TCPServer();
        ~TCPServer();
        bool open(std::string host_ip, int host_port = -1) override;
        void close() override;
        int connections() const;
        /*
            This is a blocking function, the one will block thread
            until there exists at least one parameter.
        */
        std::shared_ptr<TDS::EncodeParameter> popParameter();
    private:
        friend class TCPServerEstablished;
        friend class TCPServerServed;
        friend class TCPServerClosed;
        struct sockaddr_in _hostaddr;
        int _hostfd;
        int _port;
        std::string _ip;
        std::set<int> _clientfds;
        std::condition_variable _queueCond;
        std::mutex _syncMtx;
        std::queue<std::shared_ptr<EncodeParameter>> _queueParameter;
    };

    class TCPServerEstablished : public NetEstablished
    {
    public:
        void close(NetConnection*) override;
        bool recv (NetConnection *connection, EncodeParameter *parameter) override;
        bool send (NetConnection *connection, EncodeParameter *parameter) override;
    };

    class TCPServerServed : public NetState
    {
    public:
        TCPServerServed(TCPServer*);
        void close(NetConnection*) override;
        bool recv (NetConnection *connection, EncodeParameter *parameter) override;
        bool send (NetConnection *connection, EncodeParameter *parameter) override;
    private:
        void serviceThread();
        EncodeParameter* receiveAnEncodeParameters(int fd);
        void addEncodeParameterToQueue(EncodeParameter *p);
        bool _quit;
        TCPServer *_server;
        std::thread *_acceptThread;
    };

    class TCPServerClosed : public NetClosed
    {
    public:
        bool open (NetConnection*) override;
    };
}
#endif
