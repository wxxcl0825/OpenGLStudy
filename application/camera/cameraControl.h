#pragma once

#include "camera.h"
#include <map>

class CameraControl {
public:
  CameraControl();
  ~CameraControl();

  virtual void onKey(int key, int action, int mods);
  virtual void onMouse(int button, int action, float xpos, float ypos);
  virtual void onCursor(double xpos, double ypos);
  virtual void onScroll(double offset);
  virtual void update();

  void setCamera(Camera *camera) { mCamera = camera; }
  void setSensitivity(float s) { mSensitivity = s; }
  void setScaleSpeed(float s) { mScaleSpeed = s; }

protected:
  bool mLeftMouseDown{false};
  bool mRightMouseDown{false};
  bool mMiddleMouseDown{false};
  float mCurrentX{0.0f};
  float mCurrentY{0.0f};
  std::map<int, bool> mKeyMap;
  float mSensitivity{0.2f};
  Camera *mCamera{nullptr};
  float mScaleSpeed{0.2f};
};