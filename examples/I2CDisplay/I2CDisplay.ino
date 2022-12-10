/** I2CDisplay.ino
 * An example for the LCDGraph library.
 * Displays a sine wave on the display using the LiquidCrystal_I2C library found
 * here: https://github.com/johnrickman/LiquidCrystal_I2C.
 * Written by Jotham Gates
 * Created 10/12/2022
 * Last modiified 10/12/2022
 */

// Libraries to include
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <LCDGraph.h>

// Constructors
#define LCD_ADDRESS 0x27
#define LCD_ROWS 2
#define LCD_COLS 16
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
LCDGraph<float, LiquidCrystal_I2C> graph(8, 0); // We want to store floats in this case

void setup() {
    // Set up the lcd
    lcd.init();
    lcd.backlight();
    graph.begin(&lcd);

    // Welcome messages
    Serial.begin(38400);
    Serial.println(F("LCD Graph I2C demonstration"));
    lcd.print(F("I2C Sine Wave"));

    // Draw the graph:
    graph.yMin = -800;
    graph.yMax = 800;
    graph.filled = false;
    graph.setRegisters();
    graph.display(0,1);
}

void loop() {
    // Add a new point
    static float counter = 0;
    float result = 800 * sin(counter);
    graph.add(result);
    graph.setRegisters();
    counter += 0.2;
    delay(50);
}