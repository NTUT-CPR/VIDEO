#ifndef TDS_CLIENT_H
#define TDS_ClIENT_H
#include "tds_connection.h"
#include <curl/curl.h>
namespace TDS
{
    class TCPClient : public NetConnection
    {
    public:
        TCPClient();
        ~TCPClient();
        bool open(std::string server_ip, int server_port) override;
        void close() override;
    private:
        friend class TCPClientEstablished;
        friend class TCPClientClosed;
        struct sockaddr_in _hostaddr;
        int _hostfd;
        int _port;
        std::string _ip;
    };
    
    class TCPClientEstablished : public NetEstablished
    {
    public:
        void close(NetConnection*) override;
        bool send(NetConnection *connection, EncodeParameter *parameter) override;
    };

    class TCPClientClosed : public NetClosed
    {
    public:
        bool open(NetConnection*);
    };

    class HTTPClient : public NetConnection
    {
    public:
        HTTPClient();
        ~HTTPClient();
        bool open(std::string server_ip, int server_port) override;
        bool open(std::string url) override;
        void close() override;
        std::string getHtmlContent() const;
    private:
        friend class HTTPClientEstablished;
        friend class HTTPClientClosed;
        CURL *curl;
        std::string _url;
        std::string _ip;
        std::string _htmlcontent;
        int _port;
    };

    class HTTPClientEstablished : public NetEstablished
    {
    public:
        void close(NetConnection*) override;
        bool send(NetConnection *connnection, EncodeParameter *parameter) override; 
    private:
        static size_t curlWriteback(void *content, size_t elementSize, size_t nbElement, void *userData);
    };

    class HTTPClientClosed : public NetEstablished
    {
    public:
        bool open(NetConnection *connection) override;
    };
}
#endif
