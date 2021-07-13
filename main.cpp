#include <iostream>
#include <filesystem>

#include "server.hpp"
#include "connection.hpp"
#include "config.hpp"

using namespace std;

int main(){
    using Server_ = Server<Connection>;
    auto server = std::make_unique<Server_>(PORT);
    // Ensure the ROOT_DIR is valid
    assert(filesystem::exists(ROOT_DIR));
    assert(filesystem::is_directory(ROOT_DIR));
    server->start();
}