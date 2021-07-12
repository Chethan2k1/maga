#pragma once

#include <boost/asio.hpp>
#include <istream>
#include <memory>
#include <sstream>

class Parser; // forward declaration

class Connection : public std::enable_shared_from_this<Connection> {
private:
  std::string root_dir;
  boost::asio::streambuf buffer_;
  std::istream input_stream;
  std::stringstream ss;

  void read_left_over(std::string &, size_t &);
  template <typename Callback>
  void internal_read_body(std::string &, size_t, Callback);
  // response
  void write_initial_resp(const int &, const std::string &);

public:
  boost::asio::ip::tcp::socket sock_;
  boost::asio::io_context &ioc_;
  Parser *parser_;

  explicit Connection(boost::asio::io_context &);
  ~Connection();

  void start_operation();
  void set_root_dir(const std::string &rd) { root_dir = rd; }

  template <typename Callback> void read_body(std::string &, size_t, Callback);

  // response
  inline bool is_resp_empty() { return (ss.str().empty()); };
  void write_file(const std::string_view &);
  void write_response(const int &, const std::string &);
  void send_response();
};

// definations for templated functions

#define BUFFER_SIZE 128
template <typename Callback>
void Connection::internal_read_body(std::string &req, size_t bytes,
                                    Callback call) {
  if (sock_.is_open()) {
    std::vector<char> buff_space(BUFFER_SIZE);
    sock_.async_read_some(boost::asio::buffer(buff_space.data(), BUFFER_SIZE),
                          [me = shared_from_this(), bytes, &req,
                           buff_space = std::move(buff_space),
                           call](const boost::system::error_code ec,
                                 const size_t bytes_read) mutable {
                            req.append(buff_space.data(), bytes_read);
                            bytes -= bytes_read;
                            if (bytes > 0)
                              me->internal_read_body(req, bytes, call);
                            else
                              call();
                          });
  }
}

template <typename Callback>
void Connection::read_body(std::string &req, size_t bytes, Callback call) {
  // reserve so much space in the string
  req.reserve(bytes);
  // reading from remaining data in the streambuf
  read_left_over(req, bytes);
  ioc_.dispatch([me = shared_from_this(), &req, bytes, call] {
    me->internal_read_body(req, bytes, call);
  });
}