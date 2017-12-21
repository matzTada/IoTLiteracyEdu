/*
   mote.ino
   Program for Arduino
   Keio University Westlab 2016.10
   Author: Tada Matz
*/

//<==== Please change according to an instruction
String MOTENAME = "yourname"; //recommended to be between 1 ~ 10
int MOTEID = 0; //should be from 1 ~ 20
uint32_t DEST_ADDR_LSB = 0x40B0A672; // LSB of COODINATOR
//====>Please change according to an instruction

#include "XBee.h"
#include "source.h"

Servo myservo;
MyXBee myxbee;

int oldButtonState;
int SWITCH_ARRAY_PIN[5];    //switch
int switch_values[5];
unsigned long serialPreviousMillis;    //loop serial print
unsigned long receiveLedPreviousMillis;    //Receive LED
unsigned long clickLedPreviousMillis;
unsigned long servoPreviousMillis;

//#define WITH_PERIODIC
#ifdef WITH_PERIODIC
unsigned long sendPastMillis = millis();
#define SEND_INTERVAL 1000
#endif

void setup() {
  Serial.begin(9600);

  serialPreviousMillis = millis();
  receiveLedPreviousMillis = millis();
  clickLedPreviousMillis = millis();
  servoPreviousMillis = millis();

  oldButtonState = 0;
  int tempArray[] = {8, 7, 6, 5, 4};
  for (int i = 0; i < 5; i++) SWITCH_ARRAY_PIN[i] = tempArray[i];
  for (int i = 0; i < 5; i++) switch_values[i] = 0;

  pinMode(RECEIVE_LED_PIN, OUTPUT);
  digitalWrite(RECEIVE_LED_PIN, HIGH);

  pinMode(CLICK_LED_PIN, OUTPUT);
  digitalWrite(CLICK_LED_PIN, HIGH);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  for (int i = 0; i < 5; i++) pinMode(SWITCH_ARRAY_PIN[i], INPUT_PULLUP);

  myservo.attach(SERVO_PIN, 1500, 1900); //attach(pin number(must be PWM pin), MIN pulse width, MAX pulse width)
  myservo.write(0);

  myxbee.init();
}

void loop() { //loop
  if (millis() - receiveLedPreviousMillis >= RECEIVE_LED_ON_INTERVAL) digitalWrite(RECEIVE_LED_PIN, LOW);  //put off receive LED
  if (millis() - clickLedPreviousMillis >= CLICK_LED_ON_INTERVAL) digitalWrite(CLICK_LED_PIN, LOW);  //put off click LED
  if (millis() - servoPreviousMillis >= SERVO_ON_INTERVAL) myservo.write(0);  //make servo default

  //check receiving data
  myxbee.receiveXBeeData(myservo);

  //sending data
  if (clickDetection() == 1) {
    int valueFromSwitches = getValueFromSwitches();
    float tempTemperature = getTemperature(TEMP_SENSOR_PIN);
    char tempStr[1 + 1 + 4 + 1];
    sprintf(tempStr, "%c%c%04d%c",
            UPLINK_HEADER,
            MOTEID + int(ID_PACKET_OFFSET),
            int(tempTemperature * 10),
            valueFromSwitches + int(ID_PACKET_OFFSET));
    String temppayload = "";
    temppayload += tempStr + MOTENAME + "\n";
    myxbee.sendXBeeData(temppayload);
  }

#ifdef WITH_PERIODIC
  if (millis() - sendPastMillis > SEND_INTERVAL) {
    sendPastMillis += SEND_INTERVAL;
    int valueFromSwitches = getValueFromSwitches();
    float tempTemperature = getTemperature(TEMP_SENSOR_PIN);
    char tempStr[1 + 1 + 4 + 1];
    sprintf(tempStr, "%c%c%04d%c",
            UPLINK_HEADER,
            MOTEID + int(ID_PACKET_OFFSET),
            int(tempTemperature * 10),
            valueFromSwitches + int(ID_PACKET_OFFSET));
    String temppayload = "";
    temppayload += tempStr + MOTENAME + "\n";
    myxbee.sendXBeeData(temppayload);
  }
#endif

  //loop seconds
  if (millis() - serialPreviousMillis > 1000) {
    serialPreviousMillis += 1000;
    Serial.println(F("in a loop"));
  }
}

int clickDetection() { //click detection
  int buttonClicked = 0;
  int newButtonState = digitalRead(BUTTON_PIN);
  if (oldButtonState == HIGH && newButtonState == LOW) { //button pressed. CAUTION pull up
    buttonClicked = 1;
    Serial.println(F("Button clicked"));
    //put on click LED
    digitalWrite(CLICK_LED_PIN, HIGH);
    clickLedPreviousMillis = millis();
  }
  oldButtonState = newButtonState;
  return buttonClicked;
}

int getValueFromSwitches() { //get value from switches
  Serial.print(F("switch_values: "));
  int value_from_switches = 0;
  for (int i = 0; i < 5; i++) {
    switch_values[i] = digitalRead(SWITCH_ARRAY_PIN[i]);
    if (switch_values[i] == LOW) value_from_switches += (1 << i); //switched
    Serial.print(switch_values[i]);
    Serial.print(",");
  }
  Serial.print(F("value_from_swtches: "));
  Serial.print(value_from_switches);
  Serial.println("");
  return value_from_switches;
}

float getTemperature(int pin) { //Measure temperature, origined from Bob-san's program
  int v = 1023 - analogRead(pin);
  float res = (1023.0 / v) - 1;
  res = SERIESRESISTOR / res;
  float temp = (1 / (0.00096564 + (0.00021068 * log(res) ) + (0.000000085826 * ( pow( log(res) , 3))))) - 273.15;
  return temp;
}
