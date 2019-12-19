#include <LiquidCrystal.h>  // Лобавляем необходимую библиотеку
#include <iarduino_RTC.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // (RS, E, DB4, DB5, DB6, DB7)
iarduino_RTC timeModule(RTC_DS1307);
const int buttonOk = 8;
const int buttonNext = 9;
const int buttonBack = 10;
const int micro = 11;

const String month1 = "january";
const String month2 = "february";
const String month3 = "march";
const String month4 = "april";
const String month5 = "may";
const String month6 = "june";
const String month7 = "july";
const String month8 = "august";
const String month9 = "september";
const String month10 = "october";
const String month11 = "november";
const String month12 = "december";

const String week0 = "sunday";
const String week1 = "monday";
const String week2 = "tuesday";
const String week3 = "wednesday";
const String week4 = "thursday";
const String week5 = "friday";
const String week6 = "saturday";

bool firstStart = false;
int posOfCursor = 0;
int instSec = 0;
int instMin = 0;
int instHour = 0;
int instDate = 0;
int instMonth = 0;
int instYear = 0;
int instDay = 0;
int instWeek = 0;

int timer1Sec = 0;
int timer1Min = 0;
int timer1Hour = 0;
int timer1SaveSec = 0;
int timer1SaveMin = 0;
int timer1SaveHour = 0;
int timer2Sec = 0;
int timer2Min = 0;
int timer2Hour = 0;
int alarm1Sec = 0;
int alarm1Min = 0;
int alarm1Hour = 0;
int alarm2Sec = 0;
int alarm2Min = 0;
int alarm2Hour = 0;
long timer1SaveMillis = 0;
long timer2SaveMillis = 0;
bool timer1Enabled = false;
bool timer2Enabled = false;
bool alarm1Enabled = false;
bool alarm2Enabled = false;

int menu = 0;
/*
 * 0 - время
 * 1 - таймер1
 * 2 - таймер2
 * 3 - будильник1
 * 4 - будильник2
 * 5 - настройка времени
 * */

void setup(){ 
  pinMode(13 , OUTPUT);
  pinMode(buttonOk , INPUT);
  pinMode(buttonNext , INPUT);
  pinMode(buttonBack , INPUT);
  pinMode(micro , OUTPUT);

  Serial.begin(9600);
  
  lcd.begin(16, 2);                  
  timeModule.begin();


  
  lcd.setCursor(0, 0);              // Устанавливаем курсор в начало 1 строки
  lcd.print("clock");       // Выводим текст
  lcd.setCursor(0, 1);              // Устанавливаем курсор в начало 2 строки
  lcd.print("v. 3.015");         // Выводим текст
}

int checkButton() { /*если ok - 1
                                 next - 2
                                 back - 3
                               ничего - 0
                                           */
  if(digitalRead(buttonOk) == HIGH) {
    return 1;
  } 
  if(digitalRead(buttonNext) == HIGH) {
    return 2;
  } 
  if(digitalRead(buttonBack) == HIGH) {
    return 3;
  } 
  return 0;
}

int getNum() {
  int num = 0;
  lcd.setCursor(posOfCursor , 0);
  lcd.print(num);
  int animTime = 4;
  while(checkButton() != 1) {
    if(checkButton() == 2) num++;
    if(checkButton() == 3) num--;
    if(num == -1) num = 9;
    if(num == 10) num = 0;
    lcd.setCursor(posOfCursor , 0);
    lcd.print(num);
    delay(150);
    if(animTime == 2) {
      lcd.setCursor(posOfCursor , 1);
      lcd.print(" ");
    }
    if(animTime == 4) {
      animTime = 0;
      lcd.setCursor(posOfCursor , 1);
      lcd.print("^");
    }
    animTime++;
  }
  return num;
}

void showTime() {
  lcd.clear();
  lcd.setCursor(0 , 0);
  
  if(instHour < 10) lcd.print("0");
  lcd.print(instHour);
  
  lcd.print(" : ");
  
  if(instMin < 10)  lcd.print("0");
  lcd.print(instMin);
  
  lcd.print(" : ");
  
  if(instSec < 10)  lcd.print("0");
  lcd.print(instSec);  
}

