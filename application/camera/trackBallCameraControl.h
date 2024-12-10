#pragma once

#include "cameraControl.h"

class TrackBallCameraControl: public CameraControl {
public:
    TrackBallCameraControl();
    ~TrackBallCameraControl();

    void onCursor(double xpos, double ypos) override;
    void onScroll(double offset) override;
private:
    float mMoveSpeed{0.05f};
    
    void pitch(float angle);
    void yaw(float angle);
};