#include "DatagramTimers.h"

#define DEFAULT_TIMEOUT 50

DatagramTimers::DatagramTimers(boost::shared_ptr<std::map<unsigned, char*>> pDg)
{
}

void DatagramTimers::addTimer(unsigned seq)
{
	boost::shared_ptr<Timer> pNewTimer = boost::make_shared<Timer>(DEFAULT_TIMEOUT);
	pNewTimer->start();   //start the timer
	timers_.insert(std::pair<unsigned, Timer>(seq, *pNewTimer));
}

void DatagramTimers::recvAck(unsigned seq)
{
	// Make sure the No.seq timer is in the map
	std::map<unsigned, Timer>::iterator it = timers_.find(seq);

	// If find, delete it from the map
	if (it != timers_.end())
		timers_.erase(it);
}

std::map<unsigned, char*>::iterator DatagramTimers::checkTimeout(std::map<unsigned, char*>::iterator curpos)
{
	std::map<unsigned, Timer>::iterator it = timers_.begin();
	while (it != timers_.end()) {
		if (it->second.is_expired()) break;
		it ++;
	}

	// no timeout
	if (it == timers_.end())	
		return curpos;
	// timeout occured, return the iterator points to the timeout-datagram
	return pDatagram_->find(it->first);
}

void DatagramTimers::updateTimers()
{
	for (std::map<unsigned, Timer>::iterator it = timers_.begin();
		it != timers_.end(); it++) {
		it->second.tick();
	}
}

DatagramTimers::~DatagramTimers()
{
}