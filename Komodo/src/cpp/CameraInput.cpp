/*
 * CameraInput.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: Samsung
 */

#include <CameraInput.h>

CameraInput::CameraInput(std::shared_ptr<NetworkTable> visionTable) {
    m_visionTable = std::move(visionTable);
}

CameraInput::~CameraInput() = default;

double CameraInput::PIDGet() {
    return m_visionTable->GetNumber("xValue", 0);
}

