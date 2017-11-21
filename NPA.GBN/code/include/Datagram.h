#include <string>
#include <boost/serialization/access.hpp>
#pragma once

/*
class Datagram
{
public:
	Datagram();
	Datagram(std::string data, unsigned long seq = 0, bool ack = false);
	~Datagram();
	std::string * data();
	void ack();
	unsigned long seq();
	bool isAck();

private:
	std::string data_;
	unsigned long seq_;
	bool ack_;

	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & data_;
		ar & seq_;
		ar & ack_;
	}
};
*/

#define MAX_MSGLEN 1024
struct NewDatagram {
	char msg_[MAX_MSGLEN];
	unsigned seq_;
	NewDatagram(const char * msg, unsigned seq) {
		strcpy_s(msg_, MAX_MSGLEN, msg);
		seq_ = seq;
	}
	NewDatagram(){}
};