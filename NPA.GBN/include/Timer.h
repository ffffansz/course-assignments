#pragma once
#include <iostream>
class Timer
{
public:
	Timer(unsigned t);
	void start();
	void tick();        //if timeout, set expired_ true
	void cancel();
	void restart();     //only for canceled timer
	bool is_expired();
	~Timer();
private:
	unsigned timeout_;
	unsigned curtime_;
	bool expired_;
	bool valid_;
};

