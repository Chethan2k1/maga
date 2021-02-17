#include "header.hpp"

const std::map<HeaderKey, std::string> header_map{
    {HeaderKey::CONTENT_TYPE, "Content-Type"}};

const std::map<std::string, HeaderKey> r_header_map{
    {"Content-Type", HeaderKey::CONTENT_TYPE}};

namespace Header {
std::string to_string(const HeaderKey &key) {
  if (header_map.find(key) != header_map.end())
    return header_map.at(key);
  else
    return "";
}

HeaderKey from_string(const std::string &key) {
  if (r_header_map.find(key) != r_header_map.end())
    return r_header_map.at(key);
  else
    return HeaderKey::UNKNOWN_FIELD;
}
} // namespace Header