#include "tds_server.h"
#include "tds_client.h"
using namespace TDS;
using namespace std;
int main(int argc, char **argv)
{
    TCPServer server;
    HTTPClient rc_client;
    
    string serv_ip, rc_url;
    int serv_port; 
    cout << "Service IP:";
    cin  >> serv_ip;
    cout << "Service Port:";
    cin  >> serv_port;

    cout << "Resource controller URL:";
    cin  >> rc_url;

    if(!server.open(serv_ip, serv_port))
    {
        printf("Could not open server on [%s:%d]\n", serv_ip.c_str(), serv_port);
        return -1;
    }
    else if(!rc_client.open(rc_url))
    {
        printf("Could not open http client to resource controller on [%s]\n", rc_url.c_str());
        return -1;
    }
    
    cout << "Server has started.\n";
    printf("Waiting on [%s:%d]\n", serv_ip.c_str(), serv_port);
    while(true)
    {
        shared_ptr<EncodeParameter> parameter = server.popParameter();
        cout << "Received a new parameter from client\n";
        cout << *parameter << endl;
        if(rc_client.send(parameter.get()))
            cout << "Transfered parameter to resource controller\n";
        else
        {
            cout << "Could not transfer the parameter to resource controller.\n";
            break;
        }
        cout << "Resource controller replied:" << rc_client.getHtmlContent() << endl;
    }
    return 0;
}
