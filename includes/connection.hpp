#pragma once

#include <memory>

#include <boost/asio.hpp>
class Connection : public std::enable_shared_from_this<Connection> {
    using shared_connection = std::shared_ptr<Connection>;

    private:
        boost::asio::io_context& ioc_;
        boost::asio::streambuf header_buffer_;
        boost::asio::streambuf body_buffer_;

    public:
        boost::asio::ip::tcp::socket sock_;

        explicit Connection(boost::asio::io_context &context,const int &size = 1024)
            : ioc_(context)
            , sock_(context) {}
        
        ~Connection(){ if(sock_.is_open()) sock_.close(); }
        void start_operation();
        void handle_header_line(boost::system::error_code ec,const size_t btyes);
};