void showTimer(int i) {
  lcd.clear();
  lcd.setCursor(0 , 0);
  if(i == 1){
    
  if(timer1Hour < 10) lcd.print("0");
  lcd.print(timer1Hour);
  
  lcd.print(" : ");
  
  if(timer1Min < 10)  lcd.print("0");
  lcd.print(timer1Min);
  
  lcd.print(" : ");
  
  if(timer1Sec < 10)  lcd.print("0");
  lcd.print(timer1Sec); 
   
  } else {
    
  if(timer2Hour < 10) lcd.print("0");
  lcd.print(timer2Hour);
  
  lcd.print(" : ");
  
  if(timer2Min < 10)  lcd.print("0");
  lcd.print(timer2Min);
  
  lcd.print(" : ");
  
  if(timer2Sec < 10)  lcd.print("0");
  lcd.print(timer2Sec);  
    
  }
}

void showAlarm(int i) {
  lcd.clear();
  lcd.setCursor(0 , 0);
  if(i == 1){
    
  if(alarm1Hour < 10) lcd.print("0");
  lcd.print(alarm1Hour);
  
  lcd.print(" : ");
  
  if(alarm1Min < 10)  lcd.print("0");
  lcd.print(alarm1Min);
  
  lcd.print(" : ");
  
  if(alarm1Sec < 10)  lcd.print("0");
  lcd.print(alarm1Sec); 
   
  } else {
    
  if(alarm2Hour < 10) lcd.print("0");
  lcd.print(alarm2Hour);
  
  lcd.print(" : ");
  
  if(alarm2Min < 10)  lcd.print("0");
  lcd.print(alarm2Min);
  
  lcd.print(" : ");
  
  if(alarm2Sec < 10)  lcd.print("0");
  lcd.print(alarm2Sec);  
    
  }
}

void showDay() {
  lcd.clear();
  lcd.setCursor(0 , 0); 
  lcd.print("Day: 00") ;
  lcd.setCursor(5 , 0);
  
  if(instDay < 10) lcd.print("0");
  lcd.print(instDay);
}

void showYear() {
  lcd.clear();
  lcd.setCursor(0 , 0); 
  lcd.print("Year: 0000") ;
  lcd.setCursor(6 , 0);
  
  if(instYear < 1000) lcd.print("0");
  if(instYear < 100) lcd.print("0");
  if(instYear < 10) lcd.print("0");
  lcd.print(instYear);
}

void showDateAndMonth() {
  lcd.setCursor(0 , 1);
  if(instWeek == 0) lcd.print(week0);
  if(instWeek == 1) lcd.print(week1);
  if(instWeek == 2) lcd.print(week2);
  if(instWeek == 3) lcd.print(week3);
  if(instWeek == 4) lcd.print(week4);
  if(instWeek == 5) lcd.print(week5);
  if(instWeek == 6) lcd.print(week6);
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  timeModule.gettime();
  instSec = timeModule.seconds;
  instMin = timeModule.minutes;
  instHour = timeModule.hours;
  showTime();
  lcd.setCursor(13 , 0);
  if(timeModule.midday == 0) lcd.print("am");
  if(timeModule.midday == 1) lcd.print("pm");
  lcd.setCursor(0 , 1);
  if(instWeek == 0) lcd.print(week0);
  if(instWeek == 1) lcd.print(week1);
  if(instWeek == 2) lcd.print(week2);
  if(instWeek == 3) lcd.print(week3);
  if(instWeek == 4) lcd.print(week4);
  if(instWeek == 5) lcd.print(week5);
  if(instWeek == 6) lcd.print(week6);
  checkTimer(1);
  checkTimer(2);
  checkAlarm(1);
  checkAlarm(2);
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  timeModule.gettime();
  instSec = timeModule.seconds;
  instMin = timeModule.minutes;
  instHour = timeModule.hours;
  showTime();
  lcd.setCursor(13 , 0);
  if(timeModule.midday == 0) lcd.print("am");
  if(timeModule.midday == 1) lcd.print("pm");
  lcd.setCursor(0 , 1);
  if(instWeek == 0) lcd.print(week0);
  if(instWeek == 1) lcd.print(week1);
  if(instWeek == 2) lcd.print(week2);
  if(instWeek == 3) lcd.print(week3);
  if(instWeek == 4) lcd.print(week4);
  if(instWeek == 5) lcd.print(week5);
  if(instWeek == 6) lcd.print(week6);
  checkTimer(1);
  checkTimer(2);
  checkAlarm(1);
  checkAlarm(2);
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  timeModule.gettime();
  instSec = timeModule.seconds;
  instMin = timeModule.minutes;
  instHour = timeModule.hours;
  showTime();
  lcd.setCursor(13 , 0);
  if(timeModule.midday == 0) lcd.print("am");
  if(timeModule.midday == 1) lcd.print("pm");
  lcd.setCursor(0 , 1);
  lcd.print("                ");
  lcd.setCursor(0 , 1);
  lcd.print(instDay);
  lcd.print(".");
  lcd.print(instMonth);
  lcd.print(".");
  lcd.print(instYear);
  checkTimer(1);
  checkTimer(2);
  checkAlarm(1);
  checkAlarm(2);
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  timeModule.gettime();
  instSec = timeModule.seconds;
  instMin = timeModule.minutes;
  instHour = timeModule.hours;
  showTime();
  lcd.setCursor(13 , 0);
  if(timeModule.midday == 0) lcd.print("am");
  if(timeModule.midday == 1) lcd.print("pm");
  lcd.setCursor(0 , 1);
  lcd.print("                ");
  lcd.setCursor(0 , 1);
  lcd.print(instDay);
  lcd.print(".");
  lcd.print(instMonth);
  lcd.print(".");
  lcd.print(instYear);
  checkTimer(1);
  checkTimer(2);
  checkAlarm(1);
  checkAlarm(2);
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  timeModule.gettime();
  instSec = timeModule.seconds;
  instMin = timeModule.minutes;
  instHour = timeModule.hours;
  showTime();
  lcd.setCursor(13 , 0);
  if(timeModule.midday == 0) lcd.print("am");
  if(timeModule.midday == 1) lcd.print("pm");
  lcd.setCursor(0 , 1);
  lcd.print("                ");
  lcd.setCursor(0 , 1);
  lcd.print(instDay);
  lcd.print(".");
  lcd.print(instMonth);
  lcd.print(".");
  lcd.print(instYear);
  checkTimer(1);
  checkTimer(2);
  checkAlarm(1);
  checkAlarm(2);
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
  delay(100);
  if(checkButton() != 0) return;
}

