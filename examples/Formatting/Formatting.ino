/** Formatting.ino
 * An example for the LCDGraph library.
 * Displays a sine wave on the display.
 * Written by Jotham Gates
 * Created 20/01/2021
 * Last modiified 07/02/2021
 */

// Libraries to include
#include <LiquidCrystal.h>
#include <LCDGraph.h>

// Constructors
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LCDGraph<float> graph(8, 1, 0); // We want to store floats in this case

void setup() {
    // Set up the lcd
    lcd.begin(20, 4);
    graph.begin(&lcd);

    // Welcome messages
    Serial.begin(38400);
    Serial.println(F("LCD Graph demonstration"));
    lcd.print(F("Sine + Format"));

    // Draw the graph:
    graph.yMin = -800;
    graph.yMax = 800;
    graph.filled = false;
    // Add some static data in the form of a sine wave.
    float counter = 0;
    for(uint8_t i = 0; i < 40; i++) {
        float result = 800 * sin(counter);
        graph.add(result);
        counter += 0.2;
    }
    // Send the graph to the display
    graph.setRegisters();
    graph.display(0,1);
}

void loop() {
    // Give the viewer some time to appreciate the true masterpiece.
    delay(5000);

    // Change the format every so often
    static uint8_t format = 0; // Keep track of where we are up to in the demo.
    lcd.setCursor(0,0); // Get ready to write a short description
    switch(format) {
        case 0:
            // Make filled
            pad(lcd.print(F("Filled")));
            graph.filled = true;
            break;
        case 1:
            // Make not filled
            pad(lcd.print(F("Unfilled")));
            graph.filled = false;
            break;
        case 2:
            // Hide X axis
            pad(lcd.print(F("No X Axis")));
            graph.showXAxis = false;
            break;
        case 3:
            // Hide Y axis
            pad(lcd.print(F("No Y Axis")));
            graph.showXAxis = true; // Reset from last format
            graph.showYAxis = false;
            break;
        case 4:
            // Hide all axis
            pad(lcd.print(F("No Axis")));
            graph.showXAxis = false;
            break;
        case 5:
            // Show axis with intercepts
            pad(lcd.print(F("Intercepts")));
            graph.showXAxis = true;
            graph.showYAxis = true;
            graph.intercepts = true;
            break;
        case 6:
            // Show axis without intercepts
            pad(lcd.print(F("No Intercepts")));
            graph.intercepts = false;
            break;
        case 7:
            // Manual scale
            pad(lcd.print(F("Manual Scaling")));
            graph.yMin = 0;
            graph.yMax = 1600;
            break;
        case 8:
            // Auto rescale with allowSmallerRange = false
            pad(lcd.print(F("Rescale !smaller")));
            graph.autoRescale(false, false);
            break;
        case 9:
            // Auto rescale left to its own devices
            pad(lcd.print(F("Rescale smaller")));
            graph.autoRescale();
            break;
        case 10:
            // Reset to restart the demo
            pad(lcd.print(F("LCDGraph")));
            graph.yMin = -800;
            graph.yMax = 800;
            graph.filled = false;
    }
    // Move to next format
    format++;
    if(format == 11) {
        format = 0;
    }

    // Show the changes
    graph.setRegisters();
}

/** Writes spaces to the lcd to make sure anything already on there is completely written over */
void pad(uint8_t alreadyWritten) {
    for(uint8_t i = alreadyWritten; i < 16; i++) {
        lcd.write(' ');
    }
}