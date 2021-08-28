#define PWMA 5
#define DIRA 0
#define PWMB 4
#define DIRB 2
#define UV D8
#define IR1 D0
#define IR2 D1
#define IR3 D2
#define IR4 D3

#define DEBUG true

void time_delay(uint8_t pin, int interval_in_seconds){
    digitalWrite(pin,HIGH);
    delay(interval_in_seconds*1000);
    digitalWrite(pin,LOW);
  }

void motor_debug(uint8_t analog_pin, uint8_t digital_pin, int counts, bool reverse){
    digitalWrite(digital_pin,1);
    for (int i =0; i<counts; i++){
        analogWrite(analog_pin, (1000/counts)*i);
        delay(1500);
        analogWrite(analog_pin,0);
      }
    if (reverse){
    digitalWrite(digital_pin,0);
    for (int i =0; i<counts; i++){
        analogWrite(analog_pin, (1000/counts)*i);
        delay(1500);
        analogWrite(analog_pin,0);
      }
     }
  }

void setup() {
  Serial.begin(115200);
  pinMode(DIRA, OUTPUT);
  pinMode(PWMA, OUTPUT); 
  pinMode(DIRB, OUTPUT);
  pinMode(PWMB, OUTPUT);  
  pinMode(UV,OUTPUT);
  pinMode(IR1,OUTPUT);
  pinMode(IR2,OUTPUT);
  pinMode(IR3,OUTPUT);
  pinMode(IR4,OUTPUT);
  
  if (DEBUG){
    Serial.println("Debug");
    Serial.println("Testing UV for 10 seconds");
    time_delay(UV,10);
    Serial.println("Testing Motor A");
    motor_debug(PWMA,DIRA,5,true);
    Serial.println("Testing Motor B");
    motor_debug(PWMB,DIRB,5,true);
    }
 
}

void loop() {
  

}
