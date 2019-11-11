#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ONE_WIRE_BUS    2
#define SCREEN_WIDTH  128   // OLED display width, in pixels
#define SCREEN_HEIGHT  64   // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI      11
#define OLED_CLK       12
#define OLED_DC         9
#define OLED_CS        10
#define OLED_RESET      8

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

#define UPDATE_INTERVAL     1000
unsigned long time_current  = 0;
unsigned long time_reset    = 0;

#define RESET_BUTTON        4

#define BUZZER              3
#define TARGET_TEMPC        60
int enable_buzzer           = 0;


void setup() {
    Serial.begin(9600);
    tempSensor.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC)) { /* SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally */
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

    display.display();  /* show initial display buffer: Adafruit splash screen */
    delay(1000);
    display.clearDisplay(); /* clear buffer */

    pinMode(RESET_BUTTON, INPUT_PULLUP);
    pinMode(BUZZER, OUTPUT);
    
    tone(BUZZER, 1000, 500);
    delay(100);
    noTone(BUZZER);
}

void loop() {
    if (millis() - time_current > UPDATE_INTERVAL) {
        time_current = millis();

        if (digitalRead(RESET_BUTTON) == LOW) { /* pressed reset button */
            time_reset = millis();
        }
        unsigned long time_sec = (time_current - time_reset) / 1000;
        String time_str = getTimeStamp(time_sec);

        tempSensor.requestTemperatures();
        float tempC = tempSensor.getTempCByIndex(0);

        printSerialText(time_str, time_sec, tempC);
        drawOLEDText(time_str, tempC);

        if (tempC >= (TARGET_TEMPC + 1)) {
            enable_buzzer = 1;
        }

        if (enable_buzzer == 1 && tempC <= TARGET_TEMPC) {
            alarmBuzzer();
            enable_buzzer = 0;
        }
    }
}

void printSerialText(String time_str, unsigned long time_sec, float temp_value) {
    Serial.print(time_str);
    Serial.print(", ");
    Serial.print(time_sec);
    Serial.print(", ");
    Serial.println(temp_value);
}

String getTimeStamp(unsigned long time_ms) {
    String t_str = "";

    int t_sec = (time_ms % 60);
    int t_min = (time_ms / 60) % 60;
    int t_hor = (time_ms / 3600) % 24;

    if (t_hor < 10) {
        t_str += "0";
    }
    t_str += (String(t_hor, DEC) + ":");

    if (t_min < 10) {
        t_str += "0";
    }
    t_str += (String(t_min, DEC) + ":");

    if (t_sec < 10) {
        t_str += "0";
    }
    t_str += String(t_sec, DEC);

    return t_str;
}

void drawOLEDText(String time_str, float celsius) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.println(time_str);
    display.setCursor(0, 20);
    display.print(celsius);
    display.print(char(247));
    display.println(F(" C"));

    display.display();
}

void alarmBuzzer() {
    tone(BUZZER, 1000, 500);
    delay(100);
    noTone(BUZZER);
    delay(100);
    tone(BUZZER, 1000, 500);
    delay(100);
    noTone(BUZZER);
}

