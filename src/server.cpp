#include <iostream>

#include "server.hpp"
#include "error.hpp"

using namespace boost;

template <typename Connection>
Server<Connection>::Server(unsigned short port)
    : acceptor_(ioc_)
    , work_(ioc_)
    , port_(port)
    , endpoint_(asio::ip::tcp::v4(), port) { 
        io_thread_ = std::move(std::thread([&]{ ioc_.run();})); 
    }

template <typename Connection>
void Server<Connection>::start(){
    using namespace asio::ip;

    system::error_code ec;

    // creates an actual operating system socket
    acceptor_.open(endpoint_.protocol(),ec); 
    acceptor_.set_option(tcp::acceptor::reuse_address(true),ec);
    // binds to the endpoint
    acceptor_.bind(endpoint_,ec); 

    if(!ec){
        std::cout << "Listening for requests from port " << port_ << std::endl;
        acceptor_.listen();
    }else{
        error::print(ec);
        return;
    }

    shared_connection connection = std::make_shared<Connection>(ioc_);
    handle_new_request(ec,connection);
    io_thread_.join();
}

template <typename Connection>
void Server<Connection>::handle_new_request(system::error_code ec,shared_connection old_connection){
    if(!ec){
        old_connection->start_operation();
        shared_connection new_connection = std::make_shared<Connection>(ioc_);
        acceptor_.async_accept(new_connection->sock_,[=](system::error_code ec){
            handle_new_request(ec,new_connection);
        });
    }else{
        error::print(ec);
        return;
    }
}
