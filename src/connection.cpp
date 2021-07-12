#include "connection.hpp"
#include "error.hpp"
#include "parser.hpp"

#include <filesystem>
#include <fstream>
#include <string_view>

using namespace boost;

Connection::Connection(boost::asio::io_context &context)
    : input_stream(&buffer_), sock_(context), ioc_(context) {}

Connection::~Connection() {
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
  ss << "HTTP/1.1 " << status << " " << msg << "\r\n";
  auto ct = std::time(nullptr);
  ss << "Date: " << std::put_time(std::gmtime(&ct), "%a, %d %b %G %X %Z\r\n");
  ss << "Server: Maga/dev\r\n";
}

void Connection::write_file(const std::string_view &rpath) {
  using namespace std;
  using namespace std::filesystem;
  path apath(root_dir);
  apath += path(rpath);
  // path should exist and shoudn't be a directory
  if (!exists(apath))
    return write_initial_resp(404, "Not Found");
  else if (is_directory(apath))
    return write_initial_resp(404, "Not Found");
  write_initial_resp(200, "OK");
  ifstream file_stream(apath, std::ios::in);
  ss << "Content-Type: text/html\r\n";
  ss << "Content-Length:" << file_size(apath) << "\r\n\r\n";
  ss << file_stream.rdbuf();
  file_stream.close();
}

// this function would be overloaded for different kind of responses
void Connection::write_response(const int &status, const std::string &msg) {
  // write the basic header body
  write_initial_resp(status, msg);
}

void Connection::send_response() {
  asio::async_write(sock_, asio::buffer(ss.str()),
                    [](const system::error_code &ec, size_t) {
                      if (!ec)
                        std::cout << "Response sent!" << std::endl;
                      else
                        error::print(ec);
                    });
}