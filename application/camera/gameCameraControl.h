#pragma once

#include "cameraControl.h"

class GameCameraControl : public CameraControl {
public:
    GameCameraControl();
    ~GameCameraControl();

    void setSpeed(float s) { mSpeed = s; }
    void onCursor(double xpos, double ypos) override;
    void update() override;

private:
    float mPitch{0.0f};
    float mSpeed{0.01f};

    void pitch(float angle);
    void yaw(float angle);
};