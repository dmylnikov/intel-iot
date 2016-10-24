/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

#include <CurieBLE.h>
#include <limits.h>


//------------------------------------------------------------------------------
//                              SERVER SENT CONSTANTS
//------------------------------------------------------------------------------
#define S_LOW 0
#define S_HIGH 1
#define S_AUTO -1

//------------------------------------------------------------------------------
//                                  BT CONSTANTS
//------------------------------------------------------------------------------
BLEPeripheral blePeripheral; // create peripheral instance

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEIntCharacteristic ledCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//------------------------------------------------------------------------------
//                              MULTITASKING CONSTANTS
//------------------------------------------------------------------------------
unsigned long previousMillis = 0;        // will store last time 
#define DELAY 1000;

//------------------------------------------------------------------------------
//                                  SETUP SECTION
//------------------------------------------------------------------------------

void setup_bt() {
  // set the local name peripheral advertises
  blePeripheral.setLocalName("GenuinoLampConroller");
  // set the UUID for the service this peripheral advertises
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(ledCharacteristic);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  ledCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
// set an initial value for the characteristic
  ledCharacteristic.setValue(0);

  // advertise the service
  blePeripheral.begin();
  Serial.println(("Bluetooth device active, waiting for connections..."));

 
}
void setup() {
  Serial.begin(9600);
  while (!Serial); 
  setup_bt();
  setup_rest();
}


//------------------------------------------------------------------------------
//                                  ACTIONS SECTION
//------------------------------------------------------------------------------
void do_bt() {
  // poll peripheral
  blePeripheral.poll();
}



void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis < previous_millis)
    previous_millis = ULONG_MAX - previous_millis; // in case of overflow
    
    
  do_bt();


  // a kind of priority
  if (currentMillis - previousMillis >= DELAY)) {
    lighting_LED_control();
    previous_MilliscurrentMillis
}


//------------------------------------------------------------------------------
//                                  EVENT HANDLERS
//------------------------------------------------------------------------------
void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");
  
 
  int _value = ledCharacteristic.value();
  Serial.print(_value);
  switch(value) {
    case S_HIGH: {
      switch_state(HIGH);
      break;
    }
    case S_LOW: {
      switch_state(LOW);
      break;
    }
    case S_AUTO: {
      switch_state(AUTO);
      break;
    }
  }
 
}
