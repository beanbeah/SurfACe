#define PWMA 5
#define DIRA 0
#define PWMB 4
#define DIRB 2
#define UV D8
#define FR_IR D0
#define FL_IR D5
#define BR_IR D6
#define BL_IR D7
#define MOVE_SPEED 50
#define BRAKE_SPEED 15

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

void debug() {
  Serial.print(isOnFloor(FL_IR));
  Serial.print(isOnFloor(FR_IR));
  Serial.print(isOnFloor(BL_IR));
  Serial.print(isOnFloor(BR_IR));
  Serial.print("\n");
}

enum State{
  ON_TABLE,
  OFF_TABLE,
  REVERSE,
  TERMINATION_LAP,
  TERMINATE
};

void setup() {
  Serial.begin(115200);
  
  // Motor setup
  Motor lMotor = {PWMB, DIRB, true};
  Motor rMotor = {PWMA, DIRA, true};
  setupMotor(lMotor);
  setupMotor(rMotor);

  // uv and ir setup
//  pinMode(UV,OUTPUT)
  pinMode(FL_IR, INPUT);
  pinMode(FR_IR, INPUT);
  pinMode(BL_IR, INPUT);
  pinMode(BR_IR, INPUT);

  //table code
  int turnDir = -1;
  enum State state = ON_TABLE;
  while(true) {
    switch(state) {
      case ON_TABLE:
      {
        if(isOnFloor(FL_IR)) {
           moveMotor(lMotor, MOVE_SPEED);
        }else {
           moveMotor(lMotor, 0);
        }
        if(isOnFloor(FR_IR)) {
           moveMotor(rMotor, MOVE_SPEED);
        }else {
           moveMotor(rMotor, 0);
        }
        delay(5);
        if(!isOnFloor(FL_IR) && !isOnFloor(FR_IR)) state = OFF_TABLE;
        break;
      }
      case OFF_TABLE:
      {
        turnDir = -turnDir;
        // Reverse 
        moveMotor(lMotor, -50);
        moveMotor(rMotor, -50);
        delay(500);
        moveMotor(lMotor, 15);
        moveMotor(rMotor, 15);
        
        // turn 90
        moveMotor(lMotor, MOVE_SPEED*turnDir);
        moveMotor(rMotor, -MOVE_SPEED*turnDir);
        delay(910);
        moveMotor(lMotor, -15*turnDir);
        moveMotor(rMotor, 15*turnDir);
        
        // move fwd
        moveMotor(lMotor, 50);
        moveMotor(rMotor, 50);
//        delay(1000);
        int i = 0;
        bool offTable = false;
        while(i < 1000) {
          if(!isOnFloor(FL_IR) && !isOnFloor(FR_IR)) {
            offTable = true;
            break;
          }
          i += 5;
          delay(5);
        }
        moveMotor(lMotor, -15);
        moveMotor(rMotor, -15);
        
        if(offTable) {
          state = TERMINATION_LAP;
          break;
        }else {
          // turn 90
          moveMotor(lMotor, MOVE_SPEED*turnDir);
          moveMotor(rMotor, -MOVE_SPEED*turnDir);
          delay(870);
          moveMotor(lMotor, -15*turnDir);
          moveMotor(rMotor, 15*turnDir);
          state = ON_TABLE;
          break;
        }
      }
      case TERMINATION_LAP:
      {
        // Reverse 
        moveMotor(lMotor, -50);
        moveMotor(rMotor, -50);
        delay(500);
        moveMotor(lMotor, 15);
        moveMotor(rMotor, 15);
        
        // turn 90
        moveMotor(lMotor, MOVE_SPEED*turnDir);
        moveMotor(rMotor, -MOVE_SPEED*turnDir);
        delay(910);
        moveMotor(lMotor, -15*turnDir);
        moveMotor(rMotor, 15*turnDir);

        while(isOnFloor(FL_IR) || isOnFloor(FR_IR)) {
          if(isOnFloor(FL_IR)) {
            moveMotor(lMotor, MOVE_SPEED);
          }else {
             moveMotor(lMotor, 0);
          }
          if(isOnFloor(FR_IR)) {
             moveMotor(rMotor, MOVE_SPEED);
          }else {
             moveMotor(rMotor, 0);
          }
          delay(5);
        }
        state = TERMINATE;
        break;
      }
      case TERMINATE:
      {
        moveMotor(lMotor, 0);
        moveMotor(rMotor, 0);
        break;
      }
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
//      delay(1MOVE_SPEED0);
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
