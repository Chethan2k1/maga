#include <iostream>

#include "server.hpp"
#include "error.hpp"

using namespace boost;

template <typename Connection>
Server<Connection>::Server(unsigned short port) 
    : acceptor_(ios_)
    , port_(port)
    , endpoint_(asio::ip::tcp::v4(), port) {}

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

    shared_connection connection = make_shared<Connection>();
    acceptor_.async_accept(connection->sock_,[this, connection](system::error_code ec){
        handle_new_request(connection,ec);
    });
}

template <typename Connection>
void Server<Connection>::handle_new_request(shared_connection connection, system::error_code &ec){
    if(!ec){
    }else{
        error::print(ec);
        return;
    }
}
