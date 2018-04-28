/*
 * RobotModule.cpp
 *
 *  Created on: Feb 13, 2016
 *      Author: Samsung
 */

#include <modules/RobotModule.h>

RobotModule::RobotModule(std::string name) : m_Module_Name(name), m_Enabled(false), real(true) {
    // TODO Auto-generated constructor stub

}

void RobotModule::enable() {
    m_Enabled = true;
}

void RobotModule::disable() {
    m_Enabled = false;
}

void RobotModule::enableReal() {
    real = true;
}

void RobotModule::enablePractice() {
    real = false;
}

RobotModule::~RobotModule() {
    // TODO Auto-generated destructor stub
}

