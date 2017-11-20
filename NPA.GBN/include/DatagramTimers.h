#include <list>
#include <iostream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "Timer.h"
#pragma once
class DatagramTimers
{
public:
	DatagramTimers(boost::shared_ptr<std::map<unsigned, char*>> pDg);
	
	//add a timer relate to No.seq Datagram
	void addTimer(unsigned seq);

	//receive ACK then delete the corresponding timer
	void recvAck(unsigned seq);
	
	//check which timer is timeout, return a iterator points to the corresponding datagram
	std::map<unsigned, char*>::iterator DatagramTimers::checkTimeout(std::map<unsigned, char*>::iterator curpos);   
	
	//Each timer is reduced by one second after sending a new datagram
	void updateTimers();
	
	~DatagramTimers();
private:
	std::map<unsigned, Timer> timers_;     // seq timer pairs
	boost::shared_ptr < std::map<unsigned, char*>> pDatagram_;   // pointer to the datagram
};

