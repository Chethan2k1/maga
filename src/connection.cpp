#include <iostream>

#include "connection.hpp"
#include "error.hpp"
#include "parser.hpp"

using namespace boost;

Connection::Connection(boost::asio::io_context &context)
    : ioc_(context), sock_(context), input_stream(&request_stream_),
      output_stream(&request_stream_) {}

Connection::~Connection() {
  if (sock_.is_open())
    sock_.close();
  delete parser_;
}

void Connection::start_operation() {
  parser_ = new Parser(shared_from_this());
  if (sock_.is_open()) {
    //! TODO limit header size
    asio::async_read_until(
        sock_, request_stream_, "\r\n\r\n",
        [me = shared_from_this()](system::error_code ec, const size_t bytes) {
          if (!ec && bytes) {
            std::unique_ptr<char> req(new char[bytes]);
            me->input_stream.read(req.get(), bytes);
            me->parser_->parser_init(std::string(req.get()));
          } else {
            error::print(ec);
            return;
          }
        });
  }
}

void Connection::read_body(const size_t &bytes) {}