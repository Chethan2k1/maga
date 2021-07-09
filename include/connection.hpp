#pragma once

#include <boost/asio.hpp>
#include <istream>
#include <memory>

class Parser; // forward declaration

class Connection : public std::enable_shared_from_this<Connection> {
private:
  boost::asio::streambuf buffer_;
  std::istream input_stream;

  void read_left_over(std::string &req, size_t &bytes);
  template <typename Callback>
  void internal_read_body(std::string &req, size_t bytes, Callback call);
  // response
  void write_initial_resp(const int &status, const std::string &msg);

public:
  boost::asio::ip::tcp::socket sock_;
  boost::asio::io_context &ioc_;
  Parser *parser_;

  explicit Connection(boost::asio::io_context &);
  ~Connection();

  void start_operation();

  template <typename Callback>
  void read_body(std::string &req, size_t bytes, Callback call);

  // response
  void write_response(const int &status, const std::string &msg);
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