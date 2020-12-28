/**
 * @file CubeActutate.cpp
 * @author Team Hugo
 * @brief Provides functionalities for controlling the cube actuation
 * @version 0.1
 * @date 2020-12-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "CubeModule.hpp"

#define ACTUATOR_STEP_SIZE              5

/**
 * @brief 
 * 
 * @param interactiveBoard 
 * @param pixelCount 
 */
void InitialisedActuatorModule(CubeModule_t interactiveBoard[],int pixelCount)
{
    for(int i =0 ; i < pixelCount;i++)
    {
        interactiveBoard[i].actuator.attach(interactiveBoard[i].actuatorPin);
        interactiveBoard[i].currentAngle = interactiveBoard[i].actuatorLowSetting;
        interactiveBoard[i].actuator.write(interactiveBoard[i].actuatorLowSetting);
        interactiveBoard[i].actuatorStepLevel = ACTUATOR_STEP_SIZE;
        interactiveBoard[i].actuatorMode = NO_UPDATE_MODE;
    }
}

/**
 * @brief 
 * 
 * @param interactiveBoard 
 * @param pixelCount 
 */
void ActuatorTaskRunner(CubeModule_t interactiveBoard[],int pixelCount)
{
    ActuatorMode_t mode;
    for(int i =0 ; i < pixelCount;i++)
    {
        mode = interactiveBoard[i].actuatorMode;
        switch (mode)
        {
        case NORMAL_MODE:
            interactiveBoard[i].currentAngle = (interactiveBoard[i].actuationActivated ? 
                interactiveBoard[i].actuatorHighSetting : interactiveBoard[i].actuatorLowSetting); 
            interactiveBoard[i].actuatorMode = NO_UPDATE_MODE;
            break;
        case RAMP_MODE_UP:
            if(interactiveBoard[i].actuationActivated)
            {
                interactiveBoard[i].currentAngle += interactiveBoard[i].actuatorStepLevel;
                if(interactiveBoard[i].currentAngle >= interactiveBoard[i].actuatorHighSetting)
                {
                    interactiveBoard[i].currentAngle = interactiveBoard[i].actuatorHighSetting;
                    interactiveBoard[i].actuatorMode = RAMP_MODE_DOWN;
                }
            }
            break;
        case RAMP_MODE_DOWN:
            if(interactiveBoard[i].actuationActivated)
            {
                interactiveBoard[i].currentAngle -= interactiveBoard[i].actuatorStepLevel;
                if(interactiveBoard[i].currentAngle <= interactiveBoard[i].actuatorLowSetting)
                {
                    interactiveBoard[i].currentAngle = interactiveBoard[i].actuatorLowSetting;
                    interactiveBoard[i].actuatorMode = RAMP_MODE_UP;
                }
            }
            break;
        case DEAD_LOW_MODE:
            if(interactiveBoard[i].actuationActivated)
            {
                interactiveBoard[i].currentAngle -= interactiveBoard[i].actuatorStepLevel;
                if(interactiveBoard[i].currentAngle <= interactiveBoard[i].actuatorLowSetting)
                {
                    interactiveBoard[i].currentAngle = interactiveBoard[i].actuatorLowSetting;
                }
            }
            break;
        case ACTIVE_HIGH_MODE:
            if(interactiveBoard[i].actuationActivated)
            {
                interactiveBoard[i].currentAngle += interactiveBoard[i].actuatorStepLevel;
                if(interactiveBoard[i].currentAngle >= interactiveBoard[i].actuatorHighSetting)
                {
                    interactiveBoard[i].currentAngle = interactiveBoard[i].actuatorHighSetting;
                }
            }
            break;
        case NO_UPDATE_MODE:
            break;
        default:
            break;
        }

        if(interactiveBoard[i].actuator.read() != interactiveBoard[i].currentAngle)
        {
            interactiveBoard[i].actuator.write(interactiveBoard[i].currentAngle);
        }

    }
}

/* EOF */