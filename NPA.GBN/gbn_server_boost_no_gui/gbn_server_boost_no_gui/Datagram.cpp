#include "Datagram.h"


Datagram::Datagram(std::string data, unsigned long seq = 0, bool ack = false)
	: data_(data), seq_(seq)
{
}

Datagram::~Datagram()
{
}

void Datagram::ack()
{
	ack_ = true;
}

unsigned long Datagram::seq()
{
	return seq_;
}

bool Datagram::isAck()
{
	return ack_;
}
