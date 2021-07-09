#include <ostream>
#include <string>

template <typename T> struct Header {
  const std::string key_;
  const T value_;

  explicit Header(const std::string &key, const T &value)
      : key_(key), value_(value) {}

  std::ostream &operator<<(std::ostream &os) {
    os << key_ << ": " << value_ << "\r\n";
    return os;
  }
};