#define LED1 25
#define LED2 26
#define UP_BTN 13
#define DOWN_BTN 12
#define LEFT_BTN 32
#define RIGHT_BTN 33

static const char *buttonGetName(uint8_t number) {
  static const char *buttonName[] = {"NONE", "UP", "DOWN", "LEFT", "RIGHT"};
  uint8_t index = 0;
  switch (number) {
    case UP_BTN:
      index = 1;
      break;
    case DOWN_BTN:
      index = 2;
      break;
    case LEFT_BTN:
      index = 3;
      break;
    case RIGHT_BTN:
      index = 4;
      break;
    default:
      break;
  }
  return buttonName[index];
}

bool evButtons = false;
uint8_t evButtonsData = 0;

unsigned long last_time = 0;

void taskButtons();
void taskSerial();
void taskBeat();

void setup()
{
  Serial.begin(9600);
  taskButtons();
  taskSerial();
  taskBeat();

}

void loop()
{
  taskButtons();
  taskSerial();
  taskBeat();
}

void taskButtons() {
  enum class ButtonsStates {INIT, WAITING_PRESS, WAITING_STABLE, WAITING_RELEASE};
  static ButtonsStates buttonsState =  ButtonsStates::INIT;
  static uint8_t lastButton = 0;
  static uint32_t referenceTime;
  const uint32_t STABLETIMEOUT = 50;

  switch (buttonsState) {
    case ButtonsStates::INIT: {
        pinMode(UP_BTN, INPUT_PULLUP);
        pinMode(DOWN_BTN, INPUT_PULLUP);
        pinMode(LEFT_BTN, INPUT_PULLUP);
        pinMode(RIGHT_BTN, INPUT_PULLUP);
        buttonsState = ButtonsStates::WAITING_PRESS;
        break;
      }
    case ButtonsStates::WAITING_PRESS: {
        if (digitalRead(UP_BTN) == LOW) {
          buttonsState = ButtonsStates::WAITING_STABLE;
          lastButton = UP_BTN;
          referenceTime = millis();
        }
        else if (digitalRead(DOWN_BTN) == LOW) {
          buttonsState = ButtonsStates::WAITING_STABLE;
          lastButton = DOWN_BTN;
          referenceTime = millis();
        }
        else if (digitalRead(LEFT_BTN) == LOW) {
          buttonsState = ButtonsStates::WAITING_STABLE;
          lastButton = LEFT_BTN;
          referenceTime = millis();
        }
        else if (digitalRead(RIGHT_BTN) == LOW) {
          buttonsState = ButtonsStates::WAITING_STABLE;
          lastButton = RIGHT_BTN;
          referenceTime = millis();
        }
        break;
      }
    case ButtonsStates::WAITING_STABLE: {
        if (digitalRead(lastButton) == HIGH) {
          buttonsState = ButtonsStates::WAITING_PRESS;
        }
        else if ( (millis() - referenceTime) >= STABLETIMEOUT ) {
          buttonsState = ButtonsStates::WAITING_RELEASE;
        }
        break;
      }
    case ButtonsStates::WAITING_RELEASE: {
        if (digitalRead(lastButton) == HIGH) {
          buttonsState = ButtonsStates::WAITING_PRESS;
          evButtons = true;
          evButtonsData = lastButton;
        }

        break;
      }
    default:
      break;

  }
}

void taskSerial() {
  enum class SerialStates {INIT, WAITING_REQUESTS};
  static SerialStates serialState =  SerialStates::INIT;

  switch (serialState) {
    case SerialStates::INIT: {
        Serial.begin(9600);
        pinMode(LED2, OUTPUT);
        serialState = SerialStates::WAITING_REQUESTS;
        break;
      }
    case SerialStates::WAITING_REQUESTS: {


        // OUTPUT DEL ARDUINO
        if(evButtons == true){
          evButtons = false;
          
          if (evButtonsData == LEFT_BTN) {
            Serial.println("Se incrementa el tamaño del cubo A");//FAlta añadir en unity el cambio
          }
          else if (evButtonsData == RIGHT_BTN) {
            Serial.println("Se cambia el Color del cubo A");
          }
        }

        // INPUT DEL UNITY
        switch (Serial.read())
        {
          case 'A':
            Serial.println("Led 2 se prende");
            digitalWrite(LED2, HIGH);
            break;
          case 'Z':
            Serial.println("Led 2 se apaga");
            digitalWrite(LED2, LOW);
            break;
        }
        break;
      }
    default:
      break;
  }
}

void taskBeat() {
  enum class BeatStates {INIT, BEATING};
  static BeatStates beatlState =  BeatStates::INIT;
  static uint32_t previousMillis = 0;
  const uint32_t interval = 500;
  static bool ledState = false;

  switch (beatlState) {
    case BeatStates::INIT: {
        digitalWrite(LED1, ledState);
        pinMode(LED1, OUTPUT);
        beatlState = BeatStates::BEATING;
        break;
      }
    case BeatStates::BEATING: {
        if ( (millis() - previousMillis) >= interval) {
          previousMillis = millis();

          if (ledState == false) {
            ledState = true;
          } else {
            ledState = false;
          }
          digitalWrite(LED1, ledState);
        }

        break;
      }
    default:
      break;
  }
}
