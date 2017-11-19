#include "Timer.h"


Timer::Timer(unsigned t) 
	: timeout_(t), curtime_(0), expired_(false), valid_(false)
{
}

void Timer::start()
{
	curtime_ = timeout_;
	valid_ = true;
}

void Timer::tick()
{
	if (curtime_ > 0) curtime_ --;
	if (curtime_ == 0) expired_ = true;
}

void Timer::cancel()
{
	valid_ = false;
}

void Timer::restart()
{
	curtime_ = timeout_;
	valid_ = true;
	expired_ = false;
}

bool Timer::is_expired()
{
	return expired_;
}

Timer::~Timer()
{
}
