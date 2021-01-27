/** Grayscale.ino
 * An example for the LCDGraph library.
 * Displays a line and attemps to flash between filled and not filled very fast
 * Written by Jotham Gates
 * Created 24/01/2021
 * Last modiified 24/01/2021
 */

// Libraries to include
#include <LiquidCrystal.h>
#include <LCDGraph.h>

// Constructors
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LCDGraph<uint8_t> graph(8, 1, 0);

const uint8_t onTicks = 20;
const uint8_t offTicks = 1;
void setup() {
    // Set up the lcd
    lcd.begin(20, 4);
    graph.begin(&lcd);

    // Welcome messages
    Serial.begin(38400);
    Serial.println(F("LCD Graph demonstration"));
    lcd.print(F("Grayscale"));
    // Draw the graph:
    // Bounds are from 0 to 255 by default, so do not need to change

    
    // Draw the line
    graph.yMax = 39;
    for(uint8_t i = 0; i < 40; i++) {
        graph.add(i);
    }
    graph.setRegisters();
    graph.display(0,1);
}

void loop() {
    static uint8_t counter = 0;
    counter++;
    if(graph.filled && counter == onTicks) {
        graph.filled = false;
        counter = 0;
        graph.setRegisters();
    } else if(!graph.filled && counter == offTicks) {
        graph.filled = true;
        counter = 0;
        graph.setRegisters();
    }
}