/*********************************************************************
Replacement controller board for Record Power AC400 air filtering system.

Requires:
  IRLremote (https://github.com/NicoHood/IRLremote)
Arduino standard libraries:
  EEPROM
  SoftwareSerial
*********************************************************************/

#include <IRLremote.h>

#define LED_PIN     13
#define IR_PIN       2
#define BEEP_PIN     5
#define KEY_ON_PIN   3
#define KEY_OFF_PIN  4
#define SPEED_ZERO_PIN 5
#define SPEED_LOW_PIN 6
#define SPEED_MEDIUM_PIN 7
#define SPEED_HIGH_PIN 8
#define SPEED_COMMON_PIN 9

#define LED_LOW_PIN A0
#define LED_MEDIUM_PIN A1
#define LED_HIGH_PIN A2
#define LED_ONE_PIN A5
#define LED_TWO_PIN A4
#define LED_THREE_PIN A3

#define RELAY_OFF LOW
#define RELAY_ON HIGH
#define KEY_ADDRESS 0xFF00
#define KEY_OFF_IR  0x90
#define KEY_ON_IR   0x30
#define KEY_TIME_IR 0xA0

#define SPEED_OFF 0
#define SPEED_LOW 1
#define SPEED_MEDIUM 2
#define SPEED_HIGH 3

#define AUTO_OFF 0
#define AUTO_ONE 1
#define AUTO_TWO 2
#define AUTO_THREE 3
#define AUTO_ONE_DURATION 3600000
#define AUTO_TWO_DURATION 7200000
#define AUTO_THREE_DURATION 14400000
//#define AUTO_ONE_DURATION 10000
//#define AUTO_TWO_DURATION 20000
//#define AUTO_THREE_DURATION 40000


CNec IRLremote;

int speed = SPEED_OFF;
int auto_off = AUTO_OFF;
unsigned long lastAutoTime;
unsigned long auto_delay;

int buttonState[2] = { LOW, LOW };       // the current reading from the input pin
int lastButtonState[2] = { LOW, LOW };   // the previous reading from the input pin
unsigned long lastDebounceTime[2] = { 0, 0 };  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void speedOff();

void setup()
{
  // put your setup code here, to run once:
  //Serial.begin(9600);
  //Serial.println("AC400 controller");
  IRLremote.begin(IR_PIN);
  
  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, LOW);
  
  pinMode(KEY_ON_PIN, INPUT_PULLUP);
  pinMode(KEY_OFF_PIN, INPUT_PULLUP);

  pinMode(LED_LOW_PIN, OUTPUT);
  pinMode(LED_MEDIUM_PIN, OUTPUT);
  pinMode(LED_HIGH_PIN, OUTPUT);
  pinMode(LED_ONE_PIN, OUTPUT);
  pinMode(LED_TWO_PIN, OUTPUT);
  pinMode(LED_THREE_PIN, OUTPUT);
  
  digitalWrite(LED_LOW_PIN, LOW);
  digitalWrite(LED_MEDIUM_PIN, LOW);
  digitalWrite(LED_HIGH_PIN, LOW);
  digitalWrite(LED_ONE_PIN, LOW);
  digitalWrite(LED_TWO_PIN, LOW);
  digitalWrite(LED_THREE_PIN, LOW);

  pinMode(SPEED_LOW_PIN, OUTPUT);
  pinMode(SPEED_MEDIUM_PIN, OUTPUT);
  pinMode(SPEED_HIGH_PIN, OUTPUT);
  pinMode(SPEED_COMMON_PIN, OUTPUT);
  digitalWrite(SPEED_COMMON_PIN, RELAY_OFF);
  speedOff();
}

void beep(unsigned long duration, unsigned long pause = 0)
{
  digitalWrite(BEEP_PIN, HIGH);
  delay(duration);
  digitalWrite(BEEP_PIN, LOW);
  if(pause > 0)
    delay(pause);
}

void speedOff()
{
  digitalWrite(SPEED_LOW_PIN, RELAY_OFF);
  digitalWrite(SPEED_MEDIUM_PIN, RELAY_OFF);
  digitalWrite(SPEED_HIGH_PIN, RELAY_OFF);
}

void setSpeed(int new_speed)
{
  digitalWrite(LED_LOW_PIN, LOW);
  digitalWrite(LED_MEDIUM_PIN, LOW);
  digitalWrite(LED_HIGH_PIN, LOW);

  switch(new_speed)
  {
    case SPEED_HIGH:
      //digitalWrite(BEEP_PIN, HIGH);
      //delay(200);
      //digitalWrite(BEEP_PIN, LOW);
      //delay(100);
      beep(200, 100);
    case SPEED_MEDIUM:
      //digitalWrite(BEEP_PIN, HIGH);
      //delay(200);
      //digitalWrite(BEEP_PIN, LOW);
      //delay(100);
      beep(200, 100);
    case SPEED_LOW:
      //digitalWrite(BEEP_PIN, HIGH);
      //delay(200);
      //digitalWrite(BEEP_PIN, LOW);
      speedOff();
      beep(200);
  }
  
  speed = new_speed;
  
  digitalWrite(SPEED_COMMON_PIN, RELAY_ON);
  digitalWrite(SPEED_ZERO_PIN+speed, RELAY_ON);

  switch(speed)
  {
    case SPEED_LOW:
      digitalWrite(LED_LOW_PIN, HIGH);
      break;
    case SPEED_MEDIUM:
      digitalWrite(LED_MEDIUM_PIN, HIGH);
      break;
    case SPEED_HIGH:
      digitalWrite(LED_HIGH_PIN, HIGH);
      break;
  }
}

