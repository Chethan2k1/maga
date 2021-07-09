#include "connection.hpp"
#include "error.hpp"
#include "parser.hpp"
#include <iomanip>

#define BUFFER_SIZE 128

using namespace boost;

Connection::Connection(boost::asio::io_context &context)
    : input_stream(&buffer_), sock_(context), ioc_(context) {}

Connection::~Connection() {
  std::cout << "Destructor has been summoned!" << std::endl;
  if (sock_.is_open())
    sock_.close();
  delete parser_;
}

// this function reads the remaining bytes in the streambuffer
void Connection::read_left_over(std::string &req, size_t &bytes) {
  std::istreambuf_iterator<char> itr(this->input_stream);
  bytes -= size_t(buffer_.in_avail());
  req.append(itr, std::istreambuf_iterator<char>());
}

void Connection::start_operation() {
  if (sock_.is_open()) {
    //! TODO limit header size
    parser_ = new Parser(shared_from_this());
    asio::async_read_until(
        sock_, buffer_, "\r\n\r\n",
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

void Connection::write_initial_resp(const int &status, const std::string &msg) {
  std::stringstream ss;
  ss << "HTTP/1.1 " << status << " " << msg << "\r\n";
  auto ct = std::time(nullptr);
  ss << "Date: " << std::put_time(std::gmtime(&ct), "%a, %d %b %G %X %Z\r\n");
  ss << "Server: Maga/dev\r\n";
  ss << "Content-Type: text/html\r\n";
  ss << "Content-Length:" << 20 << "\r\n\r\n";
  ss << "This is the end!\r\n";
  std::cout << "Reached here" << std::endl;
  asio::write(sock_, asio::buffer(ss.str()));
}

// this function would be overloaded for different kind of responses
void Connection::write_response(const int &status, const std::string &msg) {
  // write the basic header body
  write_initial_resp(status, msg);
}