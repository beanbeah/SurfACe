#define PWMA 5
#define DIRA 0
#define PWMB 4
#define DIRB 2
#define UV D8
#define FR_IR D0
#define FL_IR D5
#define BR_IR D6
#define BL_IR D7

#define DEBUG true

struct Motor {
  const int PIN;
  const int DIR_PIN;
  const bool reversed;
};

void setupMotor(Motor motor) {
  pinMode(motor.PIN, OUTPUT);
  pinMode(motor.DIR_PIN, OUTPUT);
}

void moveMotor(Motor motor, int speed) {
  if(motor.reversed) speed = -speed;
  digitalWrite(motor.DIR_PIN, speed >= 1);
  analogWrite(motor.PIN, abs(speed));
}

//void get

void setUV() {
    
}

bool isOnFloor(uint8_t pin) {
  return digitalRead(pin) == 0;
}

void setup() {
  Serial.begin(115200);
  
  // Motor setup
  Motor lMotor = {PWMB, DIRB, true};
  setupMotor(lMotor);
  Motor rMotor = {PWMA, DIRA, true};
  setupMotor(rMotor);


  Serial.print("fuck you");
  // uv and ir setup
//  pinMode(UV,OUTPUT)
  pinMode(FL_IR, INPUT);
  pinMode(FR_IR, INPUT);
  pinMode(BL_IR, INPUT);
  pinMode(BR_IR, INPUT);

  while(true) {
     Serial.print("fl, fr sensor: ");
     Serial.print(isOnFloor(FL_IR));
     Serial.print(isOnFloor(FR_IR));
     Serial.print(isOnFloor(BL_IR));
     Serial.print(isOnFloor(BR_IR));
     Serial.print("\n");
  
     if(isOnFloor(FL_IR)) {
        moveMotor(lMotor, 50);
     }else {
        moveMotor(lMotor, 0);
     }
     if(isOnFloor(FR_IR)) {
        moveMotor(rMotor, 50);
     }else {
        moveMotor(rMotor, 0);
     }
    delay(5);
  }
}

void loop() {
   
}

//void time_delay(uint8_t pin, int interval){
//  digitalWrite(pin,HIGH);
//  delay(interval*1000);
//  digitalWrite(pin,LOW);
//} 

//void motor_debug(uint8_t analog_pin, uint8_t digital_pin, int counts, bool reverse){
//  digitalWrite(digital_pin,1);
//  for (int i =0; i<counts; i++){
//      analogWrite(analog_pin, (1000/counts)*i);
//      delay(1500);
//      analogWrite(analog_pin,0);
//    }
//  if (reverse){
//  digitalWrite(digital_pin,-1);
//  for (int i =0; i<counts; i++){
//      analogWrite(analog_pin, (1000/counts)*i);
//      delay(1500);
//      analogWrite(analog_pin,0);
//    }
//   }
//}
