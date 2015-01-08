/*
 *  Camera.h
 *  TP2
 *
 *  Created by Olivier Vaillancourt on 10-10-25.
 *  Copyright 2010 Université de Sherbrooke. All rights reserved.
 *
 */
#ifndef CAMERA_H_
#define CAMERA_H_

class Camera
{
public:
    
    enum InputState
    {
        State_None,
        State_Down,
        State_Up,
    };
    
    enum MouseButton
    {
        Mouse_Left,
        Mouse_Middle,
        Mouse_Right,
        Mouse_None,
    };
    
    struct MouseEventArgs
    {
        InputState  mState;
        MouseButton mButton;
        
        int mCoordX;
        int mCoordY;
    };
    
    Camera(double aElevationAngle   = 0.0,
           double aLateralangle     = 0.0, 
           double aDistance         = 20.0);
    
    virtual ~Camera();
    
    void Refresh();
    
    void UpdateControls(const MouseEventArgs& aArgs);
    
    
protected:
    
    int mOldX;
    int mOldY;
    
    double mPhi;
    double mTheta;
    double mDistance;
    
    bool mLeftDown;
    bool mRightDown;
    
private:
};

#endif //CAMERA_H_
