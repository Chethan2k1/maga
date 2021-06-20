#include <boost/asio/basic_stream_socket.hpp>
#include <utility>

#include "connection.hpp"
#include "error.hpp"
#include "parser.hpp"

#define BUFFER_SIZE 128

using namespace boost;

Connection::Connection(boost::asio::io_context &context)
    : input_stream(&request_stream_), output_stream(&request_stream_),
      sock_(context), ioc_(context) {}

Connection::~Connection() {
  std::cout << "Destructor has been summonned" << std::endl;
  if (sock_.is_open())
    sock_.close();
  delete parser_;
}

void Connection::read_left_over(std::string &req, size_t &bytes) {
  std::string extra_data;
  input_stream >> extra_data;
  bytes -= size_t(extra_data.size());
  req.append(std::move(extra_data));
}

void Connection::start_operation() {
  if (sock_.is_open()) {
    //! TODO limit header size
    parser_ = new Parser(shared_from_this());
    asio::async_read_until(
        sock_, request_stream_, "\r\n\r\n",
        [me = shared_from_this()](system::error_code ec, const size_t bytes) {
          if (!ec && bytes) {
            std::vector<char> req(bytes);
            me->input_stream.read(req.data(), bytes);
            me->parser_->parser_init(std::string(req.data()));
          } else {
            error::print(ec);
            return;
          }
        });
  }
}