#include "tds_client.h"
namespace TDS
{
    static std::string tds_easy_escape(CURL *curl, std::string url)
    {
        std::string cpp_encoded_url;
        char *encoded_url = curl_easy_escape(curl, url.c_str(), url.length()); 
        cpp_encoded_url.assign(encoded_url);
        if(encoded_url)
            curl_free(encoded_url);
        return cpp_encoded_url;
    }
    TCPClient::TCPClient()
    {
        _state = std::shared_ptr<NetState>(new TCPClientClosed());
    }
    TCPClient::~TCPClient()
    {
        close();
    }
    bool TCPClient::open(std::string server_ip, int server_port)
    {
        _ip   = server_ip;
        _port = server_port;
        return _state->open(this);
    }
    void TCPClient::close()
    {
        _state->close(this);
    }
    void TCPClientEstablished::close(NetConnection* connection)
    {
        TCPClient *client = dynamic_cast<TCPClient*>(connection);
        ::close(client->_hostfd);
    }
    bool TCPClientEstablished::send (NetConnection *connection, EncodeParameter *parameter)
    {
        bool ret = true;
        try
        {
            int len, sent_bytes = 0;
            TCPClient *client = dynamic_cast<TCPClient*>(connection);
            std::shared_ptr<uint8_t> bytes = parameter->toBytes();
            while(sent_bytes < PACKET_LENGTH::OF_TOTAL)
            {
                len = ::send(client->_hostfd , reinterpret_cast<char*>(bytes.get()),
                    PACKET_LENGTH::OF_TOTAL, 0);
                if(len <= 0)
                    throw std::system_error(std::error_code(errno, std::generic_category()));
                sent_bytes += len;
            }
        }
        catch(std::exception &e)
        {
            std::cerr << "Caught an exception in function TCPClientEstablished::send()"
                      <<"\ne.what():" << e.what() << std::endl;
            ret = false;
        }
        return ret;
    }
    bool TCPClientClosed::open(TDS::NetConnection *connection)
    {
        bool ret = true;
        try
        {
            TCPClient *client = dynamic_cast<TCPClient*>(connection);
            client->_hostfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(client->_hostfd == -1)
                throw std::system_error(std::error_code(errno, std::generic_category()));
            client->_hostaddr.sin_addr.s_addr = inet_addr(client->_ip.c_str());
            client->_hostaddr.sin_port        = htons(client->_port);
            client->_hostaddr.sin_family      = AF_INET;
            if(::connect(client->_hostfd, (struct sockaddr*)&client->_hostaddr,
                sizeof(client->_hostaddr)) == -1)
                throw std::system_error(std::error_code(errno, std::generic_category()));
            client->changeState(std::shared_ptr<NetState>(new TCPClientEstablished()));
        }
        catch(std::exception &e)
        {
            std::cerr << "Caught an exception in function TCPClientClosed::open()"
                      <<"\ne.what():" << e.what() << std::endl;
            ret = false;
        }
        return ret;
    }

    HTTPClient::HTTPClient()
    {
        _state = std::shared_ptr<NetState>(new HTTPClientClosed());
    }

    HTTPClient::~HTTPClient()
    {
        close();
    }

    bool HTTPClient::open(std::string server_ip, int server_port) 
    {
        _ip   = server_ip;
        _port = server_port;
        return _state->open(this);
    }

    bool HTTPClient::open(std::string url) 
    {
        _url = url;   
        return _state->open(this);
    }

    void HTTPClient::close() 
    {
        _state->close(this);
    }

    std::string HTTPClient::getHtmlContent() const
    {
        return _htmlcontent;
    }

    void HTTPClientEstablished::close(NetConnection* connection)
    {
        HTTPClient *client = dynamic_cast<HTTPClient*>(connection);
        curl_easy_cleanup(client->curl);
        client->changeState(std::shared_ptr<TDS::NetState>(new HTTPClientClosed()));
    }

    bool HTTPClientEstablished::send(NetConnection *connection, EncodeParameter *parameter)
    {
        HTTPClient *client = dynamic_cast<HTTPClient*>(connection);
        std::string getdata = tds_easy_escape(client->curl, parameter->toJson());
        std::string url = client->_url + getdata;
        client->_htmlcontent.clear();
        curl_easy_setopt(client->curl, CURLOPT_URL, url.c_str()); 
        curl_easy_setopt(client->curl, CURLOPT_HTTPGET, 1); 
        curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, &client->_htmlcontent); 
        curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, curlWriteback);
        return curl_easy_perform(client->curl) == CURLE_OK; 
    }

    size_t HTTPClientEstablished::curlWriteback(void *content, size_t elementSize, size_t nbElement, void *userData)
    {
        size_t len = elementSize * nbElement;
        std::string *s = reinterpret_cast<std::string*>(userData);
        s->append(reinterpret_cast<char*>(content), len); 
        return s->length(); 
    }

    bool HTTPClientClosed::open(NetConnection *connection)
    {
        bool ret = false;
        HTTPClient *client = dynamic_cast<HTTPClient*>(connection);
        if(!client->_ip.empty())
        {
            std::string url;
            std::string const &ip = client->_ip;
            int const &port = client->_port;
            client->_url = "http://" + ip + ":" + std::to_string(port) + "/myapi?videoinformation=";
            client->curl = curl_easy_init();
            ret = client->curl != NULL;
        }
        else if(!client->_url.empty())
        {
            client->curl = curl_easy_init();
            client->_url += "/myapi?videoinformation=";
            ret = client->curl != NULL;
        }
        if(ret)
            client->changeState(std::shared_ptr<TDS::NetState>(new HTTPClientEstablished()));
        return ret;
    }
}

