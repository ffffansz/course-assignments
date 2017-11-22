#include "DatagramTimers.h"
#include "Datagram.h"

DatagramTimers::DatagramTimers(boost::shared_ptr<std::map<unsigned, NewDatagram>> pDg, unsigned timeout)
	: pDatagram_(pDg), istimeout_(false), timeout_(timeout)
{
}

	void DatagramTimers::addTimer(unsigned seq)
{
	boost::shared_ptr<Timer> pNewTimer = boost::make_shared<Timer>(timeout_);
	pNewTimer->start();   //start the timer
	timers_.insert(std::pair<unsigned, Timer>(seq, *pNewTimer));
	//std::cout << "Add timer No." << seq << std::endl;
}

void DatagramTimers::recvAck(unsigned seq)
{
	// Make sure the No.seq timer is in the map
	std::map<unsigned, Timer>::iterator it = timers_.find(seq);

	// If find, delete it from the map
	if (it != timers_.end()) {
		timers_.erase(it);
		//std::cout << "Delete timer No." << seq << std::endl;
	}
		
}

std::map<unsigned, NewDatagram>::iterator DatagramTimers::checkTimeout(std::map<unsigned, NewDatagram>::iterator curpos)
{
	std::map<unsigned, Timer>::iterator it = timers_.begin();
	while (it != timers_.end()) {
		if (it->second.is_expired()) 
			break;
		it ++;
	}

	// no timeout
	if (it == timers_.end()) {
		//std::cout << "No time out" << std::endl;
		return curpos;
	}
	// timeout occured, return the iterator points to the timeout-datagram
	//std::cout << "Timeout: No." << std::endl;
	std::map<unsigned, NewDatagram>::iterator result = pDatagram_->find(it->first);
	//clearTimers();
	istimeout_ = true;
	timeoutinfo_ = boost::lexical_cast<std::string>(result->first);
	return result;
}

void DatagramTimers::updateTimers()
{
	for (std::map<unsigned, Timer>::iterator it = timers_.begin();
		it != timers_.end(); it++) {
		it->second.tick();
	}
}

void DatagramTimers::clearTimers()
{
	timers_.clear();
	istimeout_ = false;
	timeoutinfo_ = std::string();
}

bool DatagramTimers::empty()
{
	return timers_.empty();
}

bool DatagramTimers::is_timeout()
{
	return istimeout_;
}

std::string DatagramTimers::get_timeout_info()
{
	return timeoutinfo_;
}

DatagramTimers::~DatagramTimers()
{
}