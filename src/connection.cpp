#include <iostream>

#include "connection.hpp"
#include "error.hpp"

using namespace boost;

void Connection::start_operation(){
    if(sock_.is_open()){
    	//!TODO limit header size
    	asio::async_read_until(sock_,header_buffer_,"\r\n\r\n",[me = shared_from_this()](system::error_code ec,const size_t bytes){
    		std::cout << "*****Header*****" << std::endl;
    		me->handle_header_line(ec,bytes);
    	});
    	// asio::async_read_until(sock_,body_buffer)
    }
}

void Connection::handle_header_line(system::error_code ec,const size_t bytes){
	asio::async_read_until(sock_,header_buffer_,"\r\n",[me = shared_from_this()](system::error_code ec,const size_t bytes){
		if(!ec && bytes){
			std::istream input_stream(&me->header_buffer_);
			std::string header_line;
			getline(input_stream,header_line,'\n');
			if(!header_line.empty())
				std::cout << header_line << std::endl;
	    	me->handle_header_line(ec,bytes);
    	}else{
        	error::print(ec);
        	return;
    	}
    });
}