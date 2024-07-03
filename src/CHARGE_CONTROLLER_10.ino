#include <Arduino.h>

#include <SwitchManager.h>

#include <ModbusMaster.h>
#include <LowPower.h>
#include <uUnixDate.h>
//#include <Button.h>
#include <MultiMap.h>
#include <AltSoftSerial.h>    // Second serial port for connecting to the BMS
#include <Wire.h>
#include <SPI.h>              // Needed for the display
#include <M2M_LM75A.h>
#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_ADS1X15.h>
#include <PinChangeInterrupt.h>
#include "config.h"

enum ChargerStates {IdleSleep, IdleAwake, Charging, PostCharge};
unsigned char ChargerState = IdleSleep;  // What the controller is doing at any time.

ChargerStates chargerState;

//LiFeP04 Voltage curve..........................//

uint8_t in[] = {
  100,110,120,128,129,130,131,132,133,134,136,142
};

uint8_t out[] = {
  0,7,10,20,30,40,50,60,70,80,90,100
};

int sz = 12;

//................................................//

//Button button1(6);
//Button buttonStop(10);
//Button buttonMenu(11);


////////////////
#define DEBUG true
//define DEBUG false
////////////////

// instantiate ModbusMaster object
ModbusMaster node;
//AltSoftSerial MySoftSerial(8,9); // RX, TX 
AltSoftSerial mySerial;
Adafruit_ADS1115 ads1115;
// I2C Peripherals 
M2M_LM75A lm75a;
LiquidCrystal_PCF8574 lcd(0x26);  // set the LCD address to 0x27
//LiquidCrystal_PCF8574 lcd1(0x26);  // set the LCD address to 0x23

//--------------------------------------------------------------------------------

void setup() {

  //Pin assignments From top LHS
  pinMode(pirIN, INPUT_PULLUP);         // PIR Module  
  pinMode(clockIN, INPUT_PULLUP);     // wake on change, timing for power monitors
  pinMode(chrgLED, OUTPUT);            // charging_LED
  pinMode(LEDchrgd,OUTPUT);            // complete_LED
//  pinMode(chrgrON ,OUTPUT);            // DC24_EN
  pinMode(buttonStart , INPUT_PULLUP);    // Charge Start IOC
  pinMode(EnblRS485 , OUTPUT);         // Charge Start IOC
  // AltSerialRx/
  // AltSerialTx

  //From top RHS
  pinMode(balON ,OUTPUT);               // 17 A3 battery active balancer_EN
  pinMode(Enble24V ,OUTPUT);             // 18
  pinMode(Enble05V ,OUTPUT);             //  
  pinMode(buttonMenu, INPUT_PULLUP);     // Switch Input
  pinMode(buttonStop, INPUT_PULLUP);     // Switch Input

 // button1.begin();
  //buttonStop.begin();
 // buttonMenu.begin();


  attachInterrupt(digitalPinToInterrupt(clockIN), clockISR, FALLING);
  StartSwitch.begin (startSwitch, handleStartPress);
  StopSwitch.begin (stopSwitch, handleStopPress);
  MenuSwitch.begin (menuSwitch, handleMenuPress);


#ifdef DEBUG
  mySerial.begin(19200);
#endif

  Serial.begin(115200);
  while (!Serial){;}

  // EPEver Device ID 1
  node.begin(1, Serial);

  // Callbacks
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  currentMillis = millis();     //initial star

  // Callbacks
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  currentMillis = millis();     //initial start time
  EpeverPrevious = currentMillis;  //sync 
  LCDprevious = currentMillis;  //sync
  //startMillis = currentMillis;  //time
  EpeverPrevious = currentMillis;  //sync 
  LCDprevious = currentMillis;  //sync
  //startMillis = currentMillis;
  firstWake = true;
  //sleep = false;
  //sleep = true;
  mySerial.println("Setup Complete");

}

//____________ISRS____________________________________
void changeISR(){
  // Switch to the idleAwake
  chargerState = ChargerStates::IdleAwake;
  firstWake = true;
  detachInterrupt(digitalPinToInterrupt(buttonStart));
  detachInterrupt(digitalPinToInterrupt(pirIN));
}

void clockISR(){
  //count wakes from sleep routine
    if (clockCount >= counts){
      flgReadSolar = true;
      clockCount = 0;
      detachInterrupt(digitalPinToInterrupt(buttonStart));
      detachInterrupt(digitalPinToInterrupt(pirIN));
    }
    clockCount++;
    //clockINT = true;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
// flag to poll in awake code 
}

void Menu(){

}
//void stp_Chrg(){
//  currentMillis = millis();
//  startMillis = currentMillis;
//}

