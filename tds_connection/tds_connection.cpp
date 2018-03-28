#include "tds_connection.h"
namespace TDS
{
  inline void* net_memcpy(void *dest , int32_t src)
  {
    uint32_t value = htonl(static_cast<uint32_t>(src));
    return memcpy(dest, static_cast<void*>(&value), sizeof(value));
  }

  EncodeParameter::EncodeParameter(std::string filename, int32_t resolution
    ,int32_t qp, char category, int32_t bitrate)
    : _resolution(resolution), _qp(qp), _category(category), _bitrate(bitrate)
  {
    strncpy(_filename, filename.c_str(), PACKET_LENGTH::OF_FILENAME);
  }
  EncodeParameter EncodeParameter::parse(uint8_t *bytes)
  {
    int     address_offset = 0;
    char    filename[PACKET_LENGTH::OF_FILENAME] = {0};
    int32_t resolution;
    int32_t qp;
    char    category;
    int32_t bitrate;

    memcpy(filename, bytes, PACKET_LENGTH::OF_FILENAME);

    address_offset += TDS::PACKET_LENGTH::OF_FILENAME;
    resolution      = ntohl(*reinterpret_cast<int32_t*>(bytes+address_offset));

    address_offset += TDS::PACKET_LENGTH::OF_RESOLUTION;
    qp              = ntohl(*reinterpret_cast<int32_t*>(bytes+address_offset));

    address_offset += TDS::PACKET_LENGTH::OF_QP;
    category        = *reinterpret_cast<char*>(bytes+address_offset);

    address_offset += TDS::PACKET_LENGTH::OF_VIDEO_CATEGORY;
    bitrate         = ntohl(*reinterpret_cast<int32_t*>(bytes+address_offset));

    address_offset += TDS::PACKET_LENGTH::OF_BITRATE;
    return EncodeParameter(filename, resolution, qp, category, bitrate);
  }
  EncodeParameter EncodeParameter::parse(std::string json_string)
  {
    char    filename[PACKET_LENGTH::OF_FILENAME] = {0};
    int32_t resolution;
    int32_t qp;
    char    category;
    int32_t bitrate;
    nlohmann::json json = nlohmann::json::parse(json_string);
    std::string a = json.dump();
    bitrate       = json["bitrate"].get<int32_t>();
    category      = json["category"].get<std::string>().at(0);
    qp            = json["qp"].get<char>();
    resolution    = json["resolution"].get<int32_t>();
    std::string s = json["filename"].get<std::string>();
    if(s.size() > PACKET_LENGTH::OF_FILENAME)
      throw std::logic_error("The length of filename is longer than PACKET_LENGTH::OF_FILENAME(" + std::to_string(PACKET_LENGTH::OF_FILENAME)
                             +")");
    strncpy(filename, s.c_str(), s.size());
    return EncodeParameter(filename, resolution, qp, category, bitrate);
  }
  std::shared_ptr<uint8_t> EncodeParameter::toBytes() const
  {
    int address_offset = 0;
    uint8_t *bytes = new uint8_t[PACKET_LENGTH::OF_TOTAL]();
    memcpy(bytes, _filename, PACKET_LENGTH::OF_FILENAME);
    address_offset += TDS::PACKET_LENGTH::OF_FILENAME;
    net_memcpy(bytes + address_offset, _resolution);
    address_offset += TDS::PACKET_LENGTH::OF_RESOLUTION;
    net_memcpy(bytes + address_offset, _qp);
    address_offset += TDS::PACKET_LENGTH::OF_QP;
    bytes[address_offset] = _category;
    address_offset += TDS::PACKET_LENGTH::OF_VIDEO_CATEGORY;
    net_memcpy(bytes + address_offset, _bitrate);
    address_offset += TDS::PACKET_LENGTH::OF_BITRATE;
    return std::shared_ptr<uint8_t>(bytes);
  }
  std::string EncodeParameter::toJson() const
  {
    std::string category(1, _category);
    nlohmann::json json {
      {"bitrate", _bitrate},
      {"category", category},
      {"qp", _qp},
      {"resolution", _resolution},
      {"filename", _filename}
    };
    return json.dump();
  }
  std::string EncodeParameter::toPrintable() const
  {
      std::stringstream ss;
      ss << "Filename:"   << _filename   << std::endl;
      ss << "Resolution:" << _resolution << std::endl;
      ss << "QP:"         << _qp         << std::endl;
      ss << "Category:"   << _category   << std::endl;
      ss << "Bitrate:"    << _bitrate    << std::endl;
      return ss.str();
  }
  std::string EncodeParameter::getFilename() const
  {
    return _filename;
  }
  int32_t EncodeParameter::getResolution() const
  {
    return _resolution;
  }
  int32_t EncodeParameter::getQP() const
  {
    return _qp;
  }
  char EncodeParameter::getCategory() const
  {
    return _category;
  }
  int32_t EncodeParameter::getBitrate() const
  {
    return _bitrate;
  }
  std::ostream& operator<<(std::ostream &os, const EncodeParameter& parameter)
  {
    //This function isn't closed for added new members.
    os << "Filename:"   << parameter._filename   << std::endl;
    os << "Resolution:" << parameter._resolution << std::endl;
    os << "QP:"         << parameter._qp         << std::endl;
    os << "Category:"   << parameter._category   << std::endl;
    os << "Bitrate:"    << parameter._bitrate    << std::endl;
    return os;
  }
  bool NetConnection::open(std::string url)
  {
    return false;
  }
  void NetConnection::changeState(std::shared_ptr<NetState> state)
  {
    _state = state;
  }
  bool NetConnection::send(EncodeParameter *parameter)
  {
    return _state->send(this, parameter);
  }
  bool NetConnection::recv(EncodeParameter *parameter)
  {
    return _state->recv(this, parameter);
  }
  bool NetState::open(NetConnection*)
  {
    return false;
  }
  void NetState::close(NetConnection*)
  {

  }
  bool NetState::recv (NetConnection* connection, EncodeParameter *parameter)
  {
    return false;
  }
  bool NetState::send (NetConnection* connection, EncodeParameter *parameter)
  {
    return false;
  }
}
