/** Linear.ino
 * An example for the LCDGraph library.
 * Displays a line on the display that should increase and start from 0.
 * Written by Jotham Gates
 * Created 20/01/2021
 * Last modiified 20/01/2021
 */

// Libraries to include
#include <LiquidCrystal.h>
#include <LCDGraph.h>

// Constructors
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LCDGraph<uint8_t> graph(8, 1, 0);

void setup() {
    // Set up the lcd
    lcd.begin(20, 4);
    graph.begin(&lcd);

    // Welcome messages
    Serial.begin(38400);
    Serial.println(F("LCD Graph demonstration"));
    lcd.print(F("Linear Line"));
    // Draw the graph:
    // Bounds are from 0 to 255 by default, so do not need to change
    graph.filled = false;
    graph.setRegisters();
    graph.display(0,1);
}

void loop() {
    static uint8_t counter = 0;
    if(counter == 0) { // Toggle fill every so often
        graph.filled = !graph.filled;
    }
    graph.add(counter);
    graph.setRegisters();
    counter += 10; // Because counter is a byte, it will overflow. Every so often it will land on 0
                   // and toggle filling in of the graph.
    delay(50);
}