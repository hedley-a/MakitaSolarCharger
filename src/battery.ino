/* Coloumb counting*/
// https://www.victronenergy.com/media/pg/Lynx_Shunt_VE.Can/en/battery-monitor-settings.html
//Save SOC and UNIX time to EEPROM 
// could do this every 4 hours for one day 
// then at midnight save the total
//Also do days as history table
//if power cycled during day then read the historical record


//ON boot up read time from Epever: then check last record time

//IF recent <2 days then use it 

//Otherwise make a good guess from battery idle volts

void MapBatteryVolts(){
  //https://www.arduino.cc/reference/en/libraries/multimap/
  //if(DischrgIdle == true && BattTemperature > 8){
    //read eeprom
    // compare now with write date
    // if write date + now > 5 days? or first boot.. 
  myBatterySOC = multiMap<uint8_t>((live.l.bV/10), in, out, sz);
  //}  
}

void ReadingSolar(){
  RS485_setup(); // turns on serial ports   
  delay(20);
  mySerial.println("reading Solar");
  read_values();  
  UpdateDateTime();
  MapBatteryVolts();
      //ColoumbCount();
  debug_print();
  readSolar = false;
}

// Call this every 20 Seconds....
void ColoumbCount(){

/**** Globals used in routine ****
  BattIdleTime - length of time the battery has been sitting without charging or discharging
  live.l.bI current (x 100)
  live.l.bV volts (x 100)
  live.l.bP Watts
  live.l.pP 
  Charge_Wh
  Discharge_Wh
  Charge10m
  Discharge10m
  Charge20s
  Discharge20s
*/
  // Set 100% charge if 14.2Volts and idle current
  currentMillis = millis();
  if(chargerON == false && live.l.bV > Batt100pcV && live.l.bI < Batt100pcI && live.l.pV > SunsOut){
    if (currentMillis + BattIdleTime > 10000){   // 10 minuites of above condition
      myBatterySOC = 100; //
      Charge_Wh = 0;       // 300Ah@ 14Volts = 4200Wh
      Discharge_Wh = 0;    // https://www.victronenergy.com/media/pg/Lynx_Shunt_VE.Can/en/battery-monitor-settings.html
      }
  } else { BattIdleTime = currentMillis; } // reset 10 min timer

/**** charge energy accumulator ****
Use integer maths and sucessive accumulation,
Sample currents every 20S over 10 mins then divide by 18 (scaling by 10 increases resolution)
Add this result to the 10 minute charge/discharge accumulator & reset 20 second count
Max of 300 Watts discharge 200W charging  300 X 30 = 9000 use a 16 bit integer
Max battery capacity 300Ah is 4200 Wh(x10) 16 bit integer

*/

// First check load
  LoadPower = 0;
  if (chargerON == true){
    
    ads1115.begin(0x49);                // Initialize ads1115 at address 0x49
    ads1115.setGain(GAIN_ONE);          // 1x gain   +/- 4.096V  1 bit = 2mV

    //DiscWh_count = DiscWh_count + 1;
    //ADC vars  
    int16_t adc_val [4] = {0,0,0,0};      // array for reading adc vals
    const int16_t adc_offset [4] = {20003,20018,19991,19947};      
    const int16_t adc_scale [4] = {107,107,110,147};
    int16_t total = 0;      // used for total current and supply voltage
    MakitaTotalWh = 0;

    // Vcc = 4.99 Volts  
    //    AIN0: 20003, AIN1: 20018, AIN2: 19991, AIN3: 19947
    //    2.0 Amps
    //    AIN0: 22150, AIN1: 22159, AIN2: 22193, AIN3: 22885
    //    2.5 Amps
    //    AIN0: 22685, AIN1: 22691, AIN2: 22745, AIN3: 23622
    //    AIN0: 1073, AIN1: 1069, AIN2: 1102, AIN3: 1469

    // Read current monitors 
    for (int i = 0;  i < 4; i++){
      adc_val[i] = ads1115.readADC_SingleEnded(i);
      Makita_Power[i] = ((adc_val[i]-adc_offset[i])*10.0/adc_scale[i]);  // Current *100
      total = total + Makita_Power[i];  // full current val
    }
    
    // supply voltage scaled by total load current
    if (total > 50){        // 0.5 Amps cutoff
      total = (Idle*100)-((Idle-Load)*total/10); // 230 at full load, 240 at no load
    } else {
      total = (Idle*100);
    }

    for (int i = 0;  i < 4; i++){
      Makita_Power[i] = (Makita_Power[i]*total)/1000;  // This results in Watts * 100
      LoadPower =  LoadPower + Makita_Power[i];
      // now calculate Watt/hours. routine called every 20 seconds using wall clock IOC.
      Makita20s[i] = Makita20s[i] + Makita_Power[i]; 
      //after 1 hour divide by 180 and reset..Makita20s
      if(Count180 >= 180){
      Makita_Watt_Hrs[i] = Makita_Watt_Hrs[i] + Makita20s[i]/180; // 180 * 20 seconds in one hour...
      Makita20s[i] = 0;
      // Count180 reset globally
      }        
      MakitaTotalWh = MakitaTotalWh + Makita20s[i]/100;
    }    
  }
    
  if (count20s < 30){ // count 0-29
        //if(live.l.pP > 50){ // 5 watts of solar
        //    ChargePwrFactored = (live.l.pP) - (live.l.pP)/200; 95% battery charge efficency 
        //}
        //Charge20s = Charge20s + live.l.pP/100 - live.l.pP/100;  // use epever load monitor

    Charge20s = Charge20s + live.l.pP/100 - LoadPower;  // use Makita load monitors



  

    Solar_Watt_Hrs = Solar_WattSec/100;         // at 75 watts will take three counts = 1 minuite to one
    // need to reset the seconds count every hour to avoid overflow.. 
    SolarTotalWh = SolarTotalWh + Solar_WattSec/100;
  }
  

}

// Continue with hourly? calculations of Solar in and load out
// factor in charge efficency 95% and burden current.



// IF (Resting V > 14.2, Battery I < 0.2 , Solar V > 17) TRUE for 10 mins  = 100% charge

// (MAYBE) IF Resting V < 10.1 for 10 mins = 0% charge (Epever will have switched offbefore this)
//use millis() and set variable SampleTime (every 20 seconds)
//ELSE count = count++
//IF count > 30 (10 mins) charge = 100%
//reset all couloumb  counting



// Called every 20 Seconds by IOC - Couloumb (Watt Hour monitoring  
//if TRUE  Resting V > 14.2, Solar I < 0.2 Solar V > 17 for 10 mins  = 100%  charge 

//use millis() and set variable SampleTime (every 20 seconds)

//Next sample sanity check 

//IF  current ms > sample time + 21,000 resetcount 

//ELSE count = count++

void UpdateDateTime(){
  // https://github.com/Naguissa/uUnixDate/blob/main/examples/uUnixDate_example/uUnixDate_example.ino
  uUnixDate dateA = uUnixDate(rtc.r.y, rtc.r.M, rtc.r.d, rtc.r.h, rtc.r.m, rtc.r.s);
  //mySerial.print("DateTime: ");
  //mySerial.println(dateA.timestamp());
  UnixTime = (dateA.timestamp());
  //mySerial.print("UnixTime: ");2
  //mySerial.println(UnixTime);
}


