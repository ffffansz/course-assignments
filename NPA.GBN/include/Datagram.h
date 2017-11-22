#pragma once

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