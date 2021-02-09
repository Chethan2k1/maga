#include <iostream>


#include "server.hpp"
#include "connection.hpp"

class Connection;

int main(){
    using Server_ = Server<Connection>;
    auto server = std::make_unique<Server_>(8989);

    server->start();
}