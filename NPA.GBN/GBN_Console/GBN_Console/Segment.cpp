#include "Segment.h"

Segment::Segment(const string& msg = "", unsigned seq = 0)
{
	this->msg = new char[msg.size() + 1];
	strcpy(this->msg, msg.c_str());
	this->seq = seq;
}

char * Segment::get_pchar()
{
	return msg;
}

Segment::~Segment()
{
	delete[] msg;
}
