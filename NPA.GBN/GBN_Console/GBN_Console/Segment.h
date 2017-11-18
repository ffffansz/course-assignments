#include<string>
#pragma once
#define MAX_SEGLEN 800
using std::string;

class Segment
{
public:
	char * msg;
	unsigned seq;

	Segment(const string& msg, unsigned seq = 0);
	char * get_pchar();
	~Segment();
};