void setTime() {
  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("00 : 00 : 00");
  posOfCursor = 0;
  delay(1000);
  
  instHour = getNum()*10;
  showTime();
  posOfCursor++;
  delay(500);
  instHour += getNum();
  showTime();
  delay(500);

  posOfCursor+=4;
  
  instMin = getNum()*10;
  showTime();
  posOfCursor++;
  delay(500);
  instMin += getNum();
  showTime();
  delay(500);

  posOfCursor+=4;
  
  instSec = getNum()*10;
  showTime();
  posOfCursor++;
  delay(500);
  instSec += getNum();
  showTime();
  delay(500);
  lcd.clear();
}

void setTimer(int i) {
  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("00 : 00 : 00");
  delay(1000);

  if(i == 1) {
  timer1Hour = getNum()*10;
  showTimer(i);
  posOfCursor++;
  delay(500);
  timer1Hour += getNum();
  showTimer(i);
  delay(500);

  posOfCursor+=4;
  
  timer1Min = getNum()*10;
  showTimer(i);
  posOfCursor++;
  delay(500);
  timer1Min += getNum();
  showTimer(i);
  delay(500);

  posOfCursor+=4;
  
  timer1Sec = getNum()*10;
  showTimer(i);
  posOfCursor++;
  delay(500);
  timer1Sec += getNum();
  showTimer(i);

  timer1SaveMillis = millis()/1000 + timer1Hour*3600 + timer1Min*60 + timer1Sec;
  timer1Enabled = true;
  delay(500);
  
  } else {

  timer2Hour = getNum()*10;
  showTimer(i);
  posOfCursor++;
  delay(500);
  timer2Hour += getNum();
  showTimer(i);
  delay(500);

  posOfCursor+=4;
  
  timer2Min = getNum()*10;
  showTimer(i);
  posOfCursor++;
  delay(500);
  timer2Min += getNum();
  showTimer(i);
  delay(500);

  posOfCursor+=4;
  
  timer2Sec = getNum()*10;
  showTimer(i);
  posOfCursor++;
  delay(500);
  timer2Sec += getNum();
  showTimer(i);

  timer2SaveMillis = millis()/1000 + timer2Hour*3600 + timer2Min*60 + timer2Sec;
  timer2Enabled = true;
  delay(500);
  
  }
  
  lcd.clear();
  delay(500);
}

