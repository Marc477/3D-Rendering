/*
 *  Camera.cpp
 *  TP2
 *
 *  Created by Olivier Vaillancourt on 10-10-25.
 *  Copyright 2010 Université de Sherbrooke. All rights reserved.
 *
 */

#include "Camera.h"

#include <cmath>

#include "GLInclude.h"

Camera::Camera(double aElevationAngle   /*= 0.0*/,
               double aLateralAngle     /*=0.0*/,
               double aDistance         /*=20.0*/)
: mOldX(0)
, mOldY(0)
, mPhi(aElevationAngle)
, mTheta(aLateralAngle)
, mDistance(aDistance)
, mLeftDown(false)
, mRightDown(false)
{}

Camera::~Camera()
{}

void Camera::Refresh()
{
    double PosX = mDistance * cos(mPhi) * sin(mTheta);
    double PosY = mDistance * sin(mPhi);
    double PosZ = mDistance * cos(mPhi) * cos(mTheta);
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    gluLookAt(  PosX,   PosY,   PosZ,
                0,      0,      0,
                0,      1,      0);
}

void Camera::UpdateControls(const MouseEventArgs& aArgs)
{
    if(aArgs.mButton == Mouse_Left)
    {
        if(aArgs.mState == State_Down)
        {
            mLeftDown = true;
            
            mOldX = aArgs.mCoordX;
            mOldY = aArgs.mCoordY;
        }
        else if(aArgs.mState == State_Up)
        {
            mLeftDown = false;
        }
    }
    
    if(aArgs.mButton == Mouse_Right)
    {
        if(aArgs.mState == State_Down)
        {
            mRightDown = true;
            
            mOldX = aArgs.mCoordX;
            mOldY = aArgs.mCoordY;
        }
        else if(aArgs.mState == State_Up)
        {
            mRightDown = false;
        }
    }
    
    int DeltaX = aArgs.mCoordX - mOldX;
    int DeltaY = aArgs.mCoordY - mOldY;
    
    mOldX = aArgs.mCoordX;
    mOldY = aArgs.mCoordY;
    
    if(mLeftDown)
    {
        mPhi += DeltaY/200.0f;
        mTheta -= DeltaX/100.0f;
        
        if(mPhi > 3.141592f/2.0f)
        {
            mPhi  = 3.141592f/2.0f;
        }
        else if(mPhi < -0.1)
        {
            mPhi = -0.1;
        }
    }
    
    if(mRightDown)
    {
        mDistance += DeltaY/20.0f;
        
        if(mDistance < 0.5f)
        {
            mDistance = 0.5f;
        }
    }
}
