#pragma once
#include <boost/asio.hpp>

template <typename Connection> 
class Server{
    using shared_connection = std::shared_ptr<Connection>; 

    private:
    unsigned short port_;
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::ip::tcp::acceptor acceptor_;

    void handle_new_request(shared_connection connection, boost::system::error_code &ec);

    public:
    explicit Server(unsigned short port);
    ~Server() { if(acceptor_.is_open()) acceptor_.close(); }

    void start();
};