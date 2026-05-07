// Title: Traffic Light Controller
// Author: Ashot Nahapetyan
// Instructors: Narine Hovhannisyan, Satenik Mnatsakanyan
// Course: ENGS110 Intro to Programming
// SPRING 2026

// Defining pins on the Arduino
#define NS_PIN_RED 2
#define NS_PIN_YELLOW 3
#define NS_PIN_GREEN 4
#define EW_PIN_RED 5
#define EW_PIN_YELLOW 6
#define EW_PIN_GREEN 7
#define PED_NS_RED 11
#define PED_NS_GREEN 10
#define PED_EW_RED 9
#define PED_EW_GREEN 8
#define BUZZER A1
#define BTN_NS A2
#define BTN_EW A3

// Timing configurations
#define GREEN_TIME 5000
#define YELLOW_TIME 3000
#define BLINK_TIME 400
#define SAFETY_GAP 0 //change this if you want to add a safety gap
#define PED_TIME 5000
#define PED_WARN_TIME 1500
#define PED_FLASH_RATE 250
#define BUZZ_WAIT_FREQ 880
#define BUZZ_WALK_FREQ 2500

// Custom "State" type storing the current phase as a state, each all red state behaves different
enum State {
  NS_GREEN,
  NS_YELLOW,
  ALL_RED1, 
  PED_NS,
  PED_EW,
  ALL_RED2,
  EW_GREEN,
  EW_YELLOW,
  ALL_RED3
};

State currState = NS_GREEN;
unsigned long stateStart = 0;
// button press flags
bool btnNS = false;
bool btnEW = false;

// vehicle lights
void setNS(bool r, bool y, bool g) {
  digitalWrite(NS_PIN_RED, r ? HIGH : LOW);
  digitalWrite(NS_PIN_YELLOW, y ? HIGH : LOW);
  digitalWrite(NS_PIN_GREEN, g ? HIGH : LOW);
}
void setEW(bool r, bool y, bool g) {
  digitalWrite(EW_PIN_RED, r ? HIGH : LOW);
  digitalWrite(EW_PIN_YELLOW, y ? HIGH : LOW);
  digitalWrite(EW_PIN_GREEN, g ? HIGH : LOW);
}
// both sides red
void setAllRed() {
  setNS(true, false, false);
  setEW(true, false, false);
}

// pedestrian lights
void pedNS_stop() {
  digitalWrite(PED_NS_RED, HIGH);
  digitalWrite(PED_NS_GREEN, LOW);
}
void pedNS_go() {
  digitalWrite(PED_NS_RED, LOW);
  digitalWrite(PED_NS_GREEN, HIGH);
}
void pedNS_SetGreen(bool on) {
  digitalWrite(PED_NS_GREEN, on ? HIGH : LOW);
  digitalWrite(PED_NS_RED, LOW);
}
void pedEW_stop() {
  digitalWrite(PED_EW_RED, HIGH);
  digitalWrite(PED_EW_GREEN, LOW);
}
void pedEW_go() {
  digitalWrite(PED_EW_RED, LOW);
  digitalWrite(PED_EW_GREEN, HIGH);
}
void pedEW_SetGreen(bool on) {
  digitalWrite(PED_EW_GREEN, on ? HIGH : LOW);
  digitalWrite(PED_EW_RED, LOW);
}

void allPedStop() {
  pedNS_stop();
  pedEW_stop();
}


// program core
void changeState(State state) {
  currState = state;
  stateStart = millis();
  switch (state) {
    case NS_GREEN:
      setNS(false, false, true);
      setEW(true,  false, false);
      allPedStop();
      break;

    case NS_YELLOW:
      setNS(false, true, true);
      setEW(true,  false, false);
      allPedStop();
      break;

    case ALL_RED1:
    case ALL_RED2:
    case ALL_RED3:
      setAllRed();
      noTone(BUZZER);
      allPedStop();
      break;

    case PED_NS:
      setAllRed();
      pedNS_go();
      pedEW_stop();
      break;

    case PED_EW:
      setAllRed();
      pedNS_stop();
      pedEW_go();
      break;

    case EW_GREEN:
      setNS(true,  false, false);
      setEW(false, false, true);
      allPedStop();
      break;

    case EW_YELLOW:
      setNS(true,  false, false);
      setEW(false, true,  true);
      allPedStop();
      break;
  }
}

void readButtons() {
  if (digitalRead(BTN_NS) == LOW) btnNS = true;
  if (digitalRead(BTN_EW) == LOW) btnEW = true;
}

