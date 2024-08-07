



















void preTransmission()
{
//  digitalWrite(MAX485_RE, 1);
//  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
//  digitalWrite(MAX485_RE, 0);
//  digitalWrite(MAX485_DE, 0);
}

//------------------------------------------------------------
void niceDelay(unsigned long delayTime)
{  
  unsigned long startTime = millis();
  while (millis() - startTime < delayTime)
  {
    yield();
  }
}

void read_batterySOC() {
  niceDelay(50);
  node.clearResponseBuffer();
  result = node.readInputRegisters(BATTERY_SOC, 1);
  if (result == node.ku8MBSuccess)  {
    
    batterySOC = node.getResponseBuffer(0);

  } else {
#ifdef DEBUG
    Serial.print(F("Miss read batterySOC, ret val:"));
    Serial.println(result, HEX);
#endif
  }
}

void ReadEpeverClock() {
  //
  memset(rtc.buf,0,sizeof(rtc.buf));
  memset(live.buf,0,sizeof(live.buf));
  //memset(stats.buf,0,sizeof(stats.buf));

  // Read registers for clock
  //
  niceDelay(50);
  node.clearResponseBuffer();
  uint8_t result = node.readHoldingRegisters(RTC_CLOCK, RTC_CLOCK_CNT);
  if (result == node.ku8MBSuccess)  {

    rtc.buf[0]  = node.getResponseBuffer(0);
    rtc.buf[1]  = node.getResponseBuffer(1);
    rtc.buf[2]  = node.getResponseBuffer(2);
    bool rtc_good = true;
    
  } else {
  bool rtc_good = false;
#ifdef DEBUG
    Serial.print(F("Miss read rtc-data, ret val:"));
    Serial.println(result, HEX);
#endif
  } 
  if (result==226)     ErrorCounter++;
}

void ReadEpeverData() {
  // read LIVE-Data
  niceDelay(50);
  node.clearResponseBuffer();
  result = node.readInputRegisters(LIVE_DATA, LIVE_DATA_CNT);

  if (result == node.ku8MBSuccess)  {

     for(uint8_t i=0; i< LIVE_DATA_CNT ;i++) live.buf[i] = node.getResponseBuffer(i);
       
  } else {
#ifdef DEBUG
    Serial.print(F("Miss read live-data, ret val:"));
    Serial.println(result, HEX);
#endif    
  }
}

void ReadEpeverTemperatures() {
  //read_temperatures
  niceDelay(50);
  node.clearResponseBuffer();

  result = node.readInputRegisters(BATT_TEMPERATURE, 1);
  if (result == node.ku8MBSuccess)  {
    
    BattTemperature = node.getResponseBuffer(0);

  } else {
#ifdef DEBUG
    Serial.print(F("Miss read battery temperature, ret val:"));
    Serial.println(result, HEX);
#endif
  }

  niceDelay(50);
  node.clearResponseBuffer();
  result = node.readInputRegisters(CASE_TEMPERATURE, 1);
  if (result == node.ku8MBSuccess)  {
    
    CaseTemperature = node.getResponseBuffer(0);

  } else {
#ifdef DEBUG
    Serial.print(F("Miss read case temperature, ret val:"));
    Serial.println(result, HEX);
#endif
  }
}

void EpeverError(){
//#ifdef DEBUG
   // Serial.print("Error count");
   // Serial.println(ErrorCounter);
//#endif

  if (ErrorCounter>5) {
    // init modbus in receive mode
  //  pinMode(MAX485_RE, OUTPUT);
  //  pinMode(MAX485_DE, OUTPUT);
  //  postTransmission();

    // EPEver Device ID and Baud Rate
    node.begin(1, Serial);
    
    // modbus callbacks
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
    ErrorCounter = 0;  
  }

}

