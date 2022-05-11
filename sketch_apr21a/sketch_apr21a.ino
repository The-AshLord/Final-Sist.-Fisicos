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

void setup()
{
  Serial.begin(115200);
  taskButtons();
  taskSerial();

}

void loop()
{
  taskButtons();
  taskSeial();
  // Print a heartbeat
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
          LOG(String( String(buttonGetName(lastButton)) ));
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
        Serial.begin(115200)
        SerialStates = SerialStates::WAITING_REQUESTS;
        break;
      }
    case SerialStates::WAITING_REQUESTS: {
        // La respuesta se debe enviar siguiendo este protocolo:
        // Enviar la cadena
        // estado_UP,estado_DOWN,estado_LEFT,estado_RIGHT
        // No olvidar enviar el ENTER: \n

        // OUTPUT DEL ARDUINO
        if (Serial.available() > 0) {
          if (evButtons == true) {
            evButtons = false;
            Serial.print(digitalRead(LEFT_BTN));
            Serial.print(',');
            Serial.print(digitalRead(RIGHT_BTN));
            Serial.print('\n');
            if (lastButton == LEFT_BTN) {
              Serial.println("Se incrementa el tamaño del cubo A");
            }
            else if (lastButton == RIGHT_BTN) {
              Serial.println("Se cambia el Color del cubo A");
            }
          }

          // INPUT DEL UNITY
          switch (Serial.read())
          {
            case 'A':
              Serial.println("Led se prende"); // falta activar los leds y entrar los botones 
              break;
            case 'Z':
              Serial.println("Led se apaga");
              break;
          }

        }
      }
      break;
  }
default:
  break;
}
}