void setAlarm(int i) {
  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("00 : 00 : 00");
  delay(1000);

  if(i == 1) {
  alarm1Hour = getNum()*10;
  showAlarm(i);
  posOfCursor++;
  delay(500);
  alarm1Hour += getNum();
  showAlarm(i);
  delay(500);

  posOfCursor+=4;
  
  alarm1Min = getNum()*10;
  showAlarm(i);
  posOfCursor++;
  delay(500);
  alarm1Min += getNum();
  showAlarm(i);
  delay(500);

  posOfCursor+=4;
  
  alarm1Sec = getNum()*10;
  showAlarm(i);
  posOfCursor++;
  delay(500);
  alarm1Sec += getNum();
  showAlarm(i);
  alarm1Enabled = true;
  timeModule.gettime();
  delay(500);
  
  } else {

  alarm2Hour = getNum()*10;
  showAlarm(i);
  posOfCursor++;
  delay(500);
  alarm2Hour += getNum();
  showAlarm(i);
  delay(500);

  posOfCursor+=4;
  
  alarm2Min = getNum()*10;
  showAlarm(i);
  posOfCursor++;
  delay(500);
  alarm2Min += getNum();
  showAlarm(i);
  delay(500);

  posOfCursor+=4;
  
  alarm2Sec = getNum()*10;
  showAlarm(i);
  posOfCursor++;
  delay(500);
  alarm2Sec += getNum();
  showAlarm(i);
  alarm2Enabled = true;
  delay(500);
  
  }
  
  lcd.clear();
  delay(500);
}

void setMonth() {
  lcd.setCursor(0 , 0);
  lcd.print("month:");
   int i = 1;
  while(checkButton() != 1) {
    if(checkButton() == 2) i++;
    if(checkButton() == 3) i--;
    lcd.setCursor(0 , 1);
    if(i == 13) i = 1;
    if(i == 0) i = 12;
    if(i == 1) lcd.print(month1);
    if(i == 2) lcd.print(month2);
    if(i == 3) lcd.print(month3);
    if(i == 4) lcd.print(month4);
    if(i == 5) lcd.print(month5);
    if(i == 6) lcd.print(month6);
    if(i == 7) lcd.print(month7);
    if(i == 8) lcd.print(month8);
    if(i == 9) lcd.print(month9);
    if(i == 10) lcd.print(month10);
    if(i == 11) lcd.print(month11);
    if(i == 12) lcd.print(month12);
    lcd.print("     ");
    delay(150);
  }
  instMonth = i;
  lcd.clear();
}

void setWeek() {
  lcd.setCursor(0 , 0);
  lcd.print("day of week:");
  int i = 0;
  while(checkButton() != 1) {
    if(checkButton() == 2) i++;
    if(checkButton() == 3) i--;
    lcd.setCursor(0 , 1);
    if(i == 7) i = 0;
    if(i == -1) i = 6;
    if(i == 0) lcd.print(week0);
    if(i == 1) lcd.print(week1);
    if(i == 2) lcd.print(week2);
    if(i == 3) lcd.print(week3);
    if(i == 4) lcd.print(week4);
    if(i == 5) lcd.print(week5);
    if(i == 6) lcd.print(week6);
    lcd.print("     ");
    delay(150);
  }
  instWeek = i;
  lcd.clear();
}

void setDay() {
  posOfCursor = 5;
  lcd.clear();
  lcd.setCursor(0 , 0); 
  lcd.print("Day: 00") ;
  delay(500);
  instDay = getNum()*10;
  showDay();
  posOfCursor++;
  delay(500);
  instDay += getNum();
  showDay();
  delay(500);
  lcd.clear();
}

void setYear() {
  posOfCursor = 6;
  lcd.clear();
  lcd.setCursor(0 , 0); 
  lcd.print("Year: 0000") ;
  delay(500);
  instYear = getNum()*1000;
  showYear();
  posOfCursor++;
  delay(500);
  instYear += getNum()*100;
  showYear();
  posOfCursor++;
  delay(500);
  instYear += getNum()*10;
  showYear();
  posOfCursor++;
  delay(500);
  instYear += getNum();
  showYear();
  posOfCursor++;
  delay(500);
  lcd.clear();
}

