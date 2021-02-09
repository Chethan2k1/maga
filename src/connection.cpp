#include <iostream>

#include "connection.hpp"
#include "error.hpp"

using namespace boost;

void Connection::start_operation(){
    if(sock_.is_open()){
        sock_.async_read_some(buffer_,[me = shared_from_this()](const system::error_code &ec, std::size_t bytes){
            if(!ec){
                // (me->sock_).async_write_some(me->buffer_,[me_ = me->shared_from_this()](const system::error_code &ec, std::size_t bytes){
                //     me_->start_operation();
                // });
                for(int i=0;i<bytes;++i)
                    std::cout << me->buffer_space_[i];
                me->start_operation();
            }else{
                error::print(ec);
                return;
            }
        });
    }
}