/*
 * CameraInput.h
 *
 *  Created on: Feb 21, 2016
 *      Author: Samsung
 */
#include <WPILib.h>
#include <iostream>

#ifndef SRC_MODULES_CAMERAINPUT_H_
#define SRC_MODULES_CAMERAINPUT_H_


class CameraInput : public PIDSource {
public:
    explicit CameraInput(std::shared_ptr<NetworkTable> visionTable);

    virtual ~CameraInput();

    double PIDGet() override;

private:
    std::shared_ptr<NetworkTable> m_visionTable;
};

#endif /* SRC_MODULES_CAMERAINPUT_H_ */
