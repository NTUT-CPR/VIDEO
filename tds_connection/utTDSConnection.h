#ifndef UT_TDSCONNECTION_H
#define UT_TDSCONNECTION_H
#include "tds_server.h"
#include "tds_client.h"
#include <algorithm>
/*
________________________________
< DI! DA! DI! SHUT UP AND WATCH! >
--------------------------------
   \                                  ___-------___
    \                             _-~~             ~~-_
     \                         _-~                    /~-_
            /^\__/^\         /~  \                   /    \
          /|  O|| O|        /      \_______________/        \
         | |___||__|      /       /                \          \
         |          \    /      /                    \          \
         |   (_______) /______/                        \_________ \
         |         / /         \                      /            \
          \         \^\\         \                  /               \     /
            \         ||           \______________/      _-_       //\__//
              \       ||------_-~~-_ ------------- \ --/~   ~\    || __/
                ~-----||====/~     |==================|       |/~~~~~
                 (_(__/  ./     /                    \_\      \.
                        (_(___/                         \_____)_)

  Before running this test file,
  there are something you should 'KNOW'.

  The first thing is
    Accroding to the constraints in TCP,
    every connection shall generally terminate
    after leaving the TIME-WAIT state.
    In some OS such as Unix/Linux,
    the TIME-WAIT state holds for a period of time,
    SO the test file might not be passed in SUCH TIME-WAIT STATE at THE SAME PORT.
    Anyway, if you want to solve it permanently and then use setsockopt SO_REUSEADDR

  The second thing is
    GoogleTest runs all testing file in multi-threaded based on class,
    SOOOO, I used different ports for different tests.
*/

TEST(TDS_Server, HelloWorld)
{
    TDS::TCPServer server;
}
TEST(TDS_Server, ServerStarts)
{
    TDS::TCPServer server;
    ASSERT_TRUE(server.open("127.0.0.1",12345));
}
TEST(TDS_Server, ServerStartsTwiceShouldFail)
{
    TDS::TCPServer server;
    ASSERT_TRUE(server.open("127.0.0.1",12346));
    ASSERT_FALSE(server.open("127.0.0.1",12346));
}
TEST(TDS_Server, ServerCloses)
{
    TDS::TCPServer server;
    ASSERT_TRUE(server.open("127.0.0.1",12347));
    server.close();
    ASSERT_TRUE(server.open("127.0.0.1",12347));
    server.close();
}

TEST(TDS_Client, HelloWorld)
{
    TDS::TCPClient client;
}

TEST(TDS_Client, ClientConnects)
{
    TDS::TCPServer server;
    TDS::TCPClient client;
    ASSERT_TRUE(server.open("127.0.0.1", 13579));
    ASSERT_TRUE(client.open("127.0.0.1", 13579));
}

TEST(TDS_Server, ClientConnectTwiceShouldFail)
{
    TDS::TCPServer server;
    TDS::TCPClient client;
    ASSERT_TRUE(server.open("127.0.0.1",12222));
    ASSERT_TRUE(client.open("127.0.0.1",12222));
    ASSERT_FALSE(client.open("127.0.0.1",12222));
}

TEST(TDS_Server, CountClientConnections)
{
    TDS::TCPServer server;
    TDS::TCPClient client[TDS::TDS_SERVER_MAX_LISTEN];
    int count = sizeof(client) / sizeof(client[0]);
    ASSERT_TRUE(server.open("127.0.0.1",12349));
    for(int i=0;i<count;i++)
        ASSERT_TRUE(client[i].open("127.0.0.1",12349));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_EQ(count, server.connections());
}

TEST(TDS_Server, MakeEncodeParameter)
{
    TDS::EncodeParameter parameter("Big Bunny.mp4",1080,40,'X',350000);
    ASSERT_EQ("Big Bunny.mp4", parameter.getFilename());
    ASSERT_EQ(1080, parameter.getResolution());
    ASSERT_EQ(40, parameter.getQP());
    ASSERT_EQ('X', parameter.getCategory());
    ASSERT_EQ(350000, parameter.getBitrate());
}

