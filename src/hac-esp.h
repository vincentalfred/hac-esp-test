#include <SPI.h>
#include <Wire.h>

const int machine_id = 1;
const char* machine_name = "machine1";
const bool DEBUG = true;
const bool LCDDEBUG = true;
bool activate = 0; // value from cloud to ack SSR pin D8
unsigned long currentMillis, startMillis, minuteMillis, interval = 1800000, previousMillis;
unsigned long lcdMillis;
bool lcdBlink = 0;
int lcdBacklight = 255;
float startEnergy;


#define MFRC522_SS_PIN  D4
#define MFRC522_RST_PIN D3
#define SSR_PIN         D8
#define BUTTON_1        D0

void printDebug(String debugMessage);



#include "hac-lcd.h"
#include "hac-pzem004t.h"
#include "hac-mqtt.h"
#include "hac-rfid.h"


void printDebug(String debugMessage) {
    if (!DEBUG) return;
    if (LCDDEBUG) lcdPrint(debugMessage);
    else Serial.println(debugMessage);
}
