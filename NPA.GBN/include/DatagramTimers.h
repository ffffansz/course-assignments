#include <list>
#include <iostream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include "Timer.h"
#include "Datagram.h"
#pragma once
class DatagramTimers
{
public:
	DatagramTimers(boost::shared_ptr<std::map<unsigned, NewDatagram>> pDg);
	
	//add a timer relate to No.seq Datagram
	void addTimer(unsigned seq);

	//receive ACK then delete the corresponding timer
	void recvAck(unsigned seq);
	
	//check which timer is timeout, return a iterator points to the corresponding datagram
	std::map<unsigned, NewDatagram>::iterator DatagramTimers::checkTimeout(std::map<unsigned, NewDatagram>::iterator curpos);
	
	//Each timer is reduced by one second after sending a new datagram
	void updateTimers();
	
	//When timeout, clear all timers
	void clearTimers();

	bool empty();

	bool is_timeout();

	std::string get_timeout_info();

	~DatagramTimers();
private:
	std::map<unsigned, Timer> timers_;     // seq timer pairs
	boost::shared_ptr < std::map<unsigned, NewDatagram>> pDatagram_;   // pointer to the datagram
	bool istimeout_;
	std::string timeoutinfo_;
};

