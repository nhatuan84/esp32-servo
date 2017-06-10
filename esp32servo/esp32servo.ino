
typedef enum {
  SERVO_STATE_PON = 1,
  SERVO_STATE_ON,
  SERVO_STATE_POFF,
  SERVO_STATE_OFF
}ServoState;

ServoState servoState = SERVO_STATE_PON;

int REFRESH_TIME = 20000;
long lastMsg = 0;
uint16_t usPulse = 0;
int curpos = 0;
int p1 = 0;
int p2 = 0;
int servoPin = 12;
int flip = false;
bool timeout(void){
  long now = micros();
  if (usPulse > 0 && now - lastMsg >= usPulse) {
    lastMsg = now;
    return true;
  } 
  return false; 
}
void setRange(int fst, int sec){
  p1 = fst;
  p2 = sec;
  curpos = fst;
  servoState = SERVO_STATE_PON;
}
int calPulse(int myAngle){
  return ((myAngle * 10) + 600);  
}

void ServoLoop(void){
  switch(servoState){
    case SERVO_STATE_PON:
    digitalWrite(servoPin, HIGH);
    lastMsg = micros();
    if(flip == false){
      if(curpos >= p1 && curpos < p2){
        curpos++;
      }else if(curpos == p2){
        flip = true;
      }
    }else{
      if(curpos > p1 && curpos <= p2){
        curpos--;
      }else if(curpos == p1){
        flip = false;
      }
    }
    usPulse = calPulse(curpos);
    servoState = SERVO_STATE_ON;
    
    break;
    case SERVO_STATE_ON:
      if(timeout()){
        servoState = SERVO_STATE_POFF;
      }
    break;
    case SERVO_STATE_POFF:
      digitalWrite(servoPin, LOW);
      lastMsg = micros();
      usPulse = (REFRESH_TIME - usPulse);
      servoState = SERVO_STATE_OFF;
    break;
    case SERVO_STATE_OFF:
      if(timeout()){
        servoState = SERVO_STATE_PON;
      }
    break;
    default:
    break;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(servoPin, OUTPUT);
  setRange(0, 180);
}

void loop() {
  ServoLoop();
}
