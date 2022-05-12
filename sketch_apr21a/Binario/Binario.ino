#define LED_WRONG 14

void taskComp();

void setup() {
  taskComp();
}
void loop() {
  taskComp();
}
void taskComp() {
  enum class StateTaskComp {INIT,  WAIT_2A, SEND, WAIT_ANSWER, REPORT_ERRORS};
  static StateTaskComp state =  StateTaskComp::INIT;
//  static uint8_t bufferRx[3] = {0};
    static uint8_t dataCounter = 0;
//  static uint8_t checkCounter = 0;
//  static uint8_t bufferTx[3];
  static uint8_t bufferRx[17] = {0};//bufferSendPack
  static uint8_t bufferTx[4] = {0};
  static uint8_t dataCounterFloats = 3;
  static uint8_t dataCounterNumber = 0;
  uint8_t incomeData;

  switch (state) {
    case StateTaskComp :: INIT: {
        Serial.begin(9600);
        pinMode(LED_WRONG, OUTPUT);

        state = StateTaskComp :: WAIT_2A;
        break;
      }
    case StateTaskComp :: WAIT_2A: {
        if (Serial.available()) {

          if (Serial.read() == 0x2A) {

            state = StateTaskComp :: SEND;
          }
        }
        break;
      }
    case StateTaskComp :: SEND: {
        int32_t ent = 0x01e07edc;
        float floatarr[3] = {25.34, 35.22, 79.42};
        static uint8_t checkSum;
        for (int8_t i = 0; i <= 11; i++) {
          if (i == 0) {
            memcpy(bufferTx, (uint8_t *)&floatarr[0], 4); //copia los bits en big endian
          }
          else if (i == 4) {
            memset(bufferTx, 0, 4); //borro los bits anteriores
            memcpy(bufferTx, (uint8_t *)&floatarr[1], 4); //copia los bits nuevos en big endian
          }
          else if (i == 8) {
            memset(bufferTx, 0, 4); //borro los bits anteriores
            memcpy(bufferTx, (uint8_t *)&floatarr[2], 4); //copia los bits nuevos en big endian
          }
          //se guardan los bits en el bufferSendPack en little endian
          bufferRx[i] = bufferTx[dataCounterFloats];
          dataCounterFloats--;
          if (dataCounterFloats == 0) {
            dataCounterFloats = 3;
          }
        }
        //actualizo el bufferXData con los bits del numero de 32 bits en little endian, al terminar deberian haber 16 bits en el bufferSendPack
        memset(bufferTx, 0, 4);
        bufferTx[0] = (uint8_t)(ent >> 24);
        bufferTx[1] = (uint8_t)(ent >> 16);
        bufferTx[2] = (uint8_t)(ent >> 8);
        bufferTx[3] = (uint8_t)(ent & 0x00FF);
        //paso los bits en little endian a el bufferSendPack
        for  (int8_t i = 12; i <= 15; i++) {
          bufferRx[i] = bufferTx[dataCounterNumber];
          dataCounterNumber++;
        }
        //calculo el checksum
        for (int8_t i = 0; i <= 15; i++) {
          checkSum = checkSum ^ bufferRx[i];
        }
        //lo paso a la ultima posicion del bufferSendPack
        bufferRx[16] = checkSum;
        //enviar todo el paquete desde el bufferSendPack en little endian
        for (int8_t i = 0; i <= 16; i++) {
          Serial.write(bufferRx[i]);
        }
        //borro todo para el siguiente paquete
        memset(bufferTx, 0, 4);
        memset(bufferRx, 0, 17);
        dataCounterFloats = 3;
        dataCounterNumber = 0;
        checkSum = 0;
        state = StateTaskComp :: WAIT_ANSWER;
        break;
        for (int j = 0; j < 3; j++) {
          Serial.write ( (uint8_t *) &floatarr[j], 4);
        }
        Serial.println (ent);
        Serial.write(checkSum);
        state = StateTaskComp :: WAIT_ANSWER;
        break;
      }
    case StateTaskComp :: WAIT_ANSWER: {

        if (Serial.available()) {
          uint8_t comeData = Serial.read();

          if (comeData == 0x3E) {
            state = StateTaskComp :: WAIT_2A;
          }
          if ((comeData == 0xB0) ) {

            dataCounter = dataCounter + 1 ;
            state = StateTaskComp :: SEND;

          }
          if (dataCounter == 3) {
            state = StateTaskComp :: REPORT_ERRORS;
          }
        }

        break;
      }
    case StateTaskComp :: REPORT_ERRORS: {
        dataCounter = 0;
        //        const uint32_t interval = 1000;
        //        const uint32_t interval2 = 3000;
        //        static uint32_t previousMillis = 0;
        //        static uint32_t previousMillis2 = 0;
        //        static uint8_t ledState = LOW;
        //        static uint8_t counterLed = 3;
        //
        //        uint32_t currentMillis = millis();
        //        uint32_t currentMillis2 = millis();
        //
        //
        //        if (currentMillis - previousMillis >= interval) {
        //          previousMillis = currentMillis;
        //          if (ledState == LOW) {
        //            ledState = HIGH;
        //          } else {
        //            ledState = LOW;
        //            counterLed--;
        //          }
        //          digitalWrite(LED_WRONG, ledState);
        //          if (counterLed == 0) {
        //            Serial.print("apago");
        //            state = StateTaskComp :: WAIT_2A;
        //          }
        //          //Serial.print("holaaaa");
        //        }
        //
        //
        //      }
        digitalWrite(LED_WRONG, HIGH);
        delay(500);
        digitalWrite(LED_WRONG, LOW);
        delay(500);
        digitalWrite(LED_WRONG, HIGH);
        delay(500);
        digitalWrite(LED_WRONG, LOW);
        delay(500);
        digitalWrite(LED_WRONG, HIGH);
        delay(500);
        digitalWrite(LED_WRONG, LOW);
        delay(500);
        state = StateTaskComp :: WAIT_2A;

        break;
      }
  }
}
