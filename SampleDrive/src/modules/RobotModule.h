/*
 * RobotModule.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Samsung
 */

#ifndef SRC_MODULES_ROBOTMODULE_H_
#define SRC_MODULES_ROBOTMODULE_H_
#include <string>

class RobotModule {
public:
	RobotModule(std::string);
	virtual ~RobotModule();
	void enable();
	void disable();
	void enableReal();
	void enablePractice();

protected:
	std::string m_Module_Name;
	bool m_Enabled;
	bool real;
};

#endif /* SRC_MODULES_ROBOTMODULE_H_ */