void refreshTimer(int i) {
  if(i == 1) {
    long s = ( timer1SaveMillis - millis()/1000 );
    int h = s / 3600;
    s -= h*3600;
    
    int m = s / 60;
    s -= m*60;
    timer1Min = m;
    timer1Hour = h;
    timer1Sec = s;
  } else {
    long s = ( timer2SaveMillis - millis()/1000 );
    int h = s / 3600;
    s -= h*3600;
    
    int m = s / 60;
    s -= m*60;
    timer2Min = m;
    timer2Hour = h;
    timer2Sec = s;
  }
}

void audioSignal(int i) {
  analogWrite(micro , i);
}

void checkTimer(int i) {
  if(i == 1) {
    if(timer1Enabled == true) {
      if(timer1SaveMillis <= millis()/1000) {
        lcd.clear();
        lcd.print("timer 1");
        while(checkButton() != 1) {
          audioSignal(255);
          delay(500);
          audioSignal(0);
          delay(500);
        }
        timer1Enabled = false;
        menu = 0;
        delay(1000);
      }
    }
  } else {
    if(timer2Enabled == true) {
      if(timer2SaveMillis <= millis()/1000) {
        lcd.clear();
        lcd.print("timer 2");
        while(checkButton() != 1) {
          audioSignal(255);
          delay(500);
          audioSignal(0);
          delay(500);
        }
        timer2Enabled = false;
        menu = 0;
        delay(1000);
      }
    }
  }
}

void checkAlarm(int i) {
  if(i == 1) {
    if(alarm1Enabled == true) {
      timeModule.gettime();
      long alarmTime = alarm1Hour;
      alarmTime*=60;
      alarmTime += alarm1Min;
      alarmTime *= 60;
      alarmTime += alarm1Sec;
      long nowTime = timeModule.hours;
      if(timeModule.midday == 1) nowTime += 12;
      nowTime *= 60;
      nowTime += timeModule.minutes;
      nowTime *= 60;
      nowTime += timeModule.seconds;
      Serial.println(alarmTime);
      Serial.println(nowTime);
      if(alarmTime <= nowTime && (alarm1Hour == timeModule.hours || alarm1Hour == timeModule.hours + 12 || alarm1Hour == timeModule.hours- 12)) {
        lcd.clear();
        lcd.print("alarm 1");
        while(checkButton() != 1) {
          audioSignal(255);
          delay(500);
          audioSignal(0);
          delay(500);
        }
        alarm1Enabled = false;
        menu = 0;
        delay(1000);
      }
    }
  } else {
    if(alarm2Enabled == true) {
      timeModule.gettime();
      long alarmTime = alarm2Hour;
      alarmTime*=60;
      alarmTime += alarm2Min;
      alarmTime *= 60;
      alarmTime += alarm2Sec;
      long nowTime = timeModule.hours;
      if(timeModule.midday == 1) nowTime += 12;
      nowTime *= 60;
      nowTime += timeModule.minutes;
      nowTime *= 60;
      nowTime += timeModule.seconds;
      Serial.println(alarmTime);
      Serial.println(nowTime);
      if(alarmTime <= nowTime && (alarm2Hour == timeModule.hours || alarm2Hour == timeModule.hours+12 || alarm2Hour == timeModule.hours-12)) {
        lcd.clear();
        lcd.print("alarm 2");
        while(checkButton() != 1) {
          audioSignal(255);
          delay(500);
          audioSignal(0);
          delay(500);
        }
        alarm2Enabled = false;
        menu = 0;
        delay(1000);
      }
    }
  }
}

