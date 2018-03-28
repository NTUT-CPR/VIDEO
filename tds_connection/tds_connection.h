#ifndef TDS_CONNECTION_H
#define TDS_CONNECTION_H
#include <set>
#include <string>
#include <memory>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <iostream>
#include <exception>
#include <condition_variable>
#include <stdint.h>

//Networking header
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>

//Json
#include <json.hpp>
/*
________________________________________
/ Have a coffee first before shitting on \
\ your pants.                            /
----------------------------------------
     \                    / \  //\
      \    |\___/|      /   \//  \\
           /0  0  \__  /    //  | \ \
          /     /  \/_/    //   |  \  \
          @_^_@'/   \/_   //    |   \   \
          //_^_/     \/_ //     |    \    \
       ( //) |        \///      |     \     \
     ( / /) _|_ /   )  //       |      \     _\
   ( // /) '/,_ _ _/  ( ; -.    |    _ _\.-~        .-~~~^-.
 (( / / )) ,-{        _      `-.|.-~-.           .~         `.
(( // / ))  '/\      /                 ~-. _ .-~      .-~^-.  \
(( /// ))      `.   {            }                   /      \  \
 (( / ))     .----~-.\        \-'                 .~         \  `. \^-.
            ///.----..>        \             _ -~             `.  ^-`  ^-_
              ///-._ _ _ _ _ _ _}^ - - - - ~                     ~-- ,.-~
                                                                 /.-~


  The acronym of TDS is Transcoding Service System.
  According to the documents, for each packet client or server send,
  it would take 63-byte when transmits.

  Packet definition:
  Filename:   string <50-byte>
  Resolution: 2048,1024,720,576,480,380 <4-byte int)
  QP: 20-48 <4-byte int>
  Video category: W,X,Y,Z (1-byte char)
  bitrate: 45000-1350000 (4-byte int)
*/

namespace TDS
{
  int const TDS_SERVER_MAX_LISTEN    = 50;
  typedef enum
  {
    OF_FILENAME       = 50,
    OF_RESOLUTION     = 4,
    OF_QP             = 4,
    OF_VIDEO_CATEGORY = 1,
    OF_BITRATE        = 4,
    OF_TOTAL          = 63
  }PACKET_LENGTH;
  inline void* net_memcpy(void *dest , int32_t src);
  class NetState;
  class EncodeParameter
  {
  public:
    EncodeParameter(std::string filename, int32_t resolution
      ,int32_t qp, char category, int32_t bitrate);
    static EncodeParameter parse(uint8_t *bytes);
    static EncodeParameter parse(std::string json_string);
    std::string getFilename() const;
    int32_t getResolution() const;
    int32_t getQP() const;
    char getCategory() const;
    int32_t getBitrate() const;
    std::shared_ptr<uint8_t> toBytes() const;
    std::string toJson() const;
    std::string toPrintable() const;
    friend std::ostream& operator<<(std::ostream &os, const EncodeParameter& parameter);
  private:
    char    _filename[PACKET_LENGTH::OF_FILENAME];
    int32_t _resolution;
    int32_t _qp;
    char    _category;
    int32_t _bitrate;
  };

  class NetConnection
  {
  public:
    virtual bool open(std::string ip, int port) = 0;
    virtual bool open(std::string url);
    virtual void close() = 0;
    virtual bool send(EncodeParameter *);
    virtual bool recv(EncodeParameter *);
    void changeState(std::shared_ptr<NetState> state);
  protected:
    friend class NetState;
    std::shared_ptr<NetState> _state;
  };

  class NetState
  {
  public:
    virtual bool open (NetConnection*);
    virtual void close(NetConnection*);
    virtual bool recv (NetConnection*, EncodeParameter *);
    virtual bool send (NetConnection*, EncodeParameter *);
  };

  class NetEstablished : public NetState{};
  class NetClosed : public NetState{};
};
#endif
