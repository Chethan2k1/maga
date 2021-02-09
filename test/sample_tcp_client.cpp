#include <boost/asio.hpp>

using namespace boost;

int main(){
	asio::io_context ioc_;
	asio::ip::tcp::socket sock(ioc_);
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"),8989);
	sock.connect(ep);
	sock.write_some(asio::buffer("Hello from the client"));
	sock.close();
}
