#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>

#include "Datagram.h"

#define DEFAULT_PORT 20510
#define DEFAULT_BUFLEN 512

using boost::asio::ip::udp;

class udp_server
{
public:
	udp_server(boost::asio::io_service& io_service, unsigned port)
		: socket_(io_service, udp::endpoint(udp::v4(), port))
	{
		start_receive();
	}

private:
	/* receive message concurrently */
	void start_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_), 
			remote_endpoint_,
			boost::bind(
				&udp_server::handle_receive,
				this,  /* for member function of class udp_server */
				recv_buffer_.data(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	/* send ack randomly */
	void handle_receive(const boost::system::error_code& error,
		Datagram& data,
		std::size_t /*bytes_transferred*/)
	{
		if (!error || error == boost::asio::error::message_size)
		{
			/* generate a random number */
			boost::mt19937 rng(time(0));
			boost::uniform_01<boost::mt19937&> u01(rng);
			if (u01() > 0.2) {
				boost::shared_ptr<std::vector<Datagram>> message(new Datagram("", data.seq(), true);
				socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_);
			}

			start_receive();
		}
	}
	
	/* */

	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	std::vector<Datagram> recv_buffer_;
};

int main(int argc, char **argv)
{
	int port = DEFAULT_PORT;
	/*
	if (argc != 2) {
		std::cout << "Usage: server.exe <port>" << std::endl;
		return -1;
	}
	*/
	if (argc == 2)	port = boost::lexical_cast<int>(argv[1]);

	try
	{
		boost::asio::io_service io_service;
		udp_server server(io_service, port);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}