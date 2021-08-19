/**
 * LCDGraph.cpp - Arduino library for drawing graphs on alphanumeric LCDs using custom chars.
 * Jotham Gates, 27/11/2020
 */

#include "Arduino.h"
#include "LCDGraph.h"
#include <LiquidCrystal.h>

LCDGraph::LCDGraph(uint8_t width, uint8_t height, uint8_t firstRegister) {
    _width = width;
    _height = height;
    _firstRegister = firstRegister;
}

void LCDGraph::begin(LiquidCrystal *lcd) {
    _lcd = lcd;
}

/** Adds a data point to the graph circular buffer.
 * Chucks out the earlist data point if the buffer is full.
 */
void LCDGraph::add(LCDGRAPH_DATA_TYPE value) {
    // Calculate the position for the new data point
    uint8_t position = _start + length;
    if(position >= LCDGRAPH_MAX_DATA) {
        position -= LCDGRAPH_MAX_DATA;
    }

    // Add the new data point
    _data[position] = value;
    
    // Either increase the length if not full or update the address of the first element to be the next as we will have just overwritten the first element if the buffer is at capacity.
    if(length < LCDGRAPH_MAX_DATA) {
        length++;
    } else {
        _start++;
        if(_start == LCDGRAPH_MAX_DATA) { // Wrap around if needed
            _start = 0;
        }
    }
}
void LCDGraph::clear() {
    length = 0;
}

void LCDGraph::setRegisters() {
    uint8_t accessed = 0;
    // For each character to draw
    for(uint8_t block = 0; block < _width; block++) {
        // Get the data from the stored array and map it to the available pixel height.
        uint8_t mappedData[LCDGRAPH_CHAR_WIDTH];
        
        // Clear mappedData:
        for(uint8_t i = 0; i < LCDGRAPH_CHAR_WIDTH; i++) {
            mappedData[i] = 0;
        }

        uint8_t printMask = 0x00; // If a bit is 0, do not print anything in its column.
        uint8_t startPos = block * LCDGRAPH_CHAR_WIDTH;

        // Load data if available and map it to fit
        for(uint8_t i = 0; i < LCDGRAPH_CHAR_WIDTH && accessed < length; i++, accessed++) {
            mappedData[i] = map(_atPosition(i + startPos), yMax, yMin, 0, LCDGRAPH_CHAR_HEIGHT-1);
            printMask |= 1 << i; // Set the value as printable
        }

        // Generate the custom character for that block
        uint8_t xAxis = map(0, yMax, yMin, 0, LCDGRAPH_CHAR_HEIGHT-1);

        uint8_t character[LCDGRAPH_CHAR_HEIGHT];
        for(uint8_t row = 0; row < LCDGRAPH_CHAR_HEIGHT; row++) {
            character[row] = 0; // Clear the row in case there is something left in memory
            // Draw the x axis if needed
            if(axis && (row == xAxis)) {
                character[row] = 0xff; // Use exclusive or so that any point on the x axis is now light to stand out.
            }
            // For each column, check if a point should be drawn in the row.
            for(uint8_t col = 0; col < LCDGRAPH_CHAR_WIDTH; col++) {
                // Draw the y axis if needed
                if(axis && !col && !block) {
                    character[row] |= 1 << (LCDGRAPH_CHAR_WIDTH -1);
                }

                // Draw the point on the graph or point under it if shading
                if((printMask >> col) & 1) { // If there should be a point drawn on this column
                    if(mappedData[col] == row) { // An actual point
                        character[row] ^= 1 << (LCDGRAPH_CHAR_WIDTH -1 -col); // XOR so that any point on an axis appears white.
                    } else if(filled && mappedData[col] < row) { // The shaded area below
                        character[row] |= 1 << (LCDGRAPH_CHAR_WIDTH -1 -col); // Normal OR so the axis is not inverted
                    }
                }
            }
        }

        _lcd->createChar(_firstRegister + block, character); // Send the character to the display.
    }
}

/* Draws the custom characters to the display */
void LCDGraph::display(uint8_t x, uint8_t y) {
    _lcd->setCursor(x, y);
    for(uint8_t i = _firstRegister; i < _width + _firstRegister; i++) {
        _lcd->write(uint8_t(i));
    }
}

/** Rescales the graph to fit all data. If force0 is set to true, will make
 * sure that 0 is included as either the minimum or maximum.
 */
void LCDGraph::autoRescale(bool force0) {
    // Find the minimum and maximum
    LCDGRAPH_DATA_TYPE value = _atPosition(0);
    yMin = value;
    yMax = value;
    for(uint8_t i = 1; i < length; i++) {
        LCDGRAPH_DATA_TYPE value = _atPosition(i);
        if(value < yMin) {
            yMin = value;
        }
        if(value > yMax) {
            yMax = value;
        }
    }

    // Make sure the x axis is displayed if required.
    if(force0 || yMin == yMax) { // yMin == yMax is for if there is only 1 point, include a 0 as another point.
        if(yMin > 0) {
            yMin = 0;
        }
        if(yMax < 0) {
            yMax = 0;
        }
    }
}

/* Translates the position in order into the physical address and returns the value at the address. */
inline LCDGRAPH_DATA_TYPE LCDGraph::_atPosition(uint8_t position) {
    position += _start;
    if(position >= LCDGRAPH_MAX_DATA) {
        position -= LCDGRAPH_MAX_DATA;
    }

    return _data[position];
}