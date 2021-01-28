#pragma once

#include <memory>

#include <boost/asio.hpp>

class Connection : public std::enable_shared_from_this<Connection> {
    using shared_connection = std::shared_ptr<Connection>;

    private:
        boost::asio::mutable_buffer buffer;
        boost::asio::io_context context_;
    public:
        boost::asio::ip::tcp::socket sock_;

        Connection();
        ~Connection(){ if(sock_.is_open()) sock_.close(); }
        void start_operation(shared_connection connection = shared_from_this());
};