void pedestrianPhase(bool isNS) {
  unsigned long elapsed = millis() - stateStart;
  unsigned long remaining = (elapsed < PED_TIME) ? (PED_TIME - elapsed) : 0;
  
  bool inWarning = (remaining > 0 && remaining <= PED_WARN_TIME);

  if (inWarning) {
    unsigned long warningElapsed = PED_WARN_TIME - remaining;
    bool greenOn = (warningElapsed / PED_FLASH_RATE) % 2 == 0;

    if (isNS) {
      pedNS_SetGreen(greenOn);
    } else {
      pedEW_SetGreen(greenOn);
    }
    
    if (greenOn) {
      tone(BUZZER, BUZZ_WALK_FREQ);
    } else {
      noTone(BUZZER);
    }

  } else if (remaining > 0) {
  
    if (isNS) {
      pedNS_go();
    } else {
    pedEW_go();
    }

    bool buzzOn = (elapsed % 200) < 100;
    if (buzzOn) {
      tone(BUZZER, BUZZ_WALK_FREQ);
    } else {
      noTone(BUZZER);
    }

  } else {

    if (isNS) {
      pedNS_stop();
    } else {
      pedEW_stop();
    }
    noTone(BUZZER);
  }
}

void launchtest() {
  setNS(true, false, false);
  setEW(true, false, false);
  delay(500);
  setNS(false, true, false);
  setEW(false, true, false);
  delay(500);
  setNS(false, false, true);
  setEW(false, false, true);
  delay(500);
  setNS(false, false, false);
  setEW(false, false, false);
  pedNS_go(); pedEW_go();
  delay(500);
  pedNS_stop(); pedEW_stop();
  delay(500);
  tone(BUZZER, 880,  150); delay(200);
  tone(BUZZER, 1200, 150); delay(200);
  tone(BUZZER, 2500, 150); delay(250);
  tone(BUZZER, 600, 150); delay(200);
  noTone(BUZZER);
  delay(500);
}
void yellowFlash(int pin) {
  unsigned long now = millis();
  unsigned long interval = now - stateStart;
  bool on = (interval / BLINK_TIME) % 2 == 0;
  digitalWrite(pin, on ? HIGH : LOW);

  if (on) {
    tone(BUZZER, BUZZ_WAIT_FREQ);
  } else {
    noTone(BUZZER);
  }
}

void setup() {
  pinMode(NS_PIN_RED, OUTPUT);
  pinMode(NS_PIN_YELLOW, OUTPUT);
  pinMode(NS_PIN_GREEN, OUTPUT);
  pinMode(EW_PIN_RED, OUTPUT);
  pinMode(EW_PIN_YELLOW, OUTPUT);
  pinMode(EW_PIN_GREEN, OUTPUT);
  pinMode(PED_NS_RED, OUTPUT);
  pinMode(PED_NS_GREEN, OUTPUT);
  pinMode(PED_EW_RED, OUTPUT);
  pinMode(PED_EW_GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_NS, INPUT_PULLUP);
  pinMode(BTN_EW, INPUT_PULLUP);
  // does these below before starting the main loop
  launchtest();
  allPedStop();
  changeState(NS_GREEN);
}

// program loop
void loop() {
  readButtons();
  unsigned long now = millis();
  unsigned long elapsed = now - stateStart;
  switch (currState) {
    case NS_GREEN:
      if (elapsed >= GREEN_TIME) {
        changeState(NS_YELLOW);
      }
      break;

    case NS_YELLOW:
      yellowFlash(NS_PIN_YELLOW);
      yellowFlash(EW_PIN_YELLOW);
      if (elapsed >= YELLOW_TIME) {
        digitalWrite(NS_PIN_YELLOW, LOW);
        changeState(ALL_RED1);
      }
    break;

    case ALL_RED1:
      if (elapsed >= SAFETY_GAP) {
        if (btnNS) {
          btnNS = false; 
          changeState(PED_NS);
        } else if (btnEW) {
          btnEW = false;
          changeState(PED_EW);
        } else {
          changeState(EW_GREEN);
        }
      }
      break;

    case PED_NS:
      pedestrianPhase(true);
      if (elapsed >= PED_TIME) {
        noTone(BUZZER);
        pedNS_stop();
        if (btnEW) {
          btnEW = false;
          changeState(PED_EW);
        } else {
          changeState(ALL_RED2);
        }
      }
      break;

    case PED_EW:
      pedestrianPhase(false);
      if (elapsed >= PED_TIME) {
        noTone(BUZZER);
        pedEW_stop();
        changeState(ALL_RED2);
        if (btnNS) {
          btnNS = false;
          changeState(PED_NS);
        } else {
          changeState(ALL_RED2);
        }
      }
      break;

    case ALL_RED2:
      if (elapsed >= SAFETY_GAP) {
        changeState(EW_GREEN);
      }
      break;
    
    case EW_GREEN:
      if (elapsed >= GREEN_TIME) {
        changeState(EW_YELLOW);
      }
      break;

    case EW_YELLOW:
      yellowFlash(EW_PIN_YELLOW);
      yellowFlash(NS_PIN_YELLOW);
      if (elapsed >= YELLOW_TIME) {
        noTone(BUZZER);
        digitalWrite(EW_PIN_YELLOW, LOW);
        changeState(ALL_RED3);
      }
      break;

    case ALL_RED3:
      if (elapsed >= SAFETY_GAP) {
        if (btnEW) {
          btnEW = false;
          changeState(PED_EW);
        } else if (btnNS) {
          btnNS = false;
          changeState(PED_NS);
        } else {
          changeState(NS_GREEN);
        }
      }
      break;
  }
}

