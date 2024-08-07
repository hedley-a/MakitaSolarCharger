//---------------------------------------------------------------------------------------------------

// state change timeouts 

/*
void ElapsedTime(){
  int t = (currentMillis - stateStartTime)/1000;
  int s = t % 60;
  t = (t - s)/60;
  int m = t % 60;
  t = (t - m)/60;
  elapsedHrs = t;
  elapsedMin = m;
  elapsedSec = s;
}
*/
//Idle and post charge sleep timeout is reset by PIR input

void SleepSetup(){
  digitalWrite(chrgLED, HIGH);            // charging_LED
  digitalWrite(LEDchrgd, HIGH);           // complete_LED
  digitalWrite(balON , LOW);              // 17 A3 battery active balancer_EN
  digitalWrite(Enble24V , LOW);           // 18             
  lm75a.end();
  Wire.end();
  digitalWrite(Enble05V , LOW);          // PROD  // Stuff that sucks a few mA (8)
  digitalWrite(EnblRS485 , HIGH);         // Charge Start IOC
#ifndef DEBUG
  Serial.end();
#endif
}

void IdleSetup(){
  digitalWrite(chrgLED, HIGH); 
  digitalWrite(LEDchrgd, HIGH);          // complete_LEDvoid getReadableTime(String &readableTime) {
  //digitalWrite(EnblRS485 , LOW);         // 
  //digitalWrite(balON , HIGH);               // 17 A3 battery active balancer_EN
  digitalWrite(Enble24V , LOW);             // 18 
  digitalWrite(Enble05V , HIGH);           // DEV

}

//void DeferSetup(){ 
  //no change from idle 
//}

//void OverSetup(){ 
//  digitalWrite(Enble24V , LOW);             // 18 
//}

void ChargeSetup(){
  digitalWrite(chrgLED, LOW);            // charging_LED
  digitalWrite(Enble24V, HIGH);             // 18
  //digitalWrite(balON , HIGH);
}

void PostChargeSetup(){
  digitalWrite(chrgLED, HIGH);            // charging_LED
  digitalWrite(LEDchrgd, LOW);            // complete_LED
  digitalWrite(Enble24V, LOW);             // 18
}


void pre_charging(){

  digitalWrite(chrgLED, LOW);          // LED OFF
  digitalWrite(Enble05V, LOW);            // +5VSW O
  //powerON = true;
  delay(50);                          // allow PSU and I2C to stabilise
  Wire.begin();  
  lm75a.begin();
  ads1115.begin(0x49);                // Initialize ads1115 at address 0x49
  ads1115.setGain(GAIN_ONE);          // 1x gain   +/- 4.096V  1 bit = 2mV
}

void post_charging(){
  //Wire.end();
  digitalWrite(chrgLED, HIGH);          // LED OFF
  digitalWrite(LEDchrgd, HIGH);        // LED OFF
  digitalWrite(Enble24V, LOW);         // +24V SW OFF
  digitalWrite(Enble05V, LOW);            // +5VSW O
}

void getReadableTime(int elapsedMillis ) {
  unsigned long seconds;
  unsigned long minutes;
  unsigned long hours;

  seconds = elapsedMillis / 1000;
  minutes = seconds / 60;
  hours = minutes / 60;
}







