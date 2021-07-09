#pragma once

#include <boost/asio.hpp>
#include <thread>

template <typename Connection> class Server {
  using shared_connection = std::shared_ptr<Connection>;

private:
  unsigned short port_;
  std::thread io_thread_;
  boost::asio::io_context ioc_;
  boost::asio::io_context::work work_;
  boost::asio::ip::tcp::endpoint endpoint_;
  boost::asio::ip::tcp::acceptor acceptor_;

  void handle_new_request();

public:
  explicit Server(unsigned short port);

  ~Server() {
    if (acceptor_.is_open())
      acceptor_.close();
    if (io_thread_.joinable())
      io_thread_.join();
  }

  void start();
};

#include "server.cpp"