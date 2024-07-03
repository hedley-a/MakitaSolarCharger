
void IdleDisplay(){
#ifdef DEBUG
  mySerial.println(F("idle display update"));
#endif
  unsigned long time = ((idleDelay - (currentMillis - idleStartTime))/1000);
  char line0[21]; 
  //char line1[21];
  //char line2[21]; 
  char line3[21];
  
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(line0, "solar battery: %-5d", myBatterySOC);
  lcd.print(line0);
  lcd.setCursor(18,0);
  lcd.print(F("%"));   
  lcd.setCursor(0,2);
  lcd.print(F("insert batteries &"));
  lcd.setCursor(0,3);
  if (time < 180){ 
    sprintf(line3,"long press strt %03ds", time);  
  }else if (currentMillis - previousMillis < 1000){
    time = time/60; 
    sprintf(line3,"long press strt: %02dm", time);
  } else {
    time = time/60;
    sprintf(line3,"long press strt  %02dm", time);
  }
  if (currentMillis - previousMillis >= 1250){
    previousMillis = currentMillis;
  }
  lcd.print(line3);
}

void ChargeDisplay(){
#ifdef DEBUG
  mySerial.println(F("charge display update"));
#endif

  unsigned long time = (currentMillis - chargeStartTime)/1000;
  char line0[21]; 
  char line1[21];
  char line2[21]; 
  char line3[21];
  
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(line0, "Charging for: %-5d", myBatterySOC);
  lcd.print(line0);
  lcd.setCursor(18,0);
  lcd.print(F("%"));   
  lcd.setCursor(0,2);
  lcd.print(F("insert batteries &"));
  lcd.setCursor(0,3);
  if (time < 180){ 
    sprintf(line3,"long press strt %03ds", time);  
  }else if (currentMillis - previousMillis < 1000){
    time = time/60; 
    sprintf(line3,"long press strt: %02dm", time);
  } else {
    time = time/60;
    sprintf(line3,"long press strt  %02dm", time);
  }
  if (currentMillis - previousMillis >= 1250){
    previousMillis = currentMillis;
  }
  lcd.print(line3);
}



void PostChargeDisplay(){

  lcd.clear();  
  lcd.setCursor(5,0);
  lcd.print("This is");
  lcd.setCursor(5,1);
  lcd.print(" Post Charge State");

}

void LcdPrint(){
  // To select a cursor location set the column, then the row. Like: lcd.setCursor(2,1)



}

//void DisplayInit(){
//  lcd.begin(20,4);
//  lcd.clear(); 
//  lcd.setBacklight(64);  
  //delay(200);
  //lcd.setBacklight(0); 
//}


void DisplayDemo(){



  lcd.clear();  
  lcd.setCursor(5,0);
  lcd.print("This is");
  lcd.setCursor(5,1);
  lcd.print(" LCD one");
  lcd.setCursor(5,2);
  lcd.print("line 3");
  lcd.setCursor(5,3);
  lcd.print("line 4");
 
 // lcd0.setCursor(0,0);
 // lcd0.print("SolBatt ");
 // lcd0.print(myBatterySOC,0);
 // lcd0.print("%");
 // lcd0.setCursor(5,1);
 // lcd0.print(" LCD Zero");
 // lcd0.setCursor(5,2);
 // lcd0.print("line 3");
 // lcd0.setCursor(5,3);
 // lcd0.print("line 4");
 // delay(500);

}




void ClearDisplay(){
  lcd.clear();

  lcd.setBacklight(0);
  
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