/*
 * BestTimer.h
 *
 *  Created on: Jan 22, 2016
 *      Author: Cole
 */

#ifndef SRC_BESTTIMER_H_
#define SRC_BESTTIMER_H_

class BestTimer {
public:
	BestTimer();
	~BestTimer(){};
	void start();
	void stop();
	void reset();
	bool isRunning();
	unsigned long getTime();
	bool isOver(unsigned long);

private:
	bool resetted;
    bool running;
	unsigned long beg;
	unsigned long end;
};
#endif /* SRC_BESTTIMER_H_ */
