
#define MODE_SENSORS 6
#define HYSTERESIS_TEMP_STEP 0.5
#define HYSTERESIS_HUM_STEP 5
#define DHT_SENSOR 0 // IF connected DHT 1 else 0
#define CELLTEMPD1 1
#define CELLTEMPN1 3
#define CELLTEMPD2 5
#define CELLTEMPN2 7
#define CELLTEMPD3 9
#define CELLTEMPN3 11
#define CELLTEMPD4 13
#define CELLTEMPN4 15
#define CELLHUMD1 100
#define CELLHUMN1 102
#define CELLHUMD2 104
#define CELLHUMN2 106
#define UVPIN 6 // UV light relay
#define tPIN1 13 //  pin relay
#define tPIN2 12 //  pin relay
#define tPIN3 11 //  pin relay
#define tPIN4 10 //  pin relay
#define DHtPIN A0 // DHT pin
#define DHtPIN2 A1 // DHT pin
#define DHTTYPE DHT22 // DHT model 
#define LEDPIN 9 // LED light pin
#include <TimeLib.h>
#include <DHT.h>
#include "RTClib.h"
#include <EEPROM.h>
#include <SerialCommand.h>
#include <OneWire.h>
OneWire ds (A1);

byte data[12];
byte addr1[8];
byte addr2[8];// ID each for DS18DB20
byte addr3[8];
byte addr4[8];
byte i;
unsigned int raw;
float temp1;
float temp2;
float temp3;
float temp4;
boolean Day;

SerialCommand sCmd;     // The demo SerialCommand object
//DS18B22


// Terrarium
int sensor_mode = MODE_SENSORS;
//int UVpower = HIGH; // UV

int tStatus1 = HIGH;
int tStatus2 = HIGH;
int tStatus3 = HIGH;
int tStatus4 = HIGH;
int hStatus = HIGH;
int temp_Day1;
int temp_Night1;
int temp_Day2;
int temp_Night2;
int temp_Day3;
int temp_Night3;
int temp_Day4;
int temp_Night4;
int hum_Day1;
int hum_Night1;
int hum_Day2;
int hum_Night2;

// Terrarium

int Bright = 0; //
int LightOn = 8; // Sunrise
int LightOff = 20; //  Sunset

