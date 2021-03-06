/**
 * @file MessageCreator.cpp
 * @author Team Hugo
 * @brief 
 * @version 0.1
 * @date 2020-12-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

// comment the following line to disable the Debug printing over the UART
#define DEBUG_ENABLE
#include "DebugPrint.hpp"

#include "CubeModule.hpp"
#include "Arduino.h"

int msgStrByteCount = 0;

/**
 * @brief 
 * 
 * @param pixelCount 
 */
void InitialiseMessageModule(int pixelCount)
{
    msgStrByteCount = (pixelCount / 8) + ((pixelCount % 8) == 0 ? 0 : 1);
    DBG_PRINT(F("InitialiseMessageModule >> Initialised, msg payload size "));
    DBG_PRINT_LN(msgStrByteCount);
}

/**
 * @brief Get the Message Length object
 * 
 * @return int 
 */
int GetMessageLength()
{
    return msgStrByteCount;
}

/**
 * @brief Create a Message object
 * 
 * @param interactiveBoard 
 * @param pixelCount 
 * @param msgStr 
 * @param msgStrByteCount 
 * @return true if a new message is created
 * @return false if no new message is created
 */
bool CreateMessage(CubeModule_t interactiveBoard[], int pixelCount, uint8_t msgPayload[])
{
    bool reportMessageFlag = SENSOR_VALUE_NOT_UPDATED;
    if (msgStrByteCount != 0)
    {
        for (int i = 0; i < pixelCount; i++)
        {
            // DBG_PRINT(F("Sensor at pxl "));
            // DBG_PRINT(i);
            if (interactiveBoard[i].sensorStatus == SENSOR_ACTIVE)
            {
                // DBG_PRINT_LN(F(" is Active"));
                bitSet(msgPayload[i / 8], 7 - i % 8);
            }
            else
            {
                // DBG_PRINT_LN(F(" is Inactive"));
                bitClear(msgPayload[i / 8], 7 - i % 8);
            }            
            reportMessageFlag |= interactiveBoard[i].sensorStateUpdateFlag;
            interactiveBoard[i].sensorStateUpdateFlag = SENSOR_VALUE_NOT_UPDATED;
        }
    }
    #ifdef DEBUG_ENABLE
    if(reportMessageFlag == SENSOR_VALUE_UPDATED)
    {
        DBG_PRINT_LN(F("CreateMessage >> Msg created"));
    }
    #endif /* DEBUG_ENABLE */
    return (reportMessageFlag == SENSOR_VALUE_UPDATED);
}

/**
 * @brief 
 * 
 * @param interactiveBoard 
 * @param pixelCount 
 * @param msgPayload 
 */
void ParseMessage(CubeModule_t interactiveBoard[], int pixelCount,uint8_t msgPayload[],int size)
{
    if(size != msgStrByteCount)
    {        
        DBG_PRINT_LN(F("ParseMessage >> size mismatch"));
        return;
    }
    bool bitInfo = true;
    for (int i = 0; i < pixelCount; i++)
    {
        bitInfo = (bitRead(msgPayload[i / 8], 7 - i % 8) == 1);
        if( interactiveBoard[i].actuationActivated != bitInfo)
        {
            interactiveBoard[i].actuationActivated = bitInfo;
            DBG_PRINT(F("Actuator at pxl "));
            DBG_PRINT(i);
            DBG_PRINT(F(" is "));
            DBG_PRINT((bitInfo?F("Active "):F("Inactive ")));
            if(interactiveBoard[i].actuatorMode == DEACTIVE_MODE)
            {
                // DO NOTHING
                DBG_PRINT_LN(F("deactive mode"));
            }
            else if(interactiveBoard[i].actuationActivated == false)
            {
                interactiveBoard[i].actuatorMode = DEAD_LOW_MODE;
                DBG_PRINT_LN(F("dead low "));
            }
            else if(interactiveBoard[i].actuatorMode != RAMP_MODE_UP && 
                interactiveBoard[i].actuatorMode != RAMP_MODE_DOWN &&
                interactiveBoard[i].actuatorMode != ACTIVE_HIGH_MODE && 
                interactiveBoard[i].actuatorMode != ACTIVE_HIGH_STANDBY_MODE)
            {
                DBG_PRINT_LN(F("ramp mode "));
                interactiveBoard[i].actuatorMode = RAMP_MODE_UP;
            } 
             
        }    
    }
}

/* EOF */