TEST(Data, EncodeParameterToBytes)
{
    TDS::EncodeParameter parameter("Big Bunny.mp4",1080,40,'X',350000);
    std::shared_ptr<uint8_t> bytes = parameter.toBytes();
    uint8_t *p = bytes.get();
    int address_offset = 0;
    std::string s(reinterpret_cast<char*>(p), 0, TDS::PACKET_LENGTH::OF_FILENAME);
    ASSERT_EQ("Big Bunny.mp4", s);
    address_offset += TDS::PACKET_LENGTH::OF_FILENAME;
    ASSERT_EQ(1080, ntohl(*reinterpret_cast<int32_t*>(p+address_offset)));
    address_offset += TDS::PACKET_LENGTH::OF_RESOLUTION;
    ASSERT_EQ(40, ntohl(*reinterpret_cast<int32_t*>(p+address_offset)));
    address_offset += TDS::PACKET_LENGTH::OF_QP;
    ASSERT_EQ('X', *reinterpret_cast<char*>(p+address_offset));
    address_offset += TDS::PACKET_LENGTH::OF_VIDEO_CATEGORY;
    ASSERT_EQ(350000, ntohl(*reinterpret_cast<int32_t*>(p+address_offset)));
    address_offset += TDS::PACKET_LENGTH::OF_BITRATE;
}

TEST(Data, EncodeParameterCreateFromParsingBytes)
{
    TDS::EncodeParameter parameter("Big Bunny.mp4",1080,40,'X',350000);
    TDS::EncodeParameter recreated_p = TDS::EncodeParameter::parse(parameter.toBytes().get());
    ASSERT_EQ("Big Bunny.mp4", recreated_p.getFilename());
    ASSERT_EQ(1080, recreated_p.getResolution());
    ASSERT_EQ(40, recreated_p.getQP());
    ASSERT_EQ('X', recreated_p.getCategory());
    ASSERT_EQ(350000, recreated_p.getBitrate());
}

TEST(Data, EncodeParameterToJson)
{
    TDS::EncodeParameter parameter("Big Bunny.mp4",1080,40,'X',350000);
    std::string s = parameter.toJson();
    ASSERT_EQ("{\"bitrate\":350000,\"category\":\"X\",\"filename\":\"Big Bunny.mp4\",\"qp\":40,\"resolution\":1080}", s);
}

TEST(Data, EncodeParameterCreateFromParsingJson)
{
    TDS::EncodeParameter parameter("Big Bunny.mp4",1080,40,'X',350000);
    TDS::EncodeParameter recreated_p = TDS::EncodeParameter::parse(parameter.toJson());
    ASSERT_EQ("Big Bunny.mp4", recreated_p.getFilename());
    ASSERT_EQ(1080, recreated_p.getResolution());
    ASSERT_EQ(40, recreated_p.getQP());
    ASSERT_EQ('X', recreated_p.getCategory());
    ASSERT_EQ(350000, recreated_p.getBitrate());
}

TEST(TDS_Connection, ClientSendsEncodeParameter)
{
    TDS::TCPServer server;
    TDS::TCPClient client;
    TDS::EncodeParameter parameter("Big Bunny.mp4",1080,40,'X',350000);
    ASSERT_TRUE(server.open("127.0.0.1",22225));
    ASSERT_TRUE(client.open("127.0.0.1",22225));
    ASSERT_TRUE(client.send(&parameter));
}

TEST(TDS_Connection, ServerReceivesEncodeParameter)
{
    TDS::TCPServer server;
    TDS::TCPClient client;
    TDS::EncodeParameter parameter("Big Bunny.mp4",1080,40,'X',350000);
    ASSERT_TRUE(server.open("127.0.0.1",22226));
    ASSERT_TRUE(client.open("127.0.0.1",22226));
    ASSERT_TRUE(client.send(&parameter));

    std::shared_ptr<TDS::EncodeParameter> p = server.popParameter();
    ASSERT_EQ("Big Bunny.mp4", p->getFilename());
    ASSERT_EQ(1080, p->getResolution());
    ASSERT_EQ(40, p->getQP());
    ASSERT_EQ('X', p->getCategory());
    ASSERT_EQ(350000, p->getBitrate());
}

