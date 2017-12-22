#define LED_RED_PIN 3
#define LED_GREEN_PIN 5
#define LED_BLUE_PIN 6

#define BUTTON_RED_PIN 10
#define BUTTON_GREEN_PIN 11
#define BUTTON_BLUE_PIN 12

int oldR = 0;
int oldG = 0;
int oldB = 0;

#define CNT_MAX 2
int cntR = 0;
int cntG = 0;
int cntB = 0;

int clickDetection(int _pin, int * _oldstate) { //click detection
  int buttonClicked = 0;
  int newButtonState = digitalRead(_pin);
  if (*_oldstate == HIGH && newButtonState == LOW) buttonClicked = 1; //button pressed. CAUTION pull up
  *_oldstate = newButtonState;
  return buttonClicked;
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);
  pinMode(BUTTON_GREEN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE_PIN, INPUT_PULLUP);
}

void loop() {
  if (clickDetection(BUTTON_RED_PIN, &oldR)) {
    cntR = cntR + 1;
    if (cntR >= CNT_MAX) cntR = 0;
  }

  if (clickDetection(BUTTON_GREEN_PIN, &oldG)) {
    cntG = cntG + 1;
    if (cntG >= CNT_MAX) cntG = 0;
  }

  if (clickDetection(BUTTON_BLUE_PIN, &oldB)) {
    cntB = cntB + 1;
    if (cntB >= CNT_MAX) cntB = 0;
  }

  Serial.println(String(cntR) + "," + String(cntG) + "," + String(cntB));

  analogWrite(LED_RED_PIN, (int)map(cntR, 0, CNT_MAX, 255, 0));
  analogWrite(LED_GREEN_PIN, (int)map(cntG, 0, CNT_MAX, 255, 0));
  analogWrite(LED_BLUE_PIN, (int)map(cntB, 0, CNT_MAX, 255, 0));
}


