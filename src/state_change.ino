
// setting up outputs and variables for various states 

void SleepSetup(){
  digitalWrite(chrgLED, HIGH);            // charging_LED
  digitalWrite(LEDchrgd, HIGH);           // complete_LED
  digitalWrite(balON , LOW);              // 17 A3 battery active balancer_EN
  digitalWrite(Enble24V , LOW);           // 18
  digitalWrite(Enble05V , HIGH);          // These two may change with hardware
  // Stuff that sucks a few mA (8)
  digitalWrite(EnblRS485 , HIGH);         // Charge Start IOC
  Wire.end();
  mySerial.end();
  Serial.end();
}

void IdleSetup(){
  digitalWrite(chrgLED, HIGH);            // charging_LED
  digitalWrite(LEDchrgd, HIGH);            // complete_LEDvoid getReadableTime(String &readableTime) {
  digitalWrite(EnblRS485 , HIGH);         // Charge Start IOC 
  digitalWrite(balON , LOW);               // 17 A3 battery active balancer_EN
  digitalWrite(Enble24V , LOW);             // 18
  //digitalWrite(En24Imon , LOW);            // 18
  digitalWrite(Enble05V , LOW);           // These two may change with hardware
  delay(50);                          // allow PSU and I2C to stabilise
  Wire.begin();  
  lm75a.begin();
  HutTmp = (lm75a.getTemperature());  // update temperaature
  ads1115.begin(0x49);                // Initialize ads1115 at address 0x49
  ads1115.setGain(GAIN_ONE);          // 1x gain   +/- 4.096V  1 bit = 2mV
  lcd.begin(20,4);
  lcd.clear(); 
  lcd.setBacklight(64);
}

void ChargeSetup(){
  digitalWrite(chrgLED, LOW);            // charging_LED
  digitalWrite(Enble24V, HIGH);             // 18
}

void PostChargeSetup(){
  digitalWrite(chrgLED, HIGH);            // charging_LED
  digitalWrite(LEDchrgd, LOW);            // complete_LED
  digitalWrite(Enble24V, LOW);             // 18
}



// Nick Gammons switch handler -------------------------------------------------------------

void handleStartPress (const byte newState, const unsigned long interval, const byte whichPin)
  {
    // switch down?
    if (newState == LOW) {
    return;     
    }  // end of switch down
    // switch must be HIGH
    if (interval >= 1000){
      firstCharge = true;  
      chargerState = ChargerStates::Charging;
    }
  }

void handleStopPress (const byte newState, const unsigned long interval, const byte whichPin)
  {
    // switch down?
    if (newState == LOW) {
    return;     
    }  // end of switch down
    // switch must be HIGH
    if (interval >= 500){
      firstPostCharge = true; 
      chargerState = ChargerStates::PostCharge;
    }
  }

void handleMenuPress (const byte newState, const unsigned long interval, const byte whichPin)
  {
    // switch down?
    if (newState == LOW) {
    return;     
    }  // end of switch down
    // switch must be HIGH
    if (interval >= 500){
      menu = true;
    }
  }
//---------------------------------------------------------------------------------------------------

void RS485_setup(){
  // Stuff that sucks a few mA (8)
  digitalWrite(EnblRS485 , LOW);         // Start interface
#ifdef DEBUG
  mySerial.begin(19600);
#endif
  Serial.begin(115200);
  while (!Serial){;}
}

void pre_charging(){

  digitalWrite(chrgLED, LOW);          // LED OFF
  digitalWrite(Enble05V, LOW);            // +5VSW O
  powerON = true;
  delay(50);                          // allow PSU and I2C to stabilise
  Wire.begin();  
  lm75a.begin();
  HutTmp = (lm75a.getTemperature());  // update temperaature
  ads1115.begin(0x49);                // Initialize ads1115 at address 0x49
  ads1115.setGain(GAIN_ONE);          // 1x gain   +/- 4.096V  1 bit = 2mV
}

void start_charging(){
  for (int i = 0;  i < 4; i++){
    Makita20s[i] = 0;
    Makita_Watt_Hrs[i] = 0;
    MakitaTotalWh = 0;
  }
  digitalWrite(Enble24V, HIGH);         // +24V SW ON
  DiscWh_count = 1;
  chargerON = true;
}

void post_charging(){
  //Wire.end();
  digitalWrite(chrgLED, HIGH);          // LED OFF
  digitalWrite(LEDchrgd, HIGH);        // LED OFF
  digitalWrite(Enble24V, LOW);         // +24V SW OFF
  digitalWrite(Enble05V, LOW);            // +5VSW O
}


void read_buttons(){

}


void getReadableTime(int elapsedMillis ) {
  unsigned long seconds;
  unsigned long minutes;
  unsigned long hours;

  seconds = elapsedMillis / 1000;
  minutes = seconds / 60;
  hours = minutes / 60;

}





