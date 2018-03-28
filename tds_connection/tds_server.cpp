#include "tds_server.h"
namespace TDS
{
    TCPServer::TCPServer()
        : _hostfd(0), _port(0)
    {
        _state = std::shared_ptr<NetState>(new TCPServerClosed());
    }
    TCPServer::~TCPServer()
    {
        close();
    }
    bool TCPServer::open(std::string ip, int port)
    {
        _ip   = ip;
        _port = port;
        return _state->open(this);
    }
    void TCPServer::close()
    {
        return _state->close(this);
    }
    int TCPServer::connections() const
    {
        return _clientfds.size();
    }
    std::shared_ptr<TDS::EncodeParameter> TCPServer::popParameter()
    {
        std::unique_lock<std::mutex> unique_lck(_syncMtx);
        _queueCond.wait(unique_lck, [this]{return !_queueParameter.empty();});
        std::shared_ptr<TDS::EncodeParameter> p = _queueParameter.front();
        _queueParameter.pop();
        return p;
    }

    TCPServerServed::TCPServerServed(TCPServer *server)
        : _quit(false), _acceptThread(nullptr)
    {
        try
        {
            _server = server;
            _acceptThread = new std::thread(&TCPServerServed::serviceThread, this);
        }
        catch(std::bad_alloc& ba)
        {
            std::cerr << "bad_alloc caught:" << ba.what() << std::endl;
            exit(-1);
        }
    }
    bool TCPServerServed::recv(NetConnection *connection, EncodeParameter *parameter)
    {
        return false;
    }
    bool TCPServerServed::send(NetConnection *connection, EncodeParameter *parameter)
    {
        return false;
    }
    void TCPServerServed::close(NetConnection* connection)
    {
        TCPServer *server = dynamic_cast<TCPServer*>(connection);
        _quit = true;
        _acceptThread->join();
        for(int s : server->_clientfds)
            ::close(s);
        ::close(server->_hostfd);
        shutdown(server->_hostfd, SHUT_RDWR); //Let accept break out.
        server->changeState(std::shared_ptr<NetState>(new TCPServerClosed()));
    }
    void TCPServerServed::serviceThread()
    {
        int nfds, newfd;
        pollfd fds[TDS_SERVER_MAX_LISTEN];

        memset(fds, 0 ,sizeof(fds));
        nfds = 1;
        fds[0].fd     = _server->_hostfd;
        fds[0].events = POLLIN;

        while(!_quit)
        {
            if(poll(fds, nfds, 1) < 0)
            {
                std::cerr << "poll() failed" << '\n';
            }
            int current_size = nfds;
            for(int i=0;i<current_size;i++)
            {
                if(fds[i].revents == 0)
                    continue;
                if(fds[i].fd == _server->_hostfd)
                {
                    newfd = ::accept(_server->_hostfd, NULL, 0);
                    fds[nfds].fd = newfd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                    _server->_clientfds.insert(newfd);
                }
                else
                {
                    addEncodeParameterToQueue(
                       receiveAnEncodeParameters(fds[i].fd));
                }
            }
        }
    }
    EncodeParameter* TCPServerServed::receiveAnEncodeParameters(int fd)
    {
        EncodeParameter *p = nullptr;
        int len, recv_bytes = 0;
        bool recv_success;
        char buf[PACKET_LENGTH::OF_TOTAL];
        while(recv_bytes < PACKET_LENGTH::OF_TOTAL)
        {
            len = ::recv(fd, buf, sizeof(buf), 0);
            if(len <= 0)
                break;
            recv_bytes += len;
        }
        recv_success = recv_bytes == PACKET_LENGTH::OF_TOTAL;
        if(recv_success)
        {
            p = new EncodeParameter(EncodeParameter::parse(reinterpret_cast<uint8_t*>(buf)));
        }
        return p;
    }
    void TCPServerServed::addEncodeParameterToQueue(EncodeParameter *p)
    {
        if(p)
        {
            std::lock_guard<std::mutex> lck_guard(_server->_syncMtx);
            std::shared_ptr<EncodeParameter> sp(p);
            _server->_queueParameter.push(sp);
            _server->_queueCond.notify_one();
        }
    }
    bool TCPServerClosed::open (NetConnection* connection)
    {
        bool ret = true;
        try
        {
            TCPServer *server = dynamic_cast<TCPServer*>(connection);
            server->_hostfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(server->_hostfd == -1)
                throw std::system_error(std::error_code(errno, std::generic_category()));
            server->_hostaddr.sin_addr.s_addr = inet_addr(server->_ip.c_str());
            server->_hostaddr.sin_port        = htons(server->_port);
            server->_hostaddr.sin_family      = AF_INET;
            if(bind(server->_hostfd,(struct sockaddr*)&server->_hostaddr,
                sizeof(server->_hostaddr)) == -1)
                throw std::system_error(std::error_code(errno, std::generic_category()));
            if(listen(server->_hostfd, TDS_SERVER_MAX_LISTEN) == -1)
                throw std::system_error(std::error_code(errno, std::generic_category()));
            server->changeState(std::shared_ptr<NetState>(new TCPServerServed(server)));
        }
        catch(std::exception &e)
        {
            std::cerr << "Caught an exception in function TCPServerClosed::open()"
                      <<"\ne.what():" << e.what() << std::endl;
            ret = false;
        }
        return ret;
    }
}