void setNextSpeed()
{
  int s = speed+1;
  if(s > SPEED_HIGH)
    s = SPEED_LOW;
  setSpeed(s);
}

void setSpeedOff()
{
  speed = SPEED_OFF;
  auto_off = AUTO_OFF;
  //digitalWrite(BEEP_PIN, HIGH);
  //delay(500);
  //digitalWrite(BEEP_PIN, LOW);
  beep(500);
  speedOff();
  digitalWrite(SPEED_COMMON_PIN, RELAY_OFF);
  digitalWrite(LED_LOW_PIN, LOW);
  digitalWrite(LED_MEDIUM_PIN, LOW);
  digitalWrite(LED_HIGH_PIN, LOW);
  digitalWrite(LED_ONE_PIN, LOW);
  digitalWrite(LED_TWO_PIN, LOW);
  digitalWrite(LED_THREE_PIN, LOW);
}

void setAuto(int new_auto)
{
  if(speed == SPEED_OFF)
    new_auto = AUTO_OFF;
  digitalWrite(LED_ONE_PIN, LOW);
  digitalWrite(LED_TWO_PIN, LOW);
  digitalWrite(LED_THREE_PIN, LOW);

  switch(new_auto)
  {
    case AUTO_THREE:
      //digitalWrite(BEEP_PIN, HIGH);
      //delay(50);
      //digitalWrite(BEEP_PIN, LOW);
      //delay(100);
      beep(50, 100);
      beep(200, 100);
      beep(200, 100);
      beep(200);
      break;
    case AUTO_TWO:
      //digitalWrite(BEEP_PIN, HIGH);
      //delay(50);
      //digitalWrite(BEEP_PIN, LOW);
      //delay(100);
      //beep(50, 50);
      //beep(200, 100);
      //beep(50, 50);
      //beep(200);
      beep(50, 100);
      beep(200, 100);
      beep(200);
      break;
    case AUTO_ONE:
      //digitalWrite(BEEP_PIN, HIGH);
      //delay(50);
      //digitalWrite(BEEP_PIN, LOW);
      //delay(100);
      //digitalWrite(BEEP_PIN, HIGH);
      //delay(50);
      //digitalWrite(BEEP_PIN, LOW);
      beep(50, 100);
      beep(200);
      break;
    case AUTO_OFF:
      //digitalWrite(BEEP_PIN, HIGH);
      //delay(50);
      //digitalWrite(BEEP_PIN, LOW);
      beep(50, 100);
      beep(50);
      break;
  }

  auto_off = new_auto;
  lastAutoTime = millis();
  
  switch(auto_off)
  {
    case AUTO_ONE:
      digitalWrite(LED_ONE_PIN, HIGH);
      auto_delay = AUTO_ONE_DURATION;
      break;
    case AUTO_TWO:
      digitalWrite(LED_TWO_PIN, HIGH);
      auto_delay = AUTO_TWO_DURATION;
      break;
    case AUTO_THREE:
      digitalWrite(LED_THREE_PIN, HIGH);
      auto_delay = AUTO_THREE_DURATION;
      break;
  }
  //Serial.print("Auto off at ");
  //Serial.print(auto_delay);
  //Serial.print(" current time ");
  //Serial.println(lastAutoTime);
}

void setNextAuto()
{
  int a = auto_off + 1;
  if(a > AUTO_THREE)
    a = AUTO_OFF;
  setAuto(a);
}

bool pressed(uint8_t key_pin)
{
  // read the state of the switch into a local variable:
  int reading = !digitalRead(key_pin);
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState[key_pin - KEY_ON_PIN])
  {
    // reset the debouncing timer
    lastDebounceTime[key_pin - KEY_ON_PIN] = millis();
    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState[key_pin - KEY_ON_PIN] = reading;
    return false;
  }

  if ((millis() - lastDebounceTime[key_pin - KEY_ON_PIN]) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState[key_pin - KEY_ON_PIN])
    {
      buttonState[key_pin - KEY_ON_PIN] = reading;

      // only if the new button state is HIGH
      if (buttonState[key_pin - KEY_ON_PIN] == HIGH)
      {
        return true;
      }
    }
  }
  return false;
}

void loop()
{
  if(pressed(KEY_ON_PIN))
  {
    setNextSpeed();
  }
  if(pressed(KEY_OFF_PIN))
  {
    setSpeedOff();
  }
  
  if(IRLremote.available()) // have we received an IR signal?
  {
    auto data = IRLremote.read();
    //Serial.print(data.address, HEX);
    //Serial.print(" ");
    //Serial.println(data.command, HEX);
    if(data.address == KEY_ADDRESS)
    {
      switch(data.command)
      {
        case KEY_ON_IR:
          setNextSpeed();
          break;
        case KEY_OFF_IR:
          setSpeedOff();
          break;
        case KEY_TIME_IR:
          setNextAuto();
          break;
      }
    }
  }

  if(auto_off != AUTO_OFF)
  {
    unsigned long auto_elapsed = millis() - lastAutoTime;
    if(auto_elapsed > auto_delay)
    {
      setSpeedOff();
      //Serial.print("Auto off at ");
      //Serial.print(mil);
      //Serial.print(" after waiting for ");
      //Serial.print(auto_elapsed);
      //Serial.println(" millis");
    }
    else
    {
      unsigned long auto_remaining = auto_delay - auto_elapsed;
      if(auto_off == AUTO_THREE && auto_remaining < AUTO_TWO_DURATION)
      {
        setAuto(AUTO_TWO);
      }
      else if(auto_off == AUTO_TWO && auto_remaining < AUTO_ONE_DURATION)
      {
        setAuto(AUTO_ONE);
      }
    }
  }
}
