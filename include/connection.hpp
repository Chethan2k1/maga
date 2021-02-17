#pragma once

#include <boost/asio.hpp>
#include <istream>
#include <memory>
#include <ostream>

class Parser;

class Connection : public std::enable_shared_from_this<Connection> {
  using shared_connection = std::shared_ptr<Connection>;

private:
  boost::asio::io_context &ioc_;
  boost::asio::streambuf request_stream_;
  std::istream input_stream;
  std::ostream output_stream;

public:
  boost::asio::ip::tcp::socket sock_;
  Parser *parser_;

  explicit Connection(boost::asio::io_context &context);
  ~Connection();

  void start_operation();
  void read_body(const size_t &bytes);
};