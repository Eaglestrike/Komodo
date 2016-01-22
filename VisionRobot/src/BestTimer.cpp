#include <time.h>	// class needs this inclusion
#include "BestTimer.h"

//////////////////////////////////////////
// class declaration:



//////////////////////////////////////////
// class implementation:


BestTimer::BestTimer() {
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
}


void BestTimer::start() {
	if(! running) {
		if(resetted)
			beg = (unsigned long) clock();
		else
			beg -= end - (unsigned long) clock();
		running = true;
		resetted = false;
	}
}


void BestTimer::stop() {
	if(running) {
		end = (unsigned long) clock();
		running = false;
	}
}


void BestTimer::reset() {
	bool wereRunning = running;
	if(wereRunning)
		stop();
	resetted = true;
	beg = 0;
	end = 0;
	if(wereRunning)
		start();
}


bool BestTimer::isRunning() {
	return running;
}


unsigned long BestTimer::getTime() {
	if(running)
		return ((unsigned long) clock() - beg) / CLOCKS_PER_SEC;
	else
		return end - beg;
}


bool BestTimer::isOver(unsigned long seconds) {
	return seconds >= getTime();
}
