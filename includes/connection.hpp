#pragma once

#include <memory>

#include <boost/asio.hpp>

class Connection : public std::enable_shared_from_this<Connection> {
    using shared_connection = std::shared_ptr<Connection>;
        std::vector<char> buffer_space_;
    private:
        boost::asio::mutable_buffers_1 buffer_;
        boost::asio::io_context& ioc_;
    public:
        boost::asio::ip::tcp::socket sock_;

        explicit Connection(boost::asio::io_context &context,const int &size = 1024)
            : ioc_(context)
            , sock_(context)
            , buffer_space_(size) 
            , buffer_(buffer_space_.data(),size){}
        
        ~Connection(){ if(sock_.is_open()) sock_.close(); }
        void start_operation();
};