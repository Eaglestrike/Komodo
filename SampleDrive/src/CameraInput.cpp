/*
 * CameraInput.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: Samsung
 */

#include <CameraInput.h>

CameraInput::CameraInput(std::shared_ptr<NetworkTable> visionTable) {
	// TODO Auto-generated constructor stub
	m_visionTable = visionTable;
}

CameraInput::~CameraInput() {
	// TODO Auto-generated destructor stub
}

double CameraInput::PIDGet() {
	return m_visionTable->GetNumber("Center");
}

