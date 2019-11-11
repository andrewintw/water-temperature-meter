#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  11
#define OLED_CLK   12
#define OLED_DC     9
#define OLED_CS    10
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Comment out above, uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);
*/

void setup() {
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();
}

void loop() {
    drawText();
}

void drawText(void) {
    display.clearDisplay();

    display.setTextSize(1);                 // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);    // Draw white text
    display.setCursor(0, 0);                // Start at top-left corner
    display.println(F("00:10:10 23.50 C"));

    display.setCursor(0, 16);                // Start at top-left corner
    display.println(F("00:10:20 23.50 C"));

    display.setCursor(0, 32);                // Start at top-left corner
    display.println(F("ABCDEFGHIJklmnopqrstu"));

    display.setCursor(0, 48);                // Start at top-left corner
    display.println(F("123456789012345678901"));

    display.display();
    delay(2000);
}

