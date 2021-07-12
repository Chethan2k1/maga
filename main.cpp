#include <iostream>

#include "server.hpp"
#include "connection.hpp"

int main(){
    using Server_ = Server<Connection>;
    auto server = std::make_unique<Server_>();

    server->start();
}