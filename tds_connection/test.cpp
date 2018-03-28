#include "tds_client.h"
using namespace std;
using namespace TDS;
int main(void)
{
    HTTPClient client;
    //TDS::EncodeParameter parameter(data.filename,
    //        data.resolution, data.qp,  data.videotype, 
    //        data.bitrate);
    EncodeParameter parameter("The big bunny.mp4", 1080,  20,  'W', 145000); 
    if(!client.open("http://140.124.182.20"))
    {
        cout << "Could not open for connection!\n";
        return -1;
    }
    if(!client.send(&parameter))
    {
        cout << "Transfer parameter to server failed!\n"; 
        return -1;
    }
    return 0;
}
