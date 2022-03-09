#include <ESP8266WiFi.h>

//Constants
const char* ssid     = "surfACe";
const char* password = "12345678";
WiFiServer server(80);
String header;

//ESP8266 Pins
#define PWMA 5
#define DIRA 0
#define PWMB 4
#define DIRB 2
#define UV D6
#define FR_IR D7
#define FL_IR D0
#define BACK_IR D5
#define MOVE_SPEED 50   
#define BRAKE_SPEED 15

//Motor Class 
class Motor {
  private:
    int pin, dirPin;
    bool reversed;

  public:
    Motor(int pin, int dirPin, bool reversed){
      this->pin = pin;
      this->dirPin = dirPin;
      this->reversed = reversed;
    }

    void init(){
      pinMode(this->pin,OUTPUT);
      pinMode(this->dirPin,OUTPUT);
    }

    void move(int speed){
      //reverse motor speed if motor is reversed
      if (this->reversed) speed = -speed;    
      //set motor speed
      digitalWrite(this->dirPin, speed > 0);
      analogWrite(this->pin, abs(speed)); 
    }
};

//set UV-C Relay
void setUV(bool state) {
  digitalWrite(UV, state);
}

//check if Surface is on surface
bool isOnFloor(uint8_t pin) {
  return digitalRead(pin) == 0;
}

//possible states Surface can be in
enum State {
  ON_TABLE,
  OFF_TABLE,
  REVERSE,
  TERMINATION_LAP,
  TERMINATE
};

//check if surface has been lifted up or tilted
bool lifted() {
  return (digitalRead(FR_IR) && digitalRead(FL_IR) && digitalRead(BACK_IR));
}

//DECLARE Motor Object
Motor lMotor(PWMA, DIRA, true);
Motor rMotor(PWMB, DIRB, false);


void setup() {
  Serial.begin(115200); //init debug Serial console
  //declare pinmode
  pinMode(FL_IR, INPUT);
  pinMode(FR_IR, INPUT);
  pinMode(BACK_IR, INPUT);
  pinMode(UV, OUTPUT);
  lMotor.init();
  rMotor.init();
  //init wireless IP
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
}

void loop() {
  int turnDir = -1;
  boolean tracking = false;
  enum State state = ON_TABLE;
  setUV(false);

  WiFiClient client = server.available();
  if (client) {
    lMotor.move(0);
    rMotor.move(0);
    setUV(false);
    
    String currentLine = "";
    while(client.connected()) {
      Serial.println("Connected");
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            if (header.indexOf("GET /stop") >= 0) {
              Serial.println("Stop");
              lMotor.move(0);
              rMotor.move(0);
              setUV(false);
              tracking = false;
            } else if (header.indexOf("GET /spin") >=0){
                setUV(true);
                while(!lifted()){
                  lMotor.move(MOVE_SPEED);
                  rMotor.move(MOVE_SPEED);
                  delay(2);
                }
                lMotor.move(0);
                rMotor.move(0);
                setUV(false);
            }
            else if (header.indexOf("GET /start") >= 0) {
              Serial.println("Start");
              client.println();
              tracking = true;
              while(tracking) {
                Serial.println(state);
                if(header.indexOf("GET /stop") >= 0) {
                  state = TERMINATE;
                  break;
                }
                if (lifted()) {
                  state = TERMINATE;
                } else {
                  setUV(true);
                }
                switch (state) {
                  case ON_TABLE:
                    {
                      if (isOnFloor(FL_IR)) {
                        lMotor.move(MOVE_SPEED);
                      } else {
                        lMotor.move(0);
                      }
                      if (isOnFloor(FR_IR)) {
                        rMotor.move(MOVE_SPEED);
                      } else {
                        rMotor.move(0);
                      }
                      delay(5);
                      if (!isOnFloor(FL_IR) && !isOnFloor(FR_IR)) state = OFF_TABLE;
                      break;
                    }
                  case OFF_TABLE:
                    {
                      turnDir = -turnDir;
                      // Reverse
                      lMotor.move(-50);
                      rMotor.move(-50);
                      delay(500);
                      lMotor.move(15);
                      rMotor.move(15);
          
                      // turn 90
                      lMotor.move(MOVE_SPEED * turnDir);
                      rMotor.move(-MOVE_SPEED * turnDir);
                      delay(910);
                      lMotor.move(-15 * turnDir);
                      rMotor.move(15 * turnDir);
          
                      // move fwd
                      lMotor.move(50);
                      rMotor.move(50);

                      int i = 0;
                      bool offTable = false;
                      while (i < 1000) {
                        if (!isOnFloor(FL_IR) && !isOnFloor(FR_IR)) {
                          offTable = true;
                          break;
                        }
                        i += 5;
                        delay(5);
                      }
                      lMotor.move(-15);
                      rMotor.move(-15);
          
                      if (offTable) {
                        state = TERMINATION_LAP;
                        break;
                      } else {
                        // turn 90
                        lMotor.move(MOVE_SPEED * turnDir);
                        rMotor.move(-MOVE_SPEED * turnDir);
                        delay(850);
                        lMotor.move(-15 * turnDir);
                        rMotor.move(15 * turnDir);
                        state = ON_TABLE;
                        break;
                      }
                    }
                  case TERMINATION_LAP:
                    {
                      // Reverse
                      lMotor.move(-50);
                      rMotor.move(-50);
                      delay(500);
                      lMotor.move(15);
                      rMotor.move(15);
          
                      // turn 90
                      lMotor.move(MOVE_SPEED * turnDir);
                      rMotor.move(-MOVE_SPEED * turnDir);
                      delay(910);
                      lMotor.move(-15 * turnDir);
                      rMotor.move(15 * turnDir);
          
                      while (isOnFloor(FL_IR) || isOnFloor(FR_IR)) {
                        if (isOnFloor(FL_IR)) {
                          lMotor.move(MOVE_SPEED);
                        } else {
                          lMotor.move(0);
                        }
                        if (isOnFloor(FR_IR)) {
                          rMotor.move(MOVE_SPEED);
                        } else {
                          rMotor.move(0);
                        }
                        delay(5);
                      }
                      tracking = false;
                      state = TERMINATE;
                      break;
                    }
                  case TERMINATE:
                    {
                      tracking = false;
                      setUV(false);
                      lMotor.move(0);
                      rMotor.move(0);
                      break;
                    }
                }
          
                delay(5);
              }
              

            }
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            client.println("<body><h1>surfACe</h1>");
            client.println("<p><a href=\"/start\"><button class=\"button\">START</button></a></p>");
            client.println("<p><a href=\"/stop\"><button class=\"button\">STOP</button></a></p>");
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
      
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
