/**
 ***************************************************************************************************
 * This file is part of WE Sensors SDK:
 * https://www.we-online.com/sensors
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE THAT WÜRTH ELEKTRONIK
 * EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND RELATED TO, BUT NOT LIMITED
 * TO THE NON-INFRINGEMENT OF THIRD PARTIES’ INTELLECTUAL PROPERTY RIGHTS OR THE
 * MERCHANTABILITY OR FITNESS FOR YOUR INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT
 * WARRANT OR REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT
 * RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO ANY
 * COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION PUBLISHED BY
 * WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE
 * FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR ENDORSEMENT
 * THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
 *
 * COPYRIGHT (c) 2021 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/


/*
 * WSEN-PADS  communication check
 * 
 * Initializing and reading the device ID for this sensor
 * 
 * The SAO pin is here connected to positive supply voltage.
 * 
 */

//***************************************************************************************
#include <Arduino.h>
#include <Wire.h>
    #define I2C_SCL  3
    #define I2C_SDA 10
//***************************************************************************************

#include "WSEN_PADS.h"

Sensor_PADS sensor;

void setup() {

  delay(5000);
  Serial.begin(115200);

  // Initialize the I2C interface
  sensor.init(PADS_ADDRESS_I2C_1);

  // Get the device ID for this sensor
  int sensor_ID = sensor.get_DeviceID();
  Serial.print("Sensor ID: ");

  // Print the device ID in hexadecimal on the serial monitor
  Serial.print(sensor_ID, HEX);
  Serial.println();

  // Check if the determined device ID matches the correct device ID (->A0) of this sensor
  if (sensor_ID == PADS_DEVICE_ID_VALUE)
  {
    Serial.println("Communication successful !");
  } else
  {
    Serial.println("No communication with the device !");
  }

  //***************************************************************************************
    Wire.begin(I2C_SDA, I2C_SCL);
  ////// Configure PADS CTRL_1 BDU bit /////  
    Wire.beginTransmission(PADS_ADDRESS_I2C_1);  //     PADS(I2CADDR) 0x5D
    Wire.write(0x10);  // write to PADS register  CTRL-1 REG    0x10
    Wire.write(0x02);  // write to PADS CTRL-1 REG (00000010b)(BDU_bit1=1) udpdate when msb/lsb read
    Wire.endTransmission();
  ////// Configure PADS CTRL_2 ONE_SHOT bit /////
    Wire.beginTransmission(PADS_ADDRESS_I2C_1);  //     PADS(I2CADDR) 0x5D
    Wire.write(0x11);  // write to PADS register  CTRL-2 REG    0x11
    Wire.write(0x11);  // write to PADS CTRL-2 REG (00010001b)(ONE_SHOT_bit0=1) Enable ONE_SHOT 
    Wire.endTransmission();    
//***************************************************************************************
// NOTE, ArduinoPlatform.cpp has also been altered to use  I2C_SCL  3, I2C_SDA 10   
//***************************************************************************************
}

void loop() {

   // Check if sensor is ready to measure the temperature
  if (sensor.temp_ready_to_read() & sensor.pressure_ready_to_read())
  {
    // Read and calculate the temperature
    float myTemperature = sensor.read_temperature();
    Serial.print("The temperature is: ");

    // Print the temperature on the serial monitor
    Serial.print(myTemperature);
    Serial.println(" Celsius");

    // Read and calculate the pressure
    float myPressure = sensor.read_pressure();
    Serial.print("The pressure is: ");

    // Print the pressure on the serial monitor in kPa and mbar
    Serial.print(myPressure);
    Serial.println(" kPa");
    Serial.print(myPressure * 10.0);	// convert to mbar
	  Serial.println(" mbar");
  }
  else
  {
    Serial.print("Sensor is not ready.");
  }

  delay(10000); // Waste 10 Seconds

  //sensor.set_single_conversion();DOES NOT WORK ?
  ////// Configure PADS CTRL_2 ONE_SHOT bit /////
    Wire.beginTransmission(PADS_ADDRESS_I2C_1);  //     PADS(I2CADDR) 0x5D
    Wire.write(0x11);  // write to PADS register  CTRL-2 REG    0x11
    Wire.write(0x11);  // write to PADS CTRL-2 REG (00010001b)(ONE_SHOT_bit0=1) Enable ONE_SHOT 
    Wire.endTransmission();

}