RTC_DS3231 rtc;
uint32_t syncProvider() { //function which sets up the RTC as the source of external time
  return rtc . now() . unixtime();
}
DHT dht1(DHtPIN, DHTTYPE); //  dht
DHT dht2(DHtPIN2, DHTTYPE); //  dht
void setup() {
  Serial.begin(9600);
  rtc.begin();
  // rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(__DATE__, __TIME__));//comment this out when the RTC has been set
  setSyncProvider(syncProvider);   // the function to get the time from the RTC





  pinMode(LEDPIN, OUTPUT);
  pinMode(UVPIN, OUTPUT);
  callbacks();  // Setup callbacks for SerialCommand commands
  SENSOR_SEARCH();  // search onewire sensors


  switch (sensor_mode) {
    case 1:
      mode1();
      break;
    case 2:
      mode2();
      break;
    case 3:
      mode3();
      break;
    case 4:
      mode4();
      break;
    case 5:
      mode5();
      break;
    case 6:
      mode6();
      break;
    case 7:
      mode7();
      break;
    case 8:
      mode8();
      break;
  }


}
void loop() {

  sCmd.readSerial();
  DateTime now = rtc.now();

  int Hour = now.hour() ; //
  int Minute = now.minute();
  int Second = now.second() ;
  boolean Day = (Hour >= 8 & Hour < 20); //
  boolean Light = (Hour >= LightOn & Hour < LightOff); //

  switch (sensor_mode) {
    case 1:
      temp1 = DS18B20(addr1);
      tStatus1 = tempcontrol(temp1, tPIN1, Day, temp_Day1, temp_Night1, HYSTERESIS_TEMP_STEP);
      break;
    case 2:
      temp1 = dht1.readTemperature();
      temp2 = dht1.readHumidity();
      tStatus1 = tempcontrol(temp1, tPIN1, Day, temp_Day1, temp_Night1, HYSTERESIS_TEMP_STEP);
      tStatus2 = tempcontrol(temp2, tPIN2, Day, hum_Day1, hum_Night1, HYSTERESIS_HUM_STEP);
      break;
    case 3:
      temp1 = DS18B20(addr1);
      temp2 = dht1.readTemperature();
      temp3 = dht1.readHumidity();
      tStatus1 = tempcontrol(temp1, tPIN1, Day, temp_Day1, temp_Night1, HYSTERESIS_TEMP_STEP);
      tStatus2 = tempcontrol(temp2, tPIN2, Day, temp_Day2, temp_Night2, HYSTERESIS_TEMP_STEP);
      tStatus3 = tempcontrol(temp3, tPIN3, Day, hum_Day1, hum_Night1, HYSTERESIS_HUM_STEP);
      break;
    case 4:
      temp1 = DS18B20(addr1);
      temp2 = DS18B20(addr2);
      tStatus1 = tempcontrol(temp1, tPIN1, Day, temp_Day1, temp_Night1, HYSTERESIS_TEMP_STEP);
      tStatus2 = tempcontrol(temp2, tPIN2, Day, temp_Day2, temp_Night2, HYSTERESIS_TEMP_STEP);
      break;
    case 5:
      temp1 = DS18B20(addr1);
      temp2 = DS18B20(addr2);
      temp3 = DS18B20(addr3);
      tStatus1 = tempcontrol(temp1, tPIN1, Day, temp_Day1, temp_Night1, HYSTERESIS_TEMP_STEP);
      tStatus2 = tempcontrol(temp2, tPIN2, Day, temp_Day2, temp_Night2, HYSTERESIS_TEMP_STEP);
      tStatus3 = tempcontrol(temp3, tPIN3, Day, temp_Day3, temp_Night3, HYSTERESIS_TEMP_STEP);
      break;
    case 6:
      temp1 = DS18B20(addr1);
      temp2 = DS18B20(addr2);
      temp3 = DS18B20(addr3);
      temp4 = DS18B20(addr4);
      tStatus1 = tempcontrol(temp1, tPIN1, Day, temp_Day1, temp_Night1, HYSTERESIS_TEMP_STEP);
      tStatus2 = tempcontrol(temp2, tPIN2, Day, temp_Day2, temp_Night2, HYSTERESIS_TEMP_STEP);
      tStatus3 = tempcontrol(temp3, tPIN3, Day, temp_Day3, temp_Night3, HYSTERESIS_TEMP_STEP);
      tStatus4 = tempcontrol(temp4, tPIN4, Day, temp_Day4, temp_Night4, HYSTERESIS_TEMP_STEP);
      break;
    case 7:
      temp1 = DS18B20(addr1);
      temp2 = DS18B20(addr2);
      temp3 = dht1.readTemperature();
      temp4 = dht1.readHumidity();
      tStatus1 = tempcontrol(temp1, tPIN1, Day, temp_Day1, temp_Night1, HYSTERESIS_TEMP_STEP);
      tStatus1 = tempcontrol(temp1, tPIN2, Day, temp_Day2, temp_Night2, HYSTERESIS_TEMP_STEP);
      tStatus3 = tempcontrol(temp3, tPIN3, Day, temp_Day3, temp_Night3, HYSTERESIS_TEMP_STEP);
      tStatus4 = tempcontrol(temp4, tPIN4, Day, hum_Day1, hum_Night1, HYSTERESIS_HUM_STEP);
      break;
    case 8:
      temp1 = dht1.readTemperature();
      temp2 = dht1.readHumidity();
      temp3 = dht2.readTemperature();
      temp4 = dht2.readHumidity();
      tStatus1 = tempcontrol(temp1, tPIN1, Day, temp_Day1, temp_Night1, HYSTERESIS_TEMP_STEP);
      tStatus2 = tempcontrol(temp2, tPIN2, Day, hum_Day1, hum_Night1, HYSTERESIS_HUM_STEP);
      tStatus3 = tempcontrol(temp3, tPIN3, Day, temp_Day2, temp_Night2, HYSTERESIS_TEMP_STEP);
      tStatus4 = tempcontrol(temp4, tPIN4, Day, hum_Day2, hum_Night2, HYSTERESIS_HUM_STEP);
      break;
  }
}
int tempcontrol(float temp, int PIN, boolean Day, int pDay, int pNight, int Hstep) { //Hysteresis
  int Status;
  if (temp <= ((Day) ? pDay - Hstep : pNight - Hstep)) Status = HIGH;  //
  if (temp >= ((Day) ? pDay + Hstep : pNight + Hstep)) Status = LOW; //
  digitalWrite(PIN, Status);
  delay (50);
  return Status;
}
void callbacks() {  // Setup callbacks for SerialCommand commands

  sCmd.addCommand("R",    READ);          // READ DATA
  sCmd.addCommand("W",    WRITE);          // WRITE to memory
  sCmd.addCommand("TW",    TIMEWRITE);          // WRITE to memory
  sCmd.addCommand("TR",    TIMEREAD);          // WRITE to memory
  sCmd.addCommand("HELP",    HELP);          // WRITE to memory

}
void mode1() {
  temp_Day1 = CheckEEPROM (CELLTEMPD1, 40);
  temp_Night1 = CheckEEPROM (CELLTEMPN1, 40);
  pinMode(tPIN1, OUTPUT);

}
void mode2() {
  dht1.begin();
  temp_Day1 = CheckEEPROM (CELLTEMPD1, 40);
  temp_Night1 = CheckEEPROM (CELLTEMPN1, 40);
  hum_Day1 = CheckEEPROM (CELLHUMD1, 40);
  hum_Night1 = CheckEEPROM (CELLHUMN1, 70);
  pinMode(tPIN1, OUTPUT);
  pinMode(tPIN2, OUTPUT);
}
void mode3() {
  dht1.begin();
  temp_Day1 = CheckEEPROM (CELLTEMPD1, 40);
  temp_Night1 = CheckEEPROM (CELLTEMPN1, 40);
  temp_Day2 = CheckEEPROM (CELLTEMPD2, 40);
  temp_Night2 = CheckEEPROM (CELLTEMPN2, 40);
  hum_Day1 = CheckEEPROM (CELLHUMD1, 40);
  hum_Night1 = CheckEEPROM (CELLHUMN1, 70);
  pinMode(tPIN1, OUTPUT);
  pinMode(tPIN2, OUTPUT);
  pinMode(tPIN3, OUTPUT);
}
void mode4() {
  temp_Day1 = CheckEEPROM (CELLTEMPD1, 40);
  temp_Night1 = CheckEEPROM (CELLTEMPN1, 40);
  temp_Day2 = CheckEEPROM (CELLTEMPD2, 40);
  temp_Night2 = CheckEEPROM (CELLTEMPN2, 40);
  pinMode(tPIN1, OUTPUT);
  pinMode(tPIN2, OUTPUT);
}
void mode5() {
  temp_Day1 = CheckEEPROM (CELLTEMPD1, 40);
  temp_Night1 = CheckEEPROM (CELLTEMPN1, 40);
  temp_Day2 = CheckEEPROM (CELLTEMPD2, 40);
  temp_Night2 = CheckEEPROM (CELLTEMPN2, 40);
  temp_Day3 = CheckEEPROM (CELLTEMPD3, 40);
  temp_Night3 = CheckEEPROM (CELLTEMPN3, 40);
  pinMode(tPIN1, OUTPUT);
  pinMode(tPIN2, OUTPUT);
  pinMode(tPIN3, OUTPUT);
}
void mode6() {
  temp_Day1 = CheckEEPROM (CELLTEMPD1, 40);
  temp_Night1 = CheckEEPROM (CELLTEMPN1, 40);
  temp_Day2 = CheckEEPROM (CELLTEMPD2, 40);
  temp_Night2 = CheckEEPROM (CELLTEMPN2, 40);
  temp_Day3 = CheckEEPROM (CELLTEMPD3, 40);
  temp_Night3 = CheckEEPROM (CELLTEMPN3, 40);
  temp_Day4 = CheckEEPROM (CELLTEMPD4, 40);
  temp_Night4 = CheckEEPROM (CELLTEMPN4, 40);
  pinMode(tPIN1, OUTPUT);
  pinMode(tPIN2, OUTPUT);
  pinMode(tPIN3, OUTPUT);
  pinMode(tPIN4, OUTPUT);
}
void mode7() {
  mode2();
  dht2.begin();
  temp_Day3 = CheckEEPROM (CELLTEMPD3, 40);
  temp_Night3 = CheckEEPROM (CELLTEMPN3, 40);
  pinMode(tPIN3, OUTPUT);

}
void mode8() {
  mode2();
  temp_Day2 = CheckEEPROM (CELLTEMPD1, 40);
  temp_Night2 = CheckEEPROM (CELLTEMPN1, 40);
  hum_Day2 = CheckEEPROM (CELLHUMD2, 40);
  hum_Night2 = CheckEEPROM (CELLHUMN2, 70);
  pinMode(tPIN3, OUTPUT);
  pinMode(tPIN4, OUTPUT);

}
int CheckEEPROM (byte cell, int t) {
  int param;
  param = EEPROM.read(cell); // Получаем параметр из  ячейки
  if (param >= t) param = t; // Если ячейка пуста, т.е. ее значение = 255, то устанавливаем параметр температуры в значение по умолчанию, в данном случае 26С.

  return param;
}
void callsensor () {

}
void SENSOR_SEARCH() {
  // SENSOR SEARCH // SENSOR SEARCH // SENSOR SEARCH // SENSOR SEARCH // SENSOR SEARCH // SENSOR SEARCH
  Serial.println("Getting the address...\n\r");
  /* initiate a search for the OneWire object we created and read its value into
    addr array we declared above*/

  while (ds.search(addr1)) {
    Serial.print("The address is:\t");
    //read each byte in the address array
    for ( i = 0; i < 8; i++) {
      Serial.print("0x");
      if (addr1[i] < 16) {
        Serial.print('0');
      }
      // print each byte in the address array in hex format
      Serial.print(addr1[i], HEX);
      if (i < 7) {
        Serial.print(", ");
      }
    }
    // a check to make sure that what we read is correct.
    if ( OneWire::crc8( addr1, 7) != addr1[7]) {
      Serial.print("CRC is not valid!\n");
      return;
    }
  }
  while (ds.search(addr2)) {
    Serial.print("The address is:\t");
    //read each byte in the address array
    for ( i = 8; i < 16; i++) {
      Serial.print("0x");
      if (addr2[i] < 16) {
        Serial.print('0');
      }
      // print each byte in the address array in hex format
      Serial.print(addr2[i], HEX);
      if (i < 7) {
        Serial.print(", ");
      }
    }
    // a check to make sure that what we read is correct.
    if ( OneWire::crc8( addr2, 15) != addr2[15]) {
      Serial.print("CRC is not valid!\n");
      return;
    }
  }
  ds.reset_search();
  return;
  // SENSOR SEARCH // SENSOR SEARCH // SENSOR SEARCH // SENSOR SEARCH // SENSOR SEARCH
}
void print_sensor_data() {
  switch (sensor_mode) {
    case 1:
      Serial.print("Temp1 = "); Serial.println(temp1);
      break;
    case 2:
      Serial.print("Temp DHT= "); Serial.println(temp1);
      Serial.print("Hum DHT= "); Serial.println(temp2);
      break;
    case 3:
      Serial.print("Temp = "); Serial.println(temp1);
      Serial.print("Temp DHT= "); Serial.println(temp2);
      Serial.print("Hum DHT= "); Serial.println(temp3);
      break;
    case 4:
      Serial.print("Temp = "); Serial.println(temp1);
      Serial.print("Temp2 = "); Serial.println(temp2);
      break;
    case 5:
      Serial.print("Temp = "); Serial.println(temp1);
      Serial.print("Temp2 = "); Serial.println(temp2);
      Serial.print("Temp3 = "); Serial.println(temp3);

      break;
    case 6:
      Serial.print("Temp = "); Serial.println(temp1);
      Serial.print("Temp2 = "); Serial.println(temp2);
      Serial.print("Temp3 = "); Serial.println(temp3);
      Serial.print("Temp4 = "); Serial.println(temp4);
      break;
    case 7:
      Serial.print("Temp = "); Serial.println(temp1);
      Serial.print("Temp2 = "); Serial.println(temp2);
      Serial.print("Temp DHT = "); Serial.println(temp3);
      Serial.print("Hum DHT = "); Serial.println(temp4);
      break;
    case 8:
      Serial.print("Temp DHT= "); Serial.println(temp1);
      Serial.print("Hum DHT= "); Serial.println(temp2);
      Serial.print("Temp2 DHT= "); Serial.println(temp3);
      Serial.print("Hum2 DHT= "); Serial.println(temp4);
      break;
  }
}
void print_setting_data() {
  switch (sensor_mode) {
    case 1:
      Serial.print("Day temp A = "); Serial.println(temp_Day1);
      Serial.print("Night temp A = "); Serial.println(temp_Night1);
      break;
    case 2:
      Serial.print("Day temp A = "); Serial.println(temp_Day1);
      Serial.print("Night temp A = "); Serial.println(temp_Night1);
      Serial.print("Day hum A = "); Serial.println(hum_Day1);
      Serial.print("Night hum A = "); Serial.println(hum_Night1);
      break;
    case 3:
      Serial.print("Day temp A = "); Serial.println(temp_Day1);
      Serial.print("Night temp A = "); Serial.println(temp_Night1);
      Serial.print("Day temp B = "); Serial.println(temp_Day2);
      Serial.print("Night temp B = "); Serial.println(temp_Night2);
      Serial.print("Day hum B = "); Serial.println(hum_Day1);
      Serial.print("Night hum B = "); Serial.println(hum_Night1);
      break;
    case 4:
      Serial.print("Day temp A = "); Serial.println(temp_Day1);
      Serial.print("Night temp A = "); Serial.println(temp_Night1);
      Serial.print("Day temp B = "); Serial.println(temp_Day2);
      Serial.print("Night temp B = "); Serial.println(temp_Night2);
      break;
    case 5:
      Serial.print("Day temp A = "); Serial.println(temp_Day1);
      Serial.print("Night temp A = "); Serial.println(temp_Night1);
      Serial.print("Day temp B = "); Serial.println(temp_Day2);
      Serial.print("Night temp B = "); Serial.println(temp_Night2);
      Serial.print("Day temp C = "); Serial.println(temp_Day3);
      Serial.print("Night temp C = "); Serial.println(temp_Night3);
      break;
    case 6:
      Serial.print("Day temp A = "); Serial.println(temp_Day1);
      Serial.print("Night temp A = "); Serial.println(temp_Night1);
      Serial.print("Day temp B = "); Serial.println(temp_Day2);
      Serial.print("Night temp B = "); Serial.println(temp_Night2);
      Serial.print("Day temp C = "); Serial.println(temp_Day3);
      Serial.print("Night temp C = "); Serial.println(temp_Night3);
      Serial.print("Day temp D = "); Serial.println(temp_Day4);
      Serial.print("Night temp D = "); Serial.println(temp_Night4);
      break;
    case 7:
      Serial.print("Day temp A = "); Serial.println(temp_Day1);
      Serial.print("Night temp A = "); Serial.println(temp_Night1);
      Serial.print("Day temp B = "); Serial.println(temp_Day2);
      Serial.print("Night temp B = "); Serial.println(temp_Night2);
      Serial.print("Day temp C = "); Serial.println(temp_Day3);
      Serial.print("Night temp C = "); Serial.println(temp_Night3);
      Serial.print("Day hum C = "); Serial.println(hum_Day1);
      Serial.print("Night hum C = "); Serial.println(hum_Night1);
      break;
    case 8:
      Serial.print("Day temp A = "); Serial.println(temp_Day1);
      Serial.print("Night temp A = "); Serial.println(temp_Night1);
      Serial.print("Day hum A = "); Serial.println(hum_Day1);
      Serial.print("Night hum A = "); Serial.println(hum_Night1);
      Serial.print("Day temp B = "); Serial.println(temp_Day2);
      Serial.print("Night temp B = "); Serial.println(temp_Night2);
      Serial.print("Day hum B = "); Serial.println(hum_Day2);
      Serial.print("Night hum B = "); Serial.println(hum_Night2);
      break;
  }
}
void print_status_data() {
  switch (sensor_mode) {
    case 1:
      Serial.print("Heater A = "); Serial.println((tStatus1 == HIGH) ? "On" : "Off");;
      break;
    case 2:
      Serial.print("Heater A = "); Serial.println((tStatus1 == HIGH) ? "On" : "Off");;
      Serial.print("Hum A = "); Serial.println((tStatus2 == HIGH) ? "On" : "Off");;
      break;
    case 3:
      Serial.print("Heater A = "); Serial.println((tStatus1 == HIGH) ? "On" : "Off");;
      Serial.print("Heater B = "); Serial.println((tStatus2 == HIGH) ? "On" : "Off");;
      Serial.print("Hum B = "); Serial.println((tStatus3 == HIGH) ? "On" : "Off");;
      break;
    case 4:
      Serial.print("Heater A = "); Serial.println((tStatus1 == HIGH) ? "On" : "Off");;
      Serial.print("Heater B = "); Serial.println((tStatus2 == HIGH) ? "On" : "Off");;
      break;
    case 5:
      Serial.print("Heater A = "); Serial.println((tStatus1 == HIGH) ? "On" : "Off");;
      Serial.print("Heater B = "); Serial.println((tStatus2 == HIGH) ? "On" : "Off");;
      Serial.print("Heater C = "); Serial.println((tStatus3 == HIGH) ? "On" : "Off");;

      break;
    case 6:
      Serial.print("Heater A = "); Serial.println((tStatus1 == HIGH) ? "On" : "Off");;
      Serial.print("Heater B = "); Serial.println((tStatus2 == HIGH) ? "On" : "Off");;
      Serial.print("Heater C = "); Serial.println((tStatus3 == HIGH) ? "On" : "Off");;
      Serial.print("Heater D = "); Serial.println((tStatus4 == HIGH) ? "On" : "Off");;
      break;
    case 7:
      Serial.print("Heater A = "); Serial.println((tStatus1 == HIGH) ? "On" : "Off");;
      Serial.print("Heater B = "); Serial.println((tStatus2 == HIGH) ? "On" : "Off");;
      Serial.print("Heater C = "); Serial.println((tStatus3 == HIGH) ? "On" : "Off");;
      Serial.print("Hum C = "); Serial.println((tStatus4 == HIGH) ? "On" : "Off");;
      break;
    case 8:
      Serial.print("Heater A = "); Serial.println((tStatus1 == HIGH) ? "On" : "Off");;
      Serial.print("Hum A = "); Serial.println((tStatus2 == HIGH) ? "On" : "Off");;
      Serial.print("Heater B = "); Serial.println((tStatus3 == HIGH) ? "On" : "Off");;
      Serial.print("Hum B = "); Serial.println((tStatus4 == HIGH) ? "On" : "Off");;
      break;
  }
}
void write_setting_data(int p1,int p2,int cellD,int cellN,int terr) {
    EEPROM.write(cellD, p1);
    EEPROM.write(cellN, p2);
    switch (terr) {
    case 1:
       temp_Day1 = EEPROM.read(cellD);
       temp_Night1 = EEPROM.read(cellN); 
       Serial.print("Day A = "); Serial.println(temp_Day1);
       Serial.print("Night A = "); Serial.println(temp_Night1);
       Serial.print(" --- DONE");
      break;
    case 2:
         temp_Day2 = EEPROM.read(cellD);
       temp_Night2 = EEPROM.read(cellN); 
             Serial.print("Day B = "); Serial.println(temp_Day2);
       Serial.print("Night B = "); Serial.println(temp_Night2);
       Serial.print(" --- DONE");
      break;
    case 3:
           temp_Day3 = EEPROM.read(cellD);
       temp_Night3 = EEPROM.read(cellN); 
             Serial.print("Day C = "); Serial.println(temp_Day3);
       Serial.print("Night C = "); Serial.println(temp_Night3);
       Serial.print(" --- DONE");
      break;
    case 4:
        temp_Day4 = EEPROM.read(cellD);
       temp_Night4 = EEPROM.read(cellN); 
             Serial.print("Day D = "); Serial.println(temp_Day4);
       Serial.print("Night D = "); Serial.println(temp_Night4);
       Serial.print(" --- DONE");
      break;
          case 5:
        hum_Day1 = EEPROM.read(cellD);
       hum_Night1 = EEPROM.read(cellN); 
             Serial.print("Day hum A = "); Serial.println(hum_Day1);
       Serial.print("Night hum A = "); Serial.println(hum_Night1);
       Serial.print(" --- DONE");
      break;
          case 6:
        hum_Day2 = EEPROM.read(cellD);
       hum_Night2 = EEPROM.read(cellN); 
                    Serial.print("Day hum B = "); Serial.println(hum_Day2);
       Serial.print("Night hum B = "); Serial.println(hum_Night2);
       Serial.println(" --- DONE");
      break;
    }
       Serial.println("Setting complete");
}
void READ() {

  Serial.println("READ");
  int val;
  char *arg;
  arg = sCmd.next();
  if (strcmp (arg, "sensor") == 0) {
    print_sensor_data();
  }

  if  (strcmp (arg, "set") == 0) {
    print_setting_data();

  }
  if  (strcmp (arg, "status") == 0) {
    print_status_data();
  }
}
void WRITE() {
  Serial.println("WRITE");
  char *arg;
  int p1;
  int p2;
  int terr;
  arg = sCmd.next();

  if  (strcmp (arg, "setA") == 0) {
    terr = 1;
    arg = sCmd.next();
    p1 = atoi(arg);
    arg = sCmd.next();
    p2 = atoi(arg);
    write_setting_data(p1,p2,CELLTEMPD1,CELLTEMPN1,terr);
  }
  if  (strcmp (arg, "setB") == 0) {
      terr = 2;
    arg = sCmd.next();
    p1 = atoi(arg);
    arg = sCmd.next();
    p2 = atoi(arg);
    write_setting_data(p1,p2,CELLTEMPD2,CELLTEMPN2,terr);

  }
    if  (strcmp (arg, "setC") == 0) {
      terr = 3;
    arg = sCmd.next();
    p1 = atoi(arg);
    arg = sCmd.next();
    p2 = atoi(arg);
    write_setting_data(p1,p2,CELLTEMPD3,CELLTEMPN3,terr);

  }
    if  (strcmp (arg, "setD") == 0) {
      terr = 4;
    arg = sCmd.next();
    p1 = atoi(arg);
    arg = sCmd.next();
    p2 = atoi(arg);
    write_setting_data(p1,p2,CELLTEMPD4,CELLTEMPN4,terr);

  }
      if  (strcmp (arg, "setAH") == 0) {
      terr = 5;
    arg = sCmd.next();
    p1 = atoi(arg);
    arg = sCmd.next();
    p2 = atoi(arg);
    write_setting_data(p1,p2,CELLHUMD1,CELLHUMN1,terr);

  }
        if  (strcmp (arg, "setBH") == 0) {
      terr = 6;
    arg = sCmd.next();
    p1 = atoi(arg);
    arg = sCmd.next();
    p2 = atoi(arg);
    write_setting_data(p1,p2,CELLHUMD2,CELLHUMN2,terr);

  }
}
void TIMEWRITE() {
  Serial.println("SetTime");
  char *arg;
  int year;
  int month;
  int day;
  int hour;
  int minutes;
  arg = sCmd.next();
  hour = atoi(arg);
  arg = sCmd.next();
  minutes = atoi(arg);
  arg = sCmd.next();
  day = atoi(arg);
  arg = sCmd.next();
  month = atoi(arg);
  arg = sCmd.next();
  year = atoi(arg);

  rtc.adjust(DateTime(year, month, day, hour, minutes, 0));
  rtc.now();
  Serial.print("Set time to   ");




}
void TIMEREAD() {
  DateTime now = rtc.now();
  int Hour = now.hour() ; // Получаем значение текущего часа
  int Minute = now.minute();

  Serial.print(Hour); Serial.println(":"); Serial.println(Minute);



}
void HELP() {
  Serial.println("W setA_B tD tN");
  Serial.println("R set_dht_t1_t2_hum_status");
  Serial.println("TW Hour Min Day Month Year");
  Serial.println("TR");
}
float DS18B20(byte * adres) { //Считывание температуры
  ds.reset();
  ds.select(adres);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end
  ds.reset();
  ds.select(adres);
  ds.write(0xBE); // Read Scratchpad

  for (byte i = 0; i < 9; i++) // можно увеличить точность измерения до 0.0625 *С (от 9 до 12 бит)
  { // we need 9 bytes
    data[i] = ds.read ();
  }
  raw =  (data[1] << 8) | data[0];//=======Пересчитываем в температуру
  float celsius =  (float)raw / 16.0;
  return celsius;
}
