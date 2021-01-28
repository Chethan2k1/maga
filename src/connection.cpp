#include "connection.hpp"

Connection()

void Connection::start_operation(asio::io_context context,shared_connection connection){
    if(connection->sock_.is_open())
        connection->sock_.async_read_some()
}