
void IdleDisplay(){

 int16_t solPwr = live.l.pP/100;
 

  int timeToSleep  = (stateStartTime+stateTimeout-currentMillis)/1000;
  if (timeToSleep < 0) {timeToSleep = 0;}

  char line0[21];
  char line1[21];
  char line2[21]; 
  char line3[21]; 

  snprintf_P(line0, sizeof(line0),PSTR("Solar battery  %02d%%"),solarBatterySOC);
  snprintf_P(line1, sizeof(line1),PSTR("SolarPwr: %d Watts"),solPwr);
  snprintf_P(line2, sizeof(line2),PSTR("Insert batteries"));    
  snprintf_P(line3, sizeof(line3), PSTR("and press start:  %02d"), timeToSleep);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line0);
  
  lcd.setCursor(0,1);
  //lcd.print(solPwr); 
  lcd.print(line1);
  lcd.setCursor(0,2);
  lcd.print(line2);
  lcd.setCursor(0,3);
  lcd.print(line3);

}
 

void DeferDisplay(){

//unsigned long timeToIdle  = (stateStartTime+stateTimeout-currentMillis)/1000;

//HrsMinsSecs(timeToIdle);

HrsMinsSecs(currentMillis-stateStartTime);
  char line0[21];
  char line1[21];
  char line2[21]; 
  char line3[21]; 
  snprintf_P(line0, sizeof(line0),PSTR("Charge Dly: Temp %02d"),temperatureInternal);
  snprintf_P(line1, sizeof(line3), PSTR("Delayed for %02d:%02d:%02d"),elapsedHrs,elapsedMin,elapsedSec);
  snprintf_P(line2, sizeof(line3), PSTR("Idle revert 24:00:00"));
  //snprintf_P(line3, sizeof(line0),PSTR("Chrg strt < %02d deg C"),temperatureStart);
  snprintf_P(line3, sizeof(line0),PSTR("Chrg strt < %02d degC"),temperatureStart);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line0);
  
  lcd.setCursor(0,1);
  //lcd.print(solPwr); 
  lcd.print(line1);
  lcd.setCursor(0,2);
  lcd.print(line2);
  lcd.setCursor(0,3);
  lcd.print(line3);  
}

void ChargeDisplay(){

  int8_t disP [4] = {};
  int16_t tot = loadPower/100;
  for (int i = 0;  i < 4; i++){
    disP[i]=demandPower[i]/100;
  }

  char line0[21];
  char line1[21];
  char line2[21];
  char line3[21];

  snprintf_P(line0, sizeof(line0),PSTR(" Charging: %1d:%02d:%02d"),elapsedHrs,elapsedMin,elapsedSec);
  snprintf_P(line1, sizeof(line1), PSTR(" Total: %03d   Watts"), tot);    
  snprintf_P(line2, sizeof(line2), PSTR(" Batts: %02d,%02d,%02d,%02d"),disP[1],disP[2],disP[3],disP[0]);  
  sprintf(line3, "SolBatt %02d%% %02d degC", solarBatterySOC, temperatureInternal);
  //snprintf_P(line3, sizeof(line3), PSTR(" SolBat: %02d%% approx."), solarBatterySOC);

  lcd.clear();
    lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);  
  lcd.print(line1);
  lcd.setCursor(0,2);
  lcd.print(line2);
  lcd.setCursor(0,3);
  lcd.print(line3);

}
/*
void OverDisplay(){
  
HrsMinsSecs(currentMillis-stateStartTime);
  char line0[21];
  char line1[21];
  char line2[21]; 
  char line3[21]; 
  snprintf_P(line0, sizeof(line0),PSTR("Charge Stp: Temp  %02d"),temperatureInternal);
  snprintf_P(line1, sizeof(line3), PSTR("Delayed for %02d:%02d:%02d"),elapsedHrs,elapsedMin,elapsedSec);
  snprintf_P(line2, sizeof(line3), PSTR("Idle revert 12:00:00"));
  snprintf_P(line3, sizeof(line0),PSTR("Chrg strt < %02d deg C"),temperatureStart);
 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line0);
  
  lcd.setCursor(0,1);
  //lcd.print(solPwr); 
  lcd.print(line1);
  lcd.setCursor(0,2);
  lcd.print(line2);
  lcd.setCursor(0,3);
  lcd.print(line3);  
}
*/





void PostChargeDisplay(){
  char *DOW[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

  int chargeEnergy =  (demandWattHours - startWattHours)/100;

  int timeToSleep  = (stateStartTime+stateTimeout-currentMillis)/1000;
  if (timeToSleep < 0) {timeToSleep = 0;}


  char line0[21];
  char line1[21];
  char line2[21];
  char line3[21];

  snprintf_P(line0, sizeof(line0),PSTR("CHARGED : %s %02d:%02d"),DOW[ccDow],ccHr,ccMin);
  snprintf_P(line1, sizeof(line1),PSTR("Duration:   %1d:%02d:%02d"),elapsedHrs,elapsedMin,elapsedSec);     
  snprintf_P(line2, sizeof(line2), PSTR("Energy : %03d Whrs"), chargeEnergy);  
  snprintf_P(line3, sizeof(line3), PSTR("SolBat %02d%%        %02d"), solarBatterySOC, timeToSleep);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);  
  lcd.print(line1);
  lcd.setCursor(0,2);
  lcd.print(line2);
  lcd.setCursor(0,3);
  lcd.print(line3);

}






void MenuDisplay(){

  stateStartTime = currentMillis;  // keep calling state enabled
  int timeToRevert  = (menuStart + menuTimeout - currentMillis )/1000;
  /*------------------- 
    Sol: 180  Ld: 120 W
    Batt  80% Chg 100 W
    Last 100%  12/04/24
    menu 1/3 returns xx
  --------------------*/
  int solPwr  = live.l.pP/100;
  int ldPwr = loadPower/100;
  int Chg = solPwr - ldPwr;

  char line0[21];  
  char line1[21];
  char line2[21];
  char line3[21];

  snprintf_P(line0, sizeof(line0),PSTR(": %02d:%02d %02d/%02d/%02d"),rtc.r.h,rtc.r.m,rtc.r.d,rtc.r.M,rtc.r.y);
  snprintf_P(line1, sizeof(line1),PSTR("Sol : %03d Ld: %03d W"),solPwr,ldPwr);
  snprintf_P(line2, sizeof(line2),PSTR("Batt  %02d%% Chg %03d W"),solarBatterySOC,Chg);     
  //snprintf_P(line3, sizeof(line3),PSTR("conTemp %02d  %02d"), controllerTemp, timeToRevert);
  snprintf_P(line3, sizeof(line3),PSTR("          %02d"), timeToRevert);  
 // 

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);  
  lcd.print(line1);
  lcd.setCursor(0,2);
  lcd.print(line2);
  lcd.setCursor(0,3);
  lcd.print(line3);
}

/*
}

  switch (page_counter) {
    case 1:{ //Design of home page 1
      lcd.setCursor(5,0);
      lcd.print("/*This is");
      lcd.setCursor(5,1);
      lcd.print(" Home Page");
      }
      break;

    case 2: { //Design of page 2
      lcd.setCursor(5,0);
      lcd.print("This is");
      lcd.setCursor(4,1);
      lcd.print("Page 2");
      }
      break;

    case 3: { //Design of page 3
      lcd.setCursor(1,0);
      lcd.print("You are now on");
      lcd.setCursor(4,1);
      lcd.print("Page 3");
      } 
      break;

  }   //switch end


}     //loop end
*/