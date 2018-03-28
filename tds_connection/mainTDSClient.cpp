#include "tds_client.h"
using namespace TDS;
using namespace std;
int main(int argc, char **argv)
{
  TCPClient client;
  string ip;
  int port;
  cout << "Server host IP:";
  cin  >> ip;
  cout << "Server host Port:";
  cin  >> port;
  if(!client.open(ip, port)){
    printf("Could not connect to server on [%s:%d]\n", ip.c_str(), port);
    return -1;
  }
  cout << "Connected to the server.\n";
  cin.ignore(); //flush out \n from buffer.
  bool sendResult;
  do
  {
      string filename;
      int resolution;
      int qp;
      char category;
      int bitrate;
      cout << "Filename:";
      getline(cin,filename);
      cout << "Resolution:";
      cin  >> resolution;
      cout << "QP:";
      cin  >> qp;
      cout << "Category (W,X,Y,Z):";
      cin  >> category;
      cout << "Bitrate:";
      cin  >> bitrate;
      cout << '\n';
      cin.ignore(); //flush out \n from buffer.

      TDS::EncodeParameter parameter(filename,resolution,qp,category,bitrate);
      sendResult = client.send(&parameter);
  }while(sendResult);
  return 0;
}
