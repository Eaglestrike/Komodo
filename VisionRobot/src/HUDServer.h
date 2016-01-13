/*
 * HUDServer.h
 *
 *  Created on: Mar 25, 2015
 *      Author: LAHS
 */

#ifndef SRC_PERIPHERALS_HUD_HUDSERVER_H_
#define SRC_PERIPHERALS_HUD_HUDSERVER_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "WPILib.h"
#include <thread>
#include <unistd.h>
#include "DriverStation.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


class ArmModule;

class HUDServer {
public:
	HUDServer(int, Servo*, Servo*);
	virtual ~HUDServer();
	void callSend(int);
	static void send(void*, int);
	static HUDServer* getInstance();
private:
	std::thread t;
	int matchtime = 0;

};

#endif /* SRC_PERIPHERALS_HUD_HUDSERVER_H_ */
