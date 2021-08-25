#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define PWMA 5
#define DIRA 0
#define PWMB 4
#define DIRB 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting...");



  Serial.println("Preparing motor...");
  pinMode(DIRB, OUTPUT);
  pinMode(PWMB, OUTPUT);  

  pinMode(D8, OUTPUT);
  digitalWrite(D8, HIGH);
  Serial.println("UV dying");
  delay(10000);
  digitalWrite(D8,LOW);

  analogWrite(PWMB, 500);
  digitalWrite(DIRB, 1);
  delay(1000);
  analogWrite(PWMB, 0);
delay(1000);
  analogWrite(PWMA, 500);
  digitalWrite(DIRA, 1);
  delay(1000);
  analogWrite(PWMA, 0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