//____________MAIN PROGRAM LOOP________________________________________
void loop() {

  //update switches and states here..
  //delay(100);
  MenuSwitch.check ();  // check for presses
  if (flgReadSolar == true){
    ReadingSolar();
    flgReadSolar = false;
  }
  currentMillis = millis();     //initial star

  switch (chargerState) {

    case ChargerStates::IdleSleep: // read solar charge waiting for ISR Inputs
    #ifdef DEBUG
      mySerial.println("IdleSleep");
    #endif
      SleepSetup();
      //sleep_setup(); // turns off serial ports  
      attachPCINT(digitalPinToPCINT(buttonStart), changeISR, FALLING);
      attachInterrupt(digitalPinToInterrupt(pirIN), changeISR, RISING);
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
      break;
  
    case ChargerStates::IdleAwake:
      StartSwitch.check ();  // check for presses

      if(firstWake == true){
      #ifdef DEBUG
        mySerial.println(F("first IdleAwake"));
      #endif
        IdleSetup();
        idleStartTime = currentMillis;        
        displayUpdate = currentMillis - displayDelay; // immediate update
        firstWake = false;
      }

      if(currentMillis - displayUpdate >= displayDelay){
        
        IdleDisplay();
        displayUpdate = currentMillis;
      }
          
      if (currentMillis - idleStartTime >= idleDelay){
        chargerState = ChargerStates::IdleSleep; // 
        break;
      } 
      else {
        break;
      }
    

    case ChargerStates::Charging:
      StopSwitch.check  ();

      if(firstCharge == true){
      #ifdef DEBUG
        mySerial.println("Charging");
      #endif

      chargeStartTime = currentMillis;
      ChargeSetup();
      firstCharge = false;
      }
      

      if(currentMillis - displayUpdate >= displayDelay){
        ChargeDisplay();
        displayUpdate = currentMillis;
      }

      if(currentMillis - chargeStartTime >= chargeDelay){
        postChargeStartTime = millis();
        chargerState = ChargerStates::PostCharge ; // 
        break;
      } 
      else {
        break;
      }

    case ChargerStates::PostCharge:
      if(firstPostCharge == true){
      #ifdef DEBUG
        mySerial.println("PostCharge");
      #endif
        postChargeStartTime = currentMillis;
        PostChargeSetup();
        firstPostCharge = false;
      }
      if(currentMillis - postChargeStartTime >= postChargeDelay){
        chargerState = ChargerStates::IdleSleep ; // Advance to the next state
        break;
      } 
      else {
        break;
      }

    default:
      chargerState = ChargerStates::IdleSleep;
      break;

  }

}
/*
  for (clockCount = 0;  clockCount <= counts; clockCount++ ){
      //ClearDisplay();
      sleep_setup(); // turns off serial ports
      attachPCINT(digitalPinToPCINT(buttonStart), changeISR, FALLING);
      attachInterrupt(digitalPinToInterrupt(pirIN), changeISR, RISING);
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
      detachPCINT(digitalPinToPCINT(buttonStart));
      detachInterrupt(digitalPinToInterrupt(pirIN));
      mySerial.print("count ten");
      mySerial.println(i);

      // Idle - update columb count and back to sunsigned long timerMillis;  // For counting time incrementsleep
      RS485_setup(); // turns on serial ports 
      delay(20);
      read_values();  
      UpdateDateTime();
      //MapBatteryVolts();
      //ColoumbCount();
      debug_print();
      delay(100);
  }
      

  if(firstWake == true){
    currentMillis = millis();
    startMillis = currentMillis;      
    Idle_Setup();
    pre_charging();
    DisplayInit();
    DisplayDemo();e){
    currentMillis 
    firstWake = false;      
  }

  if(sleep == false && clockCount > counts){
    read_values();  
    UpdateDateTime();
    //ColoumbCount(); // BUG!
    debug_print();  // this causes LCD flicker
    //delay(50);
    clockCount = 0;
  }

  if(sleep == false){
    currentMillis = millis();
    if(currentMillis - startMillis >= SleepDly){
      sleep = true;
    }
    mySerial.print(clockCount);
    delay(20);
  }
   
*/

//ISR handling routines



 
    //mySerial.println("sleep stop");
    /*
    if (pirORstrtChrgINT == true){
      currentMillis = millis();
      SleepDue = currentMillis; // 
      MeausureDue = currentMillis; // meausure after delay
      pirORstrtChrgINT = false;
      pre_charging();
      mySerial.println("in interrupt true");
      mySerial.flush();
    }
    */
  

  /*
  currentMillis = millis();
  if (currentMillis - MeausureDue >= MeausureDly) {
    MeausureDue = currentMillis;
    //SleepDue = currentMillis;
    mySerial.println("meausure loop");
    mySerial.flush();
    pre_charging();
    read_values();
    BattDischMon();   // reads ADC and calcualates load currents,
    //read_batterySOC();
    errorCheck();       //Check error count and if it exceeds 5 reset modbus
    post_charging();    
    debug_print();
    mySerial.flush();
    //updateDisplay();
    delay(100);    
  }
  
  if (currentMillis - LCDpagePrev >= LCDpageDly) {
    LCDpagePrev = currentMillis;  // save LCD update time
    LCDprevious = currentMillis;  // avoid duplicate update 
    lcd.clear();
    if(page_counter <3){
      page_counter = page_counter +1;
    }  else {
      page_counter=1;
    }
  }
  */