void loop(){
  if(firstStart == true) {
    setTime();
    setMonth();
    setDay();
    setYear();
    setWeek();
    timeModule.settime(instSec,instMin,instHour,instDay,instMonth,instYear,instWeek);  // 0  сек, 51 мин, 21 час, 27, октября, 2015 года, вторник
    lcd.clear();
    delay(500);
    menu = 0;
    firstStart = false;
  } else {
    checkTimer(1);
    checkTimer(2);
    checkAlarm(1);
    checkAlarm(2);
    if(menu == 0) {
      //lcd.clear();
      timeModule.gettime();
      instSec = timeModule.seconds;
      instMin = timeModule.minutes;
      instHour = timeModule.hours;
      instDay = timeModule.day;
      instMonth = timeModule.month;
      instYear = timeModule.year;
      instWeek = timeModule.weekday;
      showTime();
      lcd.setCursor(13 , 0);
      if(timeModule.midday == 0) lcd.print("am");
      if(timeModule.midday == 1) lcd.print("pm");
      showDateAndMonth();
    }
    if(menu == 1) {
      lcd.setCursor(0 , 0);
      lcd.print("timer 1");
      if(timer1Enabled == true) {
      lcd.setCursor(0 , 1);
      refreshTimer(1);
      lcd.setCursor(0 , 1);
      if(timer1Hour < 10) lcd.print("0");
      lcd.print(timer1Hour);
      lcd.print(" : ");
      if(timer1Min < 10) lcd.print("0");
      lcd.print(timer1Min);
      lcd.print(" : ");
      if(timer1Sec < 10) lcd.print("0");
      lcd.print(timer1Sec);
      } else {
      lcd.setCursor(0 , 1);
      lcd.print("no timer");
      }
      if(checkButton() == 1) {posOfCursor = 0 ;setTimer(1);}
    }
    if(menu == 2) {
      lcd.setCursor(0 , 0);
      lcd.print("timer 2");
      if(timer2Enabled == true) {
      lcd.setCursor(0 , 1);
      refreshTimer(2);
      lcd.setCursor(0 , 1);
      if(timer2Hour < 10) lcd.print("0");
      lcd.print(timer2Hour);
      lcd.print(" : ");
      if(timer2Min < 10) lcd.print("0");
      lcd.print(timer2Min);
      lcd.print(" : ");
      if(timer2Sec < 10) lcd.print("0");
      lcd.print(timer2Sec);
      } else {
      lcd.setCursor(0 , 1);
      lcd.print("no timer");
      }
      if(checkButton() == 1) {posOfCursor = 0 ;setTimer(2);}
    }
    if(menu == 3) {
      lcd.setCursor(0 , 0);
      lcd.print("alarm 1");
      if(alarm1Enabled == true) {
      lcd.setCursor(0 , 1);
      if(alarm1Hour < 10) lcd.print("0");
      lcd.print(alarm1Hour);  
      lcd.print(" : ");
      if(alarm1Min < 10) lcd.print("0");
      lcd.print(alarm1Min);
      lcd.print(" : ");
      if(alarm1Sec < 10) lcd.print("0");
      lcd.print(alarm1Sec);
      } else {
      lcd.setCursor(0 , 1);
      lcd.print("no alarm");
      }
      if(checkButton() == 1) {posOfCursor = 0 ;setAlarm(1);}
    }
    if(menu == 4) {
      lcd.setCursor(0 , 0);
      lcd.print("alarm 2");
      if(alarm2Enabled == true) {
      lcd.setCursor(0 , 1);
      if(alarm2Hour < 10) lcd.print("0");
      lcd.print(alarm2Hour);  
      lcd.print(" : ");
      if(alarm2Min < 10) lcd.print("0");
      lcd.print(alarm2Min);
      lcd.print(" : ");
      if(alarm2Sec < 10) lcd.print("0");
      lcd.print(alarm2Sec);
      } else {
      lcd.setCursor(0 , 1);
      lcd.print("no alarm");
      }
      if(checkButton() == 1) {posOfCursor = 0 ;setAlarm(2);}
    }
    if(menu == 5) {
      lcd.setCursor(0 , 0);
      lcd.print("settings");
      if(checkButton() == 1) {
        firstStart = true;
      }
    }
    if(menu == 6) {
      lcd.setCursor(0 , 0);
      lcd.print("timer1  ");
      if(timer1Enabled == true) {
      refreshTimer(1);
      if(timer1Hour < 10) lcd.print("0");
      lcd.print(timer1Hour);
      lcd.print(":");
      if(timer1Min < 10) lcd.print("0");
      lcd.print(timer1Min);
      lcd.print(":");
      if(timer1Sec < 10) lcd.print("0");
      lcd.print(timer1Sec);
      } else {
      lcd.print("no timer");
      }
      lcd.setCursor(0 , 1);
      lcd.print("timer2  ");
      if(timer2Enabled == true) {
      refreshTimer(2);
      if(timer2Hour < 10) lcd.print("0");
      lcd.print(timer2Hour);
      lcd.print(":");
      if(timer2Min < 10) lcd.print("0");
      lcd.print(timer2Min);
      lcd.print(":");
      if(timer2Sec < 10) lcd.print("0");
      lcd.print(timer2Sec);
      } else {
      lcd.print("no timer");
      }
    }
    if(checkButton() == 2) {menu++;lcd.clear();}
    if(checkButton() == 3) {menu--;lcd.clear();}
    if(menu == 7) menu = 0;
    if(menu == -1) menu = 6;
    delay(200);
  }
}
