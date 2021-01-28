/**
 * LCDGraph.h - Arduino library for drawing graphs on alphanumeric LCDs using custom chars.
 * Jotham Gates
 * Created 27/11/2020
 * Last Modified 20/01/2021
 */

// TODO: Make this work with something other than a 8 by 1 graph.
// TODO: Make the data work with templates. - Everything put in the header file for this purpose
#ifndef LCDGraph_h
#define LCDGraph_h

#include "Arduino.h"
#include <LiquidCrystal.h>

#define LCDGRAPH_MAX_CHARS 8
#define LCDGRAPH_CHAR_WIDTH 5
#define LCDGRAPH_CHAR_HEIGHT 8 // Could be 10 on some LCDs
#define DataFormat int16_t
template <typename DataFormat>
class LCDGraph {
    public:
        /** Initialises the class.
         * Currently, the height must be 1 as vertical tiling is not yet implemented.
         * @param width is the width in characters.
         * @param height is the height in characters on the display, but should be 1 currently.
         * @param firstRegister is the first register to use in the display. As there are only 8, they may need to be shared around.
         * For example, using 2 4 character wide graphs would be:
         * LCDGraph graph1(4, 1, 0);
         * LCDGraph graph2(4, 1, 4);
         * An 8 char wide graph must start at 0, a 7 char wide at 0 or 1, ...
         */
        LCDGraph(uint8_t width, uint8_t height, uint8_t firstRegister) {
            _width = width;
            _height = height;
            _firstRegister = firstRegister;
            _data = (DataFormat *)malloc(width * LCDGRAPH_CHAR_WIDTH * sizeof(DataFormat));
        }

        /** Sets the pointer to the lcd object */
        void begin(LiquidCrystal *lcd) {
            _lcd = lcd;
        }

        /** Adds a data point to the graph circular buffer.
         * Chucks out the earlist data point if the buffer is full.
         */
        void add(DataFormat value) {
            // Calculate the position for the new data point
            uint8_t position = _start + length;
            uint8_t dataSize = _width * LCDGRAPH_CHAR_WIDTH;
            if(position >= dataSize) {
                position -= dataSize;
            }

            // Add the new data point
            _data[position] = value;
            // Either increase the length if not full or update the address of the first element to be the next as we will have just overwritten the first element if the buffer is at capacity.
            if(length < dataSize) {
                length++;
            } else {
                _start++;
                if(_start == dataSize) { // Wrap around if needed
                    _start = 0;
                }
            }
        }

        /** Removes all data from the circular buffer */
        void clear() {
            length = 0;
        }

        /** Generates the custom characters from the data currently in the circular buffer and sends it to the custom character registers in the lcd.
         * If any of the special characters or this graph are already on the display, they will be updated.
         * Otherwise, display(uint8_t x, uint8_t y); needs to be called to draw the graph from the registers.
         */
        void setRegisters() {
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

        /** Displays the graph in the correct location on the display.
         * setRegisters needs to be called beforehand to display the latest data.
         * In all displays I have used, display() only needs to be called once if the graph is going to be continually be updated in the same location
         */
        void display(uint8_t x, uint8_t y) {
            _lcd->setCursor(x, y);
            for(uint8_t i = _firstRegister; i < _width + _firstRegister; i++) {
                _lcd->write(uint8_t(i));
            }
        }


        /** Rescales the graph to fit all data. If force0 is set to true, will make
         * sure that 0 is included as either the minimum or maximum.
         * 0 will also be included as a limit if there is only a single value.
         */
        // TODO: Padding instead of including 0
        void autoRescale(bool force0) {
            // Find the minimum and maximum
            DataFormat value = _atPosition(0);
            yMin = value;
            yMax = value;
            for(uint8_t i = 1; i < length; i++) {
                DataFormat value = _atPosition(i);
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

        /** Deallocates the buffer from memory.
         * It is preferable to only create the instance and use it (with clear() if necessary) for the entire time the program is running.
         */
        void end() {
            free(_data);
        }

        uint8_t length = 0; // Number of data points being shown
        DataFormat yMin = 0;
        DataFormat yMax = 255;
        bool filled = true; // TODO: Make filled to the x axis rather than bottom of screen.
        bool axis = true; // Show axis

    private:
        /** Translates the position in order into the physical address and returns the value at the address. */
        DataFormat _atPosition(uint8_t position) {
            position += _start;
            uint8_t dataSize = _width * LCDGRAPH_CHAR_WIDTH;
            if(position >= dataSize) {
                position -= dataSize;
            }

            return _data[position];
        }

        LiquidCrystal *_lcd;
        DataFormat *_data;
        uint8_t _start = 0;
        uint8_t _width = 8;
        uint8_t _height = 1;

        uint8_t _firstRegister = 0;

};

#endif