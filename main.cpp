#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>


#define laser_pin 34
#define light_pin 35
#define buzzer_pin 4


#define SS_PIN 5
#define RST_PIN 21

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  
  Serial.begin(115200);
  pinMode(laser_pin,OUTPUT);
  pinMode(light_pin,INPUT);
  pinMode(buzzer_pin,OUTPUT);
  digitalWrite(laser_pin,HIGH);
  SPI.begin(18, 19, 23, 5);
  rfid.PCD_Init();
}
bool systemArmed = false;
void loop() {
  
  int light;
  light=digitalRead(light_pin);
  if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    byte authorizedUID[]={0x4E, 0x55, 0x31, 0x07}; 
    bool authorized = false;
    if(rfid.uid.size == sizeof(authorizedUID)){
      authorized = true;
      for (byte i = 0; i < sizeof(authorizedUID); i++) {
        if (rfid.uid.uidByte[i] != authorizedUID[i]) {
          authorized = false;
          break;
        }
      }
    
      if(authorized){
        systemArmed = !systemArmed;
        if(systemArmed){
          Serial.println("System armed");
        }
        else{
          Serial.println("System disarmed");
          noTone(buzzer_pin);
        }
      }
      else{
        Serial.println("Unauthorized card");
      }
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
  delay(1000);
  if(systemArmed){
    Serial.println("System is armed");
    if(light==HIGH){
      Serial.println("laser broken!Alarm triggered");
      tone(buzzer_pin,1000);
    }
    else{
      noTone(buzzer_pin);
    }
  delay(0.001);
  }
  else{
    Serial.println("System is disarmed");
  }
  
}






