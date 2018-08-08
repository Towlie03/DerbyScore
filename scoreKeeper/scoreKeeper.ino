//===============LCD SETUP
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

//=====================Time
long minute=60000;
long second = 1000;
long jamMax = minute*2;
long periodMax=minute*30;
long periodTime=0;
long jamTime=0;
long timeOutTimer=0;
long freezePeriodTime=0;
boolean periodStarted=false;
boolean jamStarted=false;
boolean timeOutStarted=false;


//==================Point Vars
int whiteScore = 0;
int blackScore=0;

//=================Button Control

int timeCorrectPin = 7;
int timeOutPin = 8;
int jamPin=6;
int teamSelectorPin = 9; 
int oneButtonPin = A1;
int twoButtonPin = A2;
int threeButtonPin = A3;
int fourButtonPin = A4;
int fiveButtonPin = A5;
int plusMinusPin=A0;

int oneButtonState=LOW;
int twoButtonState=LOW;
int threeButtonState=LOW;
int fourButtonState=LOW;
int fiveButtonState=LOW;
int jamButtonState=LOW;
int timeOutButtonState=LOW;

void setup() {
  lcd.begin(16,4);

  pinMode(teamSelectorPin,INPUT);
  pinMode(plusMinusPin,INPUT);
  pinMode(oneButtonPin,INPUT);
  pinMode(twoButtonPin,INPUT);
  pinMode(threeButtonPin,INPUT);
  pinMode(fourButtonPin,INPUT);
  pinMode(fiveButtonPin,INPUT);

  pinMode(timeCorrectPin,INPUT);
  pinMode(timeOutPin,INPUT);
  pinMode(jamPin,INPUT);
  
  Serial.begin(9600);
  Serial.print("Launched");
  
}

void loop() {
  printToScreen(whiteScore,blackScore);

  if(digitalRead(timeCorrectPin)==HIGH){
    int timeCorrection=0;
    getScore(timeCorrection);
    
    periodTime = periodTime +timeCorrection*1000;
    //freezePeriodTime =freezePeriodTime+timeCorrection*1000;
  }
  else if(digitalRead(teamSelectorPin)==HIGH){
     getScore(whiteScore);
     limitScore(whiteScore);
  }
  else{
    getScore(blackScore);
    limitScore(blackScore);
  }
  resetButtons();  
  
  if(digitalRead(timeOutPin)==HIGH){

    if(timeOutButtonState==LOW){

      if(!timeOutStarted){
        timeOutStarted=true;
        timeOutTimer=millis();
        freezePeriodTime =periodTime-millis();
      }
      else{
         timeOutStarted=false;
         periodTime=periodTime+(millis()-timeOutTimer);
      }
      
      timeOutButtonState=HIGH;
    }
  }
    
  if(digitalRead(jamPin)==HIGH)
  {
    if(jamButtonState==LOW){
      if(!periodStarted){
        periodTime=millis()+periodMax;
        periodStarted=true;
      }
      if(!jamStarted){
        jamTime=millis()+jamMax;
        jamStarted=true;
      }
      else{
        jamStarted=false;
      }
      jamButtonState=HIGH;
    }
  }

  if(jamTime<millis())
    jamStarted=false;
  if(periodTime<millis())
    periodStarted=false;
    
  Serial.println("W:"+String(whiteScore));
  Serial.println("B:"+String(blackScore));
  //To stop LCD screen flicker
  delay(125);
}

void limitScore(int &score){
  if(score<0)
    score=0;
  if(score>999)
    score=999;
}

void getScore(int &score){

int out=0;
  if(digitalRead(oneButtonPin)==HIGH){
    if(oneButtonState==LOW){
        out=1;
        oneButtonState=HIGH;
    }
  }
  if(digitalRead(twoButtonPin)==HIGH){
    if(twoButtonState==LOW){
        out=2;
        twoButtonState=HIGH;
    }
  }
  if(digitalRead(threeButtonPin)==HIGH){
    if(threeButtonState==LOW){
        out=3;
        threeButtonState=HIGH;
    }
  }
  if(digitalRead(fourButtonPin)==HIGH){
    if(fourButtonState==LOW){
        out=4;
        fourButtonState=HIGH;
    }
  }
  if(digitalRead(fiveButtonPin)==HIGH){
    if(fiveButtonState==LOW){
        out=5;
        fiveButtonState=HIGH;
    }
  }
  
  if(digitalRead(plusMinusPin)==HIGH)
    score+=out;
   else
    score-=out;
    
}

void resetButtons(){
  
  if(digitalRead(oneButtonPin)==LOW){
    oneButtonState=LOW;
  }
   if(digitalRead(twoButtonPin)==LOW){
    twoButtonState=LOW;
  }
   if(digitalRead(threeButtonPin)==LOW){
    threeButtonState=LOW;
  }
   if(digitalRead(fourButtonPin)==LOW){
    fourButtonState=LOW;
  }
   if(digitalRead(fiveButtonPin)==LOW){
    fiveButtonState=LOW;
  }
  if(digitalRead(jamPin)==LOW){
      jamButtonState=LOW;
    }
  if(digitalRead(timeOutPin)==LOW){
      timeOutButtonState=LOW;
    }
}

void printToScreen(int whitePnt, int blackPnt){

  
  String whitePointString ="WHITE:";
  whitePointString+=padScore(whitePnt);

  String blackPointString="BLACK:";
  blackPointString+=padScore(blackPnt);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(whitePointString);
  lcd.setCursor(0,1);
  lcd.print(blackPointString);
  
  lcd.setCursor(0,2);
  String periodText ="PER-";
  if(timeOutStarted){
    lcd.print(periodText+freezeTime(freezePeriodTime));
    Serial.println("P:"+freezeTime(freezePeriodTime));
  }
  else{
    
    if(periodStarted){
      lcd.print(periodText+gameTime(periodTime));
      Serial.println("P:"+gameTime(periodTime));
    }
    else{
      lcd.print(periodText+"30:00");
      Serial.println("P:30:00");
    }
  }
  
  lcd.setCursor(0,3);
  String jamText="   JAM-";
  if(jamStarted){
    lcd.print(jamText+gameTime(jamTime));
    Serial.println("J:"+gameTime(jamTime));
  }
   else{
     lcd.print(jamText+"02:00");
     Serial.println("J:02:00");
   }
}

String gameTime(long timeTracker){
 long val = timeTracker-millis();
 int minutes = val/minute;
  int seconds = (val%minute)/second;
  String output = padTime(minutes)+":"+padTime(seconds);
  return output;
}

String freezeTime(long timeTracker){
 long val = timeTracker;
 int minutes = val/minute;
  int seconds = (val%minute)/second;
  String output = padTime(minutes)+":"+padTime(seconds);
  return output;
}



String padScore(int score){
  String tempString = String(score);
  if(tempString.length() ==1)
      return String("00")+=tempString;
  if(tempString.length() ==2)
      return String("0")+=tempString;
return tempString;
}

String padTime(int score){
  String tempString = String(score);
  if(tempString.length() ==1)
      return String("0")+=tempString;
return tempString;
}


