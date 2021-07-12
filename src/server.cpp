#include <iostream>

#include "error.hpp"
#include "server.hpp"
#include <filesystem>

using namespace boost;

template <typename Connection>
Server<Connection>::Server(const unsigned short &port)
    : port_(port), root_dir("../static"), io_thread_(), work_(ioc_),
      endpoint_(asio::ip::tcp::v4(), port), acceptor_(ioc_) {
  io_thread_ = std::thread([&] { ioc_.run(); });
  using namespace std::filesystem;
  assert(exists(root_dir));
  assert(is_directory(root_dir));
}

template <typename Connection> void Server<Connection>::start() {
  using namespace asio::ip;

  system::error_code ec;

  // creates an actual operating system socket
  acceptor_.open(endpoint_.protocol(), ec);
  acceptor_.set_option(tcp::acceptor::reuse_address(true), ec);
  // binds to the endpoint
  acceptor_.bind(endpoint_, ec);

  if (!ec) {
    std::cout << "Listening for requests from port " << port_ << std::endl;
    acceptor_.listen();
  } else {
    error::print(ec);
    return;
  }

  handle_new_request();
  io_thread_.join();
}

template <typename Connection> void Server<Connection>::handle_new_request() {
  shared_connection conn = std::make_shared<Connection>(ioc_);
  acceptor_.async_accept(conn->sock_, [conn, this](system::error_code ec) {
    if (!ec) {
      conn->set_root_dir(this->root_dir);
      conn->start_operation();
      this->handle_new_request();
    } else {
      error::print(ec);
      return;
    }
  });
}

template <typename Connection>
void Server<Connection>::set_root_dir(const std::string &path) {
  using namespace std::filesystem;
  assert(exists(path));
  assert(is_directory(path));
  root_dir = path;
}
