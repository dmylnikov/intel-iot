#define AUTO 2

//------------------------------------------------------------------------------
//                              GENUINO PART CONSTANTS
//------------------------------------------------------------------------------
int LED_PIN = 13;     //П
int state = LOW;
int LED_state = LOW;
int buttonState = 0;
int ButtonPIN = 0;
int change_was = 0;
int Light_value = 1023;
int A_L_LED_C = 0;  // Automatic lighting start
int valuePIN2 = 0;
int ugranichnoe_lighting = 530;


#include <CurieBLE.h>
#include <limits.h>


//------------------------------------------------------------------------------
//                              SERVER SENT CONSTANTS
//------------------------------------------------------------------------------
#define S_LOW 'l'
#define S_HIGH 'h'
#define S_AUTO 'a'

//------------------------------------------------------------------------------
//                                  BT CONSTANTS
//------------------------------------------------------------------------------
BLEPeripheral blePeripheral; // create peripheral instance

BLEService ledService("19B10110-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLECharCharacteristic ledCharacteristic("19B10111-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//------------------------------------------------------------------------------
//                              MULTITASKING CONSTANTS
//------------------------------------------------------------------------------
unsigned long previousMillis = 0;        // will store last time 
#define DELAY 100

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
  
 
  char _value = ledCharacteristic.value();
  Serial.println(_value);
  switch(_value) {
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

//------------------------------------------------------------------------------
//                                  SETUP SECTION
//------------------------------------------------------------------------------

void setup_bt() {
  // set the local name peripheral advertises
  blePeripheral.setLocalName("GLC");
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
  ledCharacteristic.setValue('l');

  // advertise the service
  blePeripheral.begin();
  Serial.println(("Bluetooth device active, waiting for connections..."));
}

void setup_rest()
{
  pinMode(2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);              // порт как выход
  attachInterrupt(ButtonPIN, work_with_button, LOW); // привязываем 0-е прерывание к функции blink().
}

void setup() {
  Serial.begin(9600);
  while (!Serial); 
  setup_bt();
  setup_rest();
}

//------------------------------------------------------------------------------
//                                GENUINO FUNCTIONS
//------------------------------------------------------------------------------

void change_position_LED (){    // LED position changed to the opposite
  if(LED_state == HIGH)  LED_state = LOW;
  else                   LED_state = HIGH;
  change_was = 1;
}

void check_button() {      // Cutting chatter 
    delay(30);
    buttonState = digitalRead(ButtonPIN);
    if(buttonState == LOW) change_was = 1;
    else                   change_was = 0;  
}

void work_with_button()    // Cutting chatter 
{
  detachInterrupt(ButtonPIN);
  A_L_LED_C = 0;
  change_position_LED();
  LED_control();
  delay(30);
  while(change_was) check_button();    
  attachInterrupt(ButtonPIN, work_with_button, LOW);
}

void LED_control (){              // Turn ON/OFF LED
  if(LED_state == HIGH)  digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  else                   digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
}

void switch_state (int work_Mode) { 
    if (work_Mode == AUTO)
        A_L_LED_C = 1;
        
    else {
      A_L_LED_C = 0; 
      LED_state = work_Mode;
    }
    LED_control();
 }


void lighting_LED_control () {
  Light_value = analogRead(A0);
  if(Light_value < ugranichnoe_lighting ) LED_state = LOW;
  else                  LED_state = HIGH;
  LED_control();
}

//------------------------------------------------------------------------------
//                                  ACTIONS SECTION
//------------------------------------------------------------------------------
void do_bt() {
  // poll peripheral
  blePeripheral.poll();
}

void do_rest() {
  if (A_L_LED_C) 
    lighting_LED_control();
  
  valuePIN2 = !valuePIN2;
  digitalWrite(2, valuePIN2);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis < previousMillis)
    previousMillis = ULONG_MAX - previousMillis; // in case of overflow
    
    
  do_bt();


  // a kind of priority
  if (currentMillis - previousMillis >= DELAY) {
    do_rest();
    previousMillis = currentMillis;
  }
}



