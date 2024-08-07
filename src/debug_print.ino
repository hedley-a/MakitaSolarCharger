/*
//DEBUG PRINTING //

void debugPrint(){
  char buffer[40];
  sprintf(buffer,"Time: %02d:%02d:%02d ", rtc.r.h , rtc.r.m , rtc.r.s );
  Serial.print (buffer);

  snprintf_P(buffer, sizeof(buffer),PSTR("DateTime: 20%02d-%02d-%02d   %02d:%02d:%02d" ),rtc.r.y,rtc.r.M,rtc.r.d,rtc.r.h,rtc.r.m,rtc.r.s);
  //sprintf(buffer,"Date: %02d:%02d:%02d ", rtc.r.h , rtc.r.m , rtc.r.s)  
  Serial.print (buffer);
  Serial.print(" DOW ");
  Serial.println(ccDow);
  Serial.flush();

  //Serial.println(dateA.dow());

  //Serial.println(dateA.timestamp());
  
  //Serial.println("weekday  ");
  //Serial.print(dateA.day());
  //Serial.println("  ");
  //Serial.print(dateA.dow());


}

#ifdef DEBUG
  

  mySerial.print(  F("PV_V: "));
  mySerial.print((float)live.l.pV/100.0);
  //mySerial.print(  F("  PV I:  "));
  //mySerial.print((float)live.l.pI/100.0);
  mySerial.print(  F("  PV_W: "));
  mySerial.print((float)live.l.pP/100.0);
  mySerial.print(  F("   Batt_V: "));
  mySerial.print((float)live.l.bV/100.0);
  //mySerial.print(  F("  Batt_I:  "));
  //mySerial.print((float)live.l.bI/100.0);
  mySerial.print(  F("  Batt_W: "));
  mySerial.println((float)live.l.bP/100.0);
  mySerial.println("");

  mySerial.println(  F("\n Temps:"));
  mySerial.print(  F("Battery:"));
  mySerial.print((float)BattTemperature/100.0);
  mySerial.print(  F("  Device :"));
  mySerial.println((float)CaseTemperature/100.0);
   mySerial.println("");
  //mySerial.print(  F(" Heatsink:"));
  //mySerial.println((float)SinkTemperature/100.0)
  mySerial.print(  F(" dummyBatV: "));
  mySerial.println(dummyBat);  
  mySerial.print(  F(" myBatterySOC:"));
  mySerial.println(myBatterySOC);

  //mySerial.println("");
  //mySerial.print(  F("  Battery Current: "));
  //mySerial.println((float)batteryCurrent/100.0);
  //mySerial.println("");
  //mySerial.print(  F("\n Status:"));
  //sprintf(buffer,"\n batt.volt: %s batt.temp: %s ",     batt_volt_status[status_batt.volt], batt_temp_status[status_batt.temp]);
  //mySerial.println (buffer);

  //sprintf(buffer, "\n    charger.charging:  %s   \n\n",     charger_charging_status[ charger_mode] );
  //mySerial.println (buffer);

  mySerial.println("");
  mySerial.print( F("Pwr1: "));   mySerial.print(Makita_Power[0]/100);
  mySerial.print( F("  Pwr2: ")); mySerial.print(Makita_Power[1]/100);
  mySerial.print( F("  Pwr3: ")); mySerial.print(Makita_Power[2]/100);
  mySerial.print( F("  Pwr4: ")); mySerial.println(Makita_Power[3]/100);
  
  mySerial.print( F("Wh 1: "));   mySerial.print(Makita_Watt_Hrs[0]);
  mySerial.print( F("  Wh 2: ")); mySerial.print(Makita_Watt_Hrs[1]);
  mySerial.print( F("  Wh 3: ")); mySerial.print(Makita_Watt_Hrs[2]);
  mySerial.print( F("  Wh 4: ")); mySerial.println(Makita_Watt_Hrs[3]);
  mySerial.println( F("Wh C: "));  mySerial.println(DiscWh_count);
   mySerial.print( F("TotalWh: "));  mySerial.println(MakitaTotalWh);
  sprintf(buffer,"\n chargerON: %d  powerON: %d",chargerON, powerON ); // prints true )
   mySerial.println (buffer);
  mySerial.println("");


  mySerial.print("LM75 = ");
  mySerial.print(HutTmp);
  mySerial.println(" C");
  mySerial.flush();

#endif
*/