TEST(TDS_Connection, ServerReceivesTwoEncodeParameters)
{
    TDS::TCPServer server;
    TDS::TCPClient client;
    TDS::EncodeParameter parameter1("Big Bunny.mp4",1080,40,'X',350000);
    TDS::EncodeParameter parameter2("Foreman.mp4",720,35,'W',500000);
    ASSERT_TRUE(server.open("127.0.0.1",22227));
    ASSERT_TRUE(client.open("127.0.0.1",22227));
    ASSERT_TRUE(client.send(&parameter1));
    ASSERT_TRUE(client.send(&parameter2));

    std::shared_ptr<TDS::EncodeParameter> p = server.popParameter();
    ASSERT_EQ("Big Bunny.mp4", p->getFilename());
    ASSERT_EQ(1080, p->getResolution());
    ASSERT_EQ(40, p->getQP());
    ASSERT_EQ('X', p->getCategory());
    ASSERT_EQ(350000, p->getBitrate());

    p = server.popParameter();
    ASSERT_EQ("Foreman.mp4", p->getFilename());
    ASSERT_EQ(720, p->getResolution());
    ASSERT_EQ(35, p->getQP());
    ASSERT_EQ('W', p->getCategory());
    ASSERT_EQ(500000, p->getBitrate());
}

TEST(TDS_Connection, ServerReceivesFiveThousandEncodeParameters)
{
    TDS::TCPServer server;
    TDS::TCPClient client;
    std::vector<std::shared_ptr<TDS::EncodeParameter>> array_parameter(5000);
    for(size_t i=0;i<array_parameter.size();i++)
    {
        array_parameter[i] =
            std::shared_ptr<TDS::EncodeParameter>(
                new TDS::EncodeParameter(
                    "Big Bunny.mp4",1080,40,'X',350000+i
                )
            );
    }
    ASSERT_TRUE(server.open("127.0.0.1",22227));
    ASSERT_TRUE(client.open("127.0.0.1",22227));
    for(size_t i=0;i<array_parameter.size();i++)
        ASSERT_TRUE(client.send(array_parameter[i].get()));
    for(size_t i=0;i<array_parameter.size();i++)
    {
        std::shared_ptr<TDS::EncodeParameter> p = server.popParameter();
        ASSERT_EQ("Big Bunny.mp4", p->getFilename());
        ASSERT_EQ(1080, p->getResolution());
        ASSERT_EQ(40, p->getQP());
        ASSERT_EQ('X', p->getCategory());
        ASSERT_EQ(350000+i, p->getBitrate());
    }
}

TEST(TDS_Connection, ServerReceivesTwoEncodeParameterThatClientSendAtTheSameTime)
{
    TDS::TCPServer server;
    TDS::TCPClient client1, client2;
    TDS::EncodeParameter parameter1("Big Bunny.mp4",1080,40,'X',350000);
    TDS::EncodeParameter parameter2("Foreman.mp4",720,35,'W',500000);
    auto client_send = [](TDS::TCPClient *client, TDS::EncodeParameter *p) -> void
    {
            client->send(p);
    };
    ASSERT_TRUE(server.open ("127.0.0.1",22227));
    ASSERT_TRUE(client1.open("127.0.0.1",22227));
    ASSERT_TRUE(client2.open("127.0.0.1",22227));
    std::thread t1(client_send, &client1, &parameter1);
    std::thread t2(client_send, &client2, &parameter2);
    t1.join();
    t2.join();
    std::vector<std::shared_ptr<TDS::EncodeParameter>> array_parameter(2);
    array_parameter[0] = server.popParameter();
    array_parameter[1] = server.popParameter();

    std::sort(array_parameter.begin(), array_parameter.end(),
      [](std::shared_ptr<TDS::EncodeParameter> const& p1,std::shared_ptr<TDS::EncodeParameter> const& p2)
      {
          return p1->getBitrate() < p2->getBitrate();
      }
    );

    std::shared_ptr<TDS::EncodeParameter> p;
    p = array_parameter[0];
    ASSERT_EQ("Big Bunny.mp4", p->getFilename());
    ASSERT_EQ(1080, p->getResolution());
    ASSERT_EQ(40, p->getQP());
    ASSERT_EQ('X', p->getCategory());
    ASSERT_EQ(350000, p->getBitrate());

    p = array_parameter[1];
    ASSERT_EQ("Foreman.mp4", p->getFilename());
    ASSERT_EQ(720, p->getResolution());
    ASSERT_EQ(35, p->getQP());
    ASSERT_EQ('W', p->getCategory());
    ASSERT_EQ(500000, p->getBitrate());
}
#endif
