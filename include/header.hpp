#pragma once
#include <map>
#include <string>

enum HeaderKey { CONTENT_TYPE, UNKNOWN_FIELD };

extern const std::map<HeaderKey, std::string> header_map;
extern const std::map<std::string, HeaderKey> r_header_map;

namespace Header {
std::string to_string(const HeaderKey &key);
HeaderKey from_string(const std::string &value);
} // namespace Header
