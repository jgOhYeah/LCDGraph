/** Sine.ino
 * An example for the LCDGraph library.
 * Displays a sine wave on the display.
 * Written by Jotham Gates
 * Created 20/01/2021
 * Last modiified 20/01/2021
 */

// Libraries to include
#include <LiquidCrystal.h>
#include <LCDGraph.h>

// Constructors
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LCDGraph<float> graph(8, 0); // We want to store floats in this case

void setup() {
    // Set up the lcd
    lcd.begin(20, 4);
    graph.begin(&lcd);

    // Welcome messages
    Serial.begin(38400);
    Serial.println(F("LCD Graph demonstration"));
    lcd.print(F("Sine Wave"));

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