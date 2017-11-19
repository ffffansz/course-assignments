#include <string>
#include <boost/shared_ptr.hpp>
#pragma once

class Datagram
{
public:
	Datagram(std::string data = "", unsigned long seq_ = 0, bool ack_ = false);
	~Datagram();
	void ack();
	unsigned long seq();
	bool isAck();

private:
	std::string data_;
	unsigned long seq_;
	bool ack_;
};

