/** Multiple.ino
 * An example for the LCDGraph library.
 * Displays multiple graphs on the display.
 * Written by Jotham Gates
 * Created 20/01/2021
 * Last modiified 20/01/2021
 */

// Libraries to include
#include <LiquidCrystal.h>
#include <LCDGraph.h>

// Constructors
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LCDGraph<float> graphSine(4, 1, 0);
LCDGraph<float> graphLine(4, 1, 4); // Even though this only needs to be uint8_t, setting this would
                                    // require an entirely separate copy of the library to be
                                    // included, so in this case it is more efficient to use floats
                                    // and accept the higher ram use.

void setup() {
    // Set up the lcd
    lcd.begin(20, 4);
    graphSine.begin(&lcd);
    graphLine.begin(&lcd);

    // Welcome messages
    Serial.begin(38400);
    Serial.println(F("LCD Graph demonstration"));

    // Draw the titles on the display
    lcd.print(F("Sine | Line"));
    lcd.setCursor(5, 1);
    lcd.write('|');

    // Draw the sine graph:
    graphSine.yMin = -800;
    graphSine.yMax = 800;
    graphSine.filled = false;
    graphSine.setRegisters();
    graphSine.display(0, 1);

    // Draw the line graph:
    // Bounds are from 0 to 255 by default, so do not need to change
    graphLine.setRegisters();
    graphLine.display(7, 1);
}

void loop() {
    // Update the sine graph
    static float counter = 0;
    float result = 800 * sin(counter);
    graphSine.add(result);
    graphSine.setRegisters();
    counter += 0.2;

    // Update the line graph
    static uint8_t lineCounter = 0;
    if(lineCounter == 0) { // Toggle fill every so often
        graphLine.filled = !graphLine.filled;
    }
    graphLine.add(lineCounter);
    graphLine.setRegisters();
    lineCounter += 10;

    delay(50);
}