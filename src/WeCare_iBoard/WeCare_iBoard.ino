/**
 * @file WeCare_iBoard.ino
 * @author Team Hugo
 * @brief WeCare_iBoard is a project developed for remote interaction between two or more people using 
 * a interactive board.
 * 
 * Software Modules used:
 * DebugPrint : For debug related functionalities
 * CubeModule : Provide a single point data point for all the info related to modular peice designed
 * Sensor : 
 * 
 * Libraries used:
 * Servo by Michael Margolis, Arduino version 1.1.7
 * Adafruit NeoPixel version 1.7.0
 * 
 * 
 * @version 0.1
 * @date 2020-12-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*
TODO: 
  Reschedule watchdog
  Additional serial for ESP communication
  Use of CBOR for data transfer or custom protocol
  MQTT feasiblity
  EEPROM module for the actuator calibration information
  NEOPIXEL module
*/

#include <avr/wdt.h>

// comment the following line to disable the Debug printing over the UART
#define DEBUG_ENABLE
#include "DebugPrint.hpp"

#include "CubeModule.hpp"
#include "NetworkCommunicationModule.hpp"

// Interactive Board 
CubeModule_t interactiveBoard[16];

// pixel count of the interactive board
int pixelCount = sizeof(interactiveBoard)/sizeof(CubeModule_t);

/**
 * @brief setup function to intialise all the components
 * 
 */
void setup() {
  // Disabled watchdog timer for testing 
  wdt_disable();
  // enable watchdog timer
  //wdt_enable(WDTO_8S);
  DBG_BEGIN(115200);
  EstablishedInterBoardConnection();
  CubeModuleInitialise(interactiveBoard,pixelCount);
}

/**
 * @brief loop function
 * 
 */
void loop() {
  CubeTaskRunner(interactiveBoard,pixelCount);
  ConnectionTaskRunner();
}


/* EOF */