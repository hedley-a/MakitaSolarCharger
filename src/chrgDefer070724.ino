#include <Arduino.h>

//CHANGE SUMMARY// Scroll down to bottom of this page

//#include <avr/wdt.h> /* Header for watchdog timers in AVR */
#include <ModbusMaster.h>
#include <LowPower.h>
#include <uUnixDate.h>
#include <stdio.h>
#include <MultiMap.h>
//#include <AltSoftSerial.h>    // Second serial port for connecting to the BMS
//#include <Wire.h>     //https://docs.particle.io/reference/device-os/api/wire-i2c/stretchclock/
#include <SPI.h>              // Needed for the display
#include <M2M_LM75A.h>
#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_ADS1X15.h>
#include "config.h"

enum ChargerStates {Sleep, Idle, IdleMenu, ChargeDefer, ChargeDeferMenu, Charge, ChargeMenu, ChargeOverHeat, ChargeOverHeatMenu, PostCharge, PostChargeMenu,};
unsigned char ChargerState = Idle;  // What the controller is doing at any time.

ChargerStates chargerState;

//Battery map LiFeP04 Voltage curve..........................//

uint8_t in[] = {
  100,110,120,128,129,130,131,132,133,134,136,142
};

uint8_t out[] = {
  0,7,10,20,30,40,50,60,70,80,90,100
};

int sz = 12;

//................................................//

#define SET_PIN_MODE_OUTPUT(port, pin) DDR ## port |= (1 << pin)
#define SET_PIN_HIGH(port, pin) (PORT ## port |= (1 << pin))
#define SET_PIN_LOW(port, pin) ((PORT ## port) &= ~(1 << (pin)))


////////////////
//#define DEBUG 
////////////////

// instantiate ModbusMaster object
ModbusMaster node;
//AltSoftSerial MySoftSerial(8,9); // RX, TX 
//AltSoftSerial mySerial;


// I2C Peripherals 
M2M_LM75A lm75a;
Adafruit_ADS1115 ads1115;
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27

//--------------------------------------------------------------------------------

void setup() {

  //Pin assignments From top LHS
  pinMode(pirIN, INPUT_PULLUP);         // PIR Module  
  pinMode(clockIN, INPUT_PULLUP);     // wake on change, timing for power monitors
  pinMode(chrgLED, OUTPUT);            // charging_LED
  pinMode(LEDchrgd,OUTPUT);            // complete_LED
  pinMode(buttonStart , INPUT_PULLUP);    // Charge Start IOC
  pinMode(buttonStop, INPUT_PULLUP);     // Switch Input

  //From top RHS
  pinMode(EnblRS485 , OUTPUT);         // Charge Start IOC
  pinMode(balON ,OUTPUT);               // 17 A3 battery active balancer_EN
  pinMode(Enble24V ,OUTPUT);             // 18    
  pinMode(buttonMenu, INPUT_PULLUP);     // Switch Input
  // ??
  pinMode(Enble05V ,OUTPUT);             //

  Serial.begin(115200);
  while (!Serial){;}

  digitalWrite(Enble05V, HIGH);
  digitalWrite(EnblRS485, LOW);           
  digitalWrite(chrgLED, LOW);            // charging_LED
  digitalWrite(LEDchrgd, LOW);            // charging_LED
  delay(100);
  lm75a.begin();
  ads1115.begin(0x49);                // Initialize ads1115 at address 0x49
  ads1115.setGain(GAIN_ONE);          // 1x gain   +/- 4.096V  1 bit = 2mV
  lcd.begin(20,4);
  lcd.clear(); 
  lcd.setBacklight(32);
  //wdt_disable();  // Disable the watchdog and wait for more than 2 seconds 

  delay(1000);  // Done so that the Arduino doesn't keep resetting infinitely in case of wrong configuration */
  
  //wdt_enable(WDTO_8S);  // Enable the watchdog with a timeout of 2 seconds

  //digitalWrite(EnblRS485, HIGH);            // charging_LED
  digitalWrite(chrgLED, HIGH);            // charging_LED
  digitalWrite(LEDchrgd, HIGH);            // charging_LED

  attachInterrupt(digitalPinToInterrupt(clockIN), clockISR, FALLING); 
  attachInterrupt(digitalPinToInterrupt(pirIN), pirISR, FALLING);

  // init modbus in receive mode
  pinMode(MAX485_RE, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  postTransmission();


  //EPEver Device ID 1
  node.begin(1, Serial);

 //Callbacks
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  firstState = true;
  chargerState  = ChargerStates::Idle;
  Serial.println("Setup Complete");
  delay(50);

  SET_PIN_MODE_OUTPUT(B, 1);  // Set Pin9 (PB1) As Output
}
//_________ISRS____________________________________
void pirISR(){
  pirIntFlg = true;
}

void clockISR(){
  if (readDataCount >= 10 ){
    clkIntFlg = true;
    readDataCount = 0;
  }
  readDataCount++;    
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

//https://github.com/e-tinkers/button

bool startDebounce() {
  static uint16_t state = 0;
  state = (state<<1) | digitalRead(buttonStart) | 0xfe00;
  return (state == 0xff00);
}

bool stopDebounce() {
  static uint16_t state = 0;
  state = (state<<1) | digitalRead(buttonStop) | 0xfe00;
  return (state == 0xff00);
}

bool menuDebounce() {
  static uint16_t state = 0;
  state = (state<<1) | digitalRead(buttonMenu) | 0xfe00;
  return (state == 0xff00); 
}

bool pirDebounce() {
  static uint16_t state = 0;
  state = (state<<1) | digitalRead(pirIN ) | 0xfe00;
  return (state == 0xff00); 
}


//____________MAIN PROGRAM LOOP________________________________________
void loop() {

  // Read sensors:
  if(clkIntFlg == true){
    SET_PIN_LOW(B, 1);
    ReadEpeverClock();   // - this hangs for seconds if comms fails - MODBUS library?? 
    ControllerTemperature();
    ReadEpeverData();
    EpeverError(); 
    //Serial.println("");
    //Serial.print("reading data");
    //delay(50);   
    CheckFullCharge();   // is solar zero pwr for 10 min?
    EnergyBalance();     // sums the figures
    DischargePowers();   // updates TotalPower     
    MapBatteryVolts();
    SET_PIN_HIGH(B, 1); 
    clkIntFlg = false;
    Serial.println("");
    //debugPrint();  //Serial.print("reading data");
    //delay(50);  

  }
 

  delay(5);  // loop time = debounce delay!! 
  currentMillis = millis(); //sync all timing ..
  // wdt_reset();  /* Reset the watchdog */


  
  //--------------------------------------------------------------------
  switch (chargerState) {
  
    //----------------------------------------------------------------------------
    case ChargerStates::Sleep:
      break;

    case ChargerStates::Idle:

      if(firstState == true){
        IdleSetup();
        IdleDisplay();
        FirstStateFunction();
        stateTimeout = idleTimeout; // 30 seconds        
      } 

      if (pirDebounce()){ 
        stateStartTime = currentMillis;  //reset sleep delay
        lcd.setBacklight(32);      
      } 

      if (menuDebounce()){
        chargerState = ChargerStates::IdleMenu;
        firstState = true;     
      }

      if (startDebounce()&& currentMillis > buttonInhibit + 1000){ //prevent charge on wake
        //SyncStateStart();
        stateStartTime = currentMillis;
        if (temperatureInternal > temperatureDefer){
        chargerState = ChargerStates::ChargeDefer; //
        }else{
        chargerState = ChargerStates::Charge; //        
        }
        firstState = true;
        detachInterrupt(0);                     // don't want this . 
        //startButtonPress = false; 
      }

      if (currentMillis >= displayUpdate+1000){
        IdleDisplay();  // set menu active here
        displayUpdate = currentMillis;
      }

      if(currentMillis > stateStartTime + stateTimeout){
        stateStartTime = currentMillis - stateTimeout;
        //chargerState = ChargerStates::Sleep ; //
        lcd.setBacklight(00);
       // chargerState = ChargerStates::IdleAwake ;
       // lcd.setBacklight(00);
       // firstState = true; 
      }
        break;

      case ChargerStates::IdleMenu:
        MenuFunction(); // common menu stuff
        if(currentMillis > menuStart + menuTimeout){
        chargerState = ChargerStates::Idle;
        }
        break;       
    //-----------------------------------------------------------------------------
    case ChargerStates::ChargeDefer:
      if(firstState == true){   
        //DeferSetup();
        DeferDisplay();
        FirstStateFunction();   
        lcd.setBacklight(32);  
        stateTimeout = deferTimeout; // 24 hours 
      } 

      if (pirDebounce()){ 
        lcd.setBacklight(32);      
      } 

      if (menuDebounce()){
        chargerState = ChargerStates::ChargeDeferMenu;
        firstState = true;     
      }

      if (stopDebounce()&& currentMillis > buttonInhibit + 1000){ //prevent charge on wake
        //SyncStateStart();
        stateStartTime = currentMillis;
        chargerState = ChargerStates::Idle; //
        firstState = true;
        detachInterrupt(0);                     // don't want this . 
        //startButtonPress = false; 
      }

      if (currentMillis >= displayUpdate+1000){
        DeferDisplay();  // set menu active here
        digitalWrite(chrgLED, !digitalRead(chrgLED ));
        displayUpdate = currentMillis;
      }

      if (temperatureInternal < temperatureStart){
        chargerState = ChargerStates::Charge; //
        firstState = true;
        }

      if(currentMillis > stateStartTime + stateTimeout){
        chargerState = ChargerStates::Idle; //
        firstState = true; 
      }
      break;

    case ChargerStates::ChargeDeferMenu:
      MenuFunction(); // common menu stuff
      if(currentMillis > menuStart + menuTimeout){
        chargerState = ChargerStates::ChargeDefer;
      }
      break;

    case ChargerStates::Charge:
 
      //digitalWrite(Enble24V , HIGH);

      if(firstState == true){
        ChargeSetup();
        ChargeDisplay();
        FirstStateFunction();     
        startWattHours = demandWattHours;        // register Wh at charge start
        stateTimeout = chrgTimeout;  // 140 mins.        
        loadIdleDly = 0;
      }

      if (stopDebounce()&& currentMillis > buttonInhibit + 1000){
        CompletionDateTime();
        chargerState = ChargerStates::PostCharge ; //
        firstState = true;     
      } 

      if (menuDebounce()){
        chargerState = ChargerStates::ChargeMenu;
        firstState = true;     
      }

      if (currentMillis >= displayUpdate+1000) {

        if(loadPower < 1500){
          loadIdleDly ++;
        } else {
          loadIdleDly = 0;
        }
        if (loadIdleDly >= 40){  
          CompletionDateTime();        
          chargerState = ChargerStates::PostCharge ; //
          firstState = true;
        }

        HrsMinsSecs(currentMillis-stateStartTime);
        ChargeDisplay();          
        if (currentMillis < stateStartTime + 20000){
          DischargePowers(); // update during first 2o secs of charge
        }
        displayUpdate = currentMillis;
      }

      if (temperatureInternal > temperatureOver){
        chargerState = ChargerStates::ChargeOverHeat; //
        firstState = true;
        }

      if(currentMillis > stateStartTime + stateTimeout){
        CompletionDateTime();        
        chargerState = ChargerStates::PostCharge ; //
        //charge = false;
        firstState = true; 
      }      
        break;

      case ChargerStates::ChargeMenu:
      MenuFunction(); // common menu stuff
      if(currentMillis > menuStart + menuTimeout){
        chargerState = ChargerStates::Charge;
      }
      break;

    case ChargerStates::ChargeOverHeat:
        if(firstState == true){ 
        digitalWrite(Enble24V , LOW);             // 18   
        DeferDisplay();
        //stateStartTime = currentMillis; use Charge start  
        displayUpdate = currentMillis+1000;  
        lcd.setBacklight(32);  
        stateTimeout = overTimeout; // 12 hours
        firstState = false;  
      } 

      if (pirDebounce()){ 
        lcd.setBacklight(32);      
      } 

      if (menuDebounce()){
        chargerState = ChargerStates::ChargeOverHeatMenu;
        firstState = true;     
      }

      if (stopDebounce()){ //prevent charge on wake
        //SyncStateStart();
        stateStartTime = currentMillis;
        chargerState = ChargerStates::Idle; //
        firstState = true;
        //startButtonPress = false; 
      }

      if (currentMillis >= displayUpdate+1000){
        DeferDisplay();  // set menu active here
        digitalWrite(chrgLED, !digitalRead(chrgLED ));
        displayUpdate = currentMillis;
      }

      if (temperatureInternal < temperatureStart){
        chargerState = ChargerStates::Charge; //
        digitalWrite(Enble24V , HIGH);             // 18 
        //firstState = true;
        }

      if(currentMillis > stateStartTime + stateTimeout){
        chargerState = ChargerStates::Idle; //
        firstState = true; 
      }

      break;
    
    case ChargerStates::ChargeOverHeatMenu:
      MenuFunction(); // common menu stuff
      if(currentMillis > menuStart + menuTimeout){
        chargerState = ChargerStates::ChargeOverHeat;
      }
      break;


    //--------------------------------------------------------------------------------- 
    case ChargerStates::PostCharge:
       
      if(firstState == true){  
        PostChargeSetup();
        PostChargeDisplay();
        FirstStateFunction();
        stateTimeout = postTimeout;
      }

      if (pirDebounce()){ 
        stateStartTime = currentMillis;  //reset sleep delay
        lcd.setBacklight(32);                 
      } 

      if (menuDebounce()){
        chargerState = ChargerStates::PostChargeMenu;
        firstState = true;            
      }

      if (stopDebounce()&& currentMillis > buttonInhibit + 1000){
        //SyncStateStart();
        stateStartTime = currentMillis;
        chargerState = ChargerStates::Idle ; //
        firstState = true;

      } 

      //display refresh periodically
      if (currentMillis >= displayUpdate+1000) {
        //HrsMinsSecs(); Time 
        PostChargeDisplay();
        displayUpdate = currentMillis;
      }

      if(currentMillis > stateStartTime + stateTimeout){
         stateStartTime = currentMillis - stateTimeout;
      //  chargerState = ChargerStates::IdleAwake ; //
          lcd.setBacklight(00);
      //  firstState = true; 
      } 
      break;

    case ChargerStates::PostChargeMenu:
      MenuFunction(); // common menu stuff
      if(currentMillis > menuStart + menuTimeout){
        chargerState = ChargerStates::PostCharge;
      }
      break;

    default:
      chargerState = ChargerStates::Idle;
      break;

  }
  
} //end of main loop

void FirstStateFunction(){
  lcd.setBacklight(32);  
  stateStartTime = currentMillis;  
  displayUpdate = currentMillis+1000;        
  buttonInhibit = currentMillis;
  firstState = false;
}

void MenuFunction(){
  if(firstState == true){        
    lcd.setBacklight(32);  
    menuStart = currentMillis;
    MenuDisplay();
    displayUpdate = currentMillis+1000;       
    firstState = false;
    }

  if (currentMillis >= displayUpdate+1000) {
    MenuDisplay();
    displayUpdate = currentMillis;
  }
}


//----------Timing stuff-------------------------

void SyncStateStart(){  
  //Two second Xtal clkIntTime must be < 2 seconds from currentMillis
  // sync stateStartTime to nearest XTAL ISR time'tick'
  Serial.print("Clk Int Time ");
  Serial.println (clkIntTime);
  unsigned long timeSinceInt = currentMillis - clkIntTime;
  if(timeSinceInt < 500){
    stateStartTime = clkIntTime;
  } else if (timeSinceInt < 1500){
    stateStartTime = clkIntTime + 1000;
  } else {
    stateStartTime = clkIntTime + 2000;
  }
  countReadInts = 0;
  //Serial.print("State Start Time ");
  //Serial.println (stateStartTime);
  //Serial.flush();
}

void ResyncStateStart(){
  // resonator may drift 0.5% ~ 36 seconds in 2hrs
  // adjust stateStartTime to XTAL ISR 20 seconds max resonator error +/-100mS
  // interrupt returns with difference since start 
  // stateOffsetTime = millis()-stateStartTime;
  int norm = (readIntTime - stateStartTime)/countReadInts;
  if (norm < -1 ||norm > 1){
    stateStartTime = stateStartTime - norm;
  }
}

void HrsMinsSecs(unsigned long t){
  t = t/1000; //ms
  int s = t % 60;
  t = (t- s)/60;
  int m = t % 60;
  t = (t - m)/60;
  elapsedHrs = t;
  elapsedMin = m;
  elapsedSec = s;
}

void CompletionDateTime(){

  int year = rtc.r.y +  2000;  
  uUnixDate dateA = uUnixDate(year , rtc.r.M , rtc.r.d , rtc.r.h , rtc.r.m , rtc.r.s);
  UnixTime = (dateA.timestamp());  
  ccDow = (dateA.dow());
  ccHr = rtc.r.h;
  ccMin = rtc.r.m;

  char buffer[40];
  Serial.println ("");
  sprintf(buffer,"Time: %02d:%02d:%02d ", rtc.r.h , rtc.r.m , rtc.r.s );
  Serial.println (buffer);


  // https://github.com/Naguissa/uUnixDate/blob/main/examples/uUnixDate_example/uUnixDate_example.ino
}








