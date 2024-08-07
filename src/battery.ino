/* Coloumb counting -----------------------------------------------------------------------
https://www.victronenergy.com/media/pg/Lynx_Shunt_VE.Can/en/battery-monitor-settings.html

Save SOC and UNIX time to EEPROM 
If power cycled then read the EEPROM historical record
https://docs.arduino.cc/learn/built-in-libraries/eeprom/

Continue with hourly? calculations of Solar in and load out
factor in charge efficency 95%? and burden current.

IF (Resting V > 14.2, Battery I < 0.2 , Solar V > 17) TRUE for 10 mins  = 100% charge

(MAYBE) IF Resting V < 10.2 for 10 mins = 0% charge? (pack UVP 10.0 Volts)

Future 'Stuff' - maybe...
Do this every 4 hours for one day 
At midnight save the total
Do days as history table

ON boot up read time from Epever: then check last record time
IF recent <2 days then use it 
Otherwise make best guess from battery idle volts using lookup
//https://www.arduino.cc/reference/en/libraries/multimap/

------------------------------------------------------------------------------------------
*/
void MapBatteryVolts(){   
  if(live.l.bI < Batt100pcI){
    battIdle ++;
    if (battIdle > 10){   // 200S mins of above condition //   
      solarBatterySOC = multiMap<uint8_t>((live.l.bV/10), in, out, sz);
    }
  } else { battIdle = 0; } // reset  timer
}

void ControllerTemperature(){
  temperatureInternal = (lm75a.getTemperature());  // update temperature
  if (temperatureInternal <-20){
    temperatureInternal = 99;
  }
  Serial.println("temp: ");
  Serial.println(temperatureInternal);
}

void CheckFullCharge(){ //----Test for 100% charge
  if(live.l.bV > Batt100pcV && live.l.bI < Batt100pcI && live.l.pV > 1700){
    solarBattIdleTime ++;
    if (solarBattIdleTime > 30){   // 10 mins of above condition
      solarBatterySOC = 100; //
      energyMeter = 0;
     // Serial.println("100% charged");
    } 
  } else { solarBattIdleTime = 0; } // reset 10 min timer
}

void DischargePowers(){
  // Current monitors on each charger > ADC > inverter volts:
  //Serial.println("Discharge_powers");
    
  int32_t loadCurrent = 0;     // used for total current and supply voltage
  loadPower = 0;

    // Vcc = 4.99 Volts  
    //IDLE    AIN0: 16559, AIN1: 16533, AIN2: 16549, AIN3: 16719
    //2.5A    AIN0: 18645, AIN1: 18658, AIN2: 18734, AIN3: 19633
    //2.5A MINUS IDLE MULTIPLY BY 120,118,115,86 DIVIDE BY 1000 MULTIPLY BY 204 VOLTS
#ifdef DEBUG
   // Serial.println("adcValueS: ");
#endif

  for (int i = 0;  i < 4; i++){
    adcValue = ads1115.readADC_SingleEnded(i); 
   // delay(20);
#ifdef DEBUG
   // Serial.print(adcValue);
  //  Serial.print(", ");
#endif
    if (adcValue < adc_idle[i]){
      demandPower[i] = 0;
    } else {
    demandPower[i] = (adcValue - adc_idle[i]);    
    demandPower[i] = demandPower[i]*adc_scale; //
    demandPower[i] = demandPower[i]/1000; //100 X current
    }
    loadCurrent = loadCurrent + demandPower[i];
  }
#ifdef DEBUG
    Serial.println("");
#endif
  int16_t LoadedVolts = (6000 - loadCurrent)/25; // ~200 divide by 25 drop of 4 volts at 10 amps
  //Serial.print("demandPowerx100: ");
  for (int i = 0;  i < 4; i++){  
    demandPower[i] = demandPower[i]*LoadedVolts; //1000X actual
    demandPower[i] = demandPower[i]/10;
    loadPower = loadPower + demandPower[i];

  }

}
/*----------------------------------------------------------------------------------------------
/**** charge energy accumulator ****
Sample currents every 20S over 5 mins then divide by 18 (scaling by 10 increases resolution)
Add this result to the 10 minute charge/discharge accumulator & reset 20 second count
Max of 300 Watts discharge 200W charging  300 X 30 = 9000 use a 16 bit integer
Max battery capacity 300Ah is 4200 Wh(x10) 16 bit integer
-----------------------------------------------------------------------------------------------
*/
void EnergyBalance(){ // Called every 20 Seconds....
  // called after updates to times and power readings
  

  demandSecs = demandSecs + (loadPower);// positve for charge, values x100
  chargeSecs = chargeSecs + (live.l.bP);//x100
  count20min ++ ;


  if(count20min > 60){      
    if (demandSecs/180 < 10 ) {chargerIdle = true;} else {chargerIdle = false;}
    demandWattHours = demandWattHours + (demandSecs/180); //100 x Wh
    chargeWattHours = chargeWattHours + (chargeSecs/180);
    count20min = demandSecs = chargeSecs = 0;
    energyMeter = energyMeter - demandWattHours + chargeWattHours;

#ifdef DEBUG
    Serial.print(F("EnergyMeter "));
    Serial.println(energyMeter);
#endif 
  }

}

/*
  // Set 100% charge if 14.2Volts and idle current
  // https://www.victronenergy.com/media/pg/Lynx_Shunt_VE.Can/en/battery-monitor-settings.html
*/







