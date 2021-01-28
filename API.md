# LCDGraph API
## Table of Contents
- [LCDGraph API](#lcdgraph-api)
  - [Table of Contents](#table-of-contents)
- [Including](#including)
- [Constructor](#constructor)
- [Attributes](#attributes)
- [Methods](#methods)
  - [`begin(LiquidCrystal *lcd)`](#beginliquidcrystal-lcd)
  - [`add(DataFormat value)`](#adddataformat-value)
  - [`clear()`](#clear)
  - [`setRegisters()`](#setregisters)
  - [`display(uint8_t x, uint8_t y)`](#displayuint8_t-x-uint8_t-y)
  - [`autoRescale(bool force0)`](#autorescalebool-force0)
  - [`end()`](#end)

# Including
Add these lines to the top of your main project file:
```c++
// Libraries to include
#include <LiquidCrystal.h>
#include <LCDGraph.h>
```


# Constructor
```c++
LCDGraph<DataFormat>(uint8_t width, uint8_t height, uint8_t firstRegister)
```
Initialises the class.
Currently, the height must be 1 as vertical tiling is not yet implemented.
- `DataFormat` is a template and is the data format that will be expected when calling `add` and setting and reading `yMax` and `yMin`. This can be changed to a smaller data type such as a `byte` to save memory if needed or a larger one such as an `unsigned long` or `float` if larger or floating point numbers need to be displayed. Because each data type will require its own copy of the class, it is recommended that if possible, only a single data type is used accross all instances to save program memory.
- `width` is the width in characters.
- `height` is the height in characters on the display, but should be 1 currently.
- `firstRegister` is the first register to use in the display. As there are only 8, they may need to be shared around.

For example, using 2 4 character wide graphs would be:
```c++
LCDGraph<int> graph1(4, 1, 0);
LCDGraph<int> graph2(4, 1, 4);
```

An 8 char wide graph must start at register 0 in the display, a 7 char wide at 0 or 1, ...

# Attributes
| Name     | Data Type                               | Comments                                                                | Should be written to | Default |
| -------- | --------------------------------------- | ----------------------------------------------------------------------- | :------------------: | :-----: |
| `length` | `uint8_t` (`byte`)                      | The number of points in the circular buffer                             |          No          |    0    |
| `yMin`   | `DataFormat` (specified in constructor) | The current minimum of the Y axis. Can be written to to set it manually |         Yes          |    0    |
| `yMax`   | `DataFormat` (specified in constructor) | The current maximum of the Y axis. Can be written to to set it manually |         Yes          |   255   |
| `filled` | `bool`                                  | Whether everything under the line will be filled in                     |         Yes          | `true`  |
| `axis`   | `bool`                                  | Whether to draw X and Y axis as solid lines.                            |         Yes          | `true`  |


# Methods
## `begin(LiquidCrystal *lcd)`
Gives the library the pointer to the lcd object to use.
For example:
```c++
// Set up the lcd
lcd.begin(20, 4);
graph.begin(&lcd);
```

## `add(DataFormat value)`
Adds a data point to the internal graph circular buffer.
Chucks out the earlist data point if the buffer is full.

## `clear()`
Removes all data from the circular buffer

## `setRegisters()`
Generates the custom characters from the data currently in the circular buffer and sends it to the custom character registers in the lcd. If any of the special characters or this graph are already on the display, they will be updated. Otherwise, `display(uint8_t x, uint8_t y);` needs to be called to draw the graph from the registers.

## `display(uint8_t x, uint8_t y)`
Displays the graph in the correct location on the display. setRegisters needs to be called beforehand to display the latest data. In all displays I have used, `display` only needs to be called once if the graph is going to be continually be updated in the same location.

## `autoRescale(bool force0)`
Rescales the y axis of the graph to fit all data. If `force0` is set to `true`, will make sure that 0 is included as either the minimum or maximum. 0 will also be included as a limit if there is only a single value.

## `end()`
Deallocates the internal circular buffer from memory.
It is preferable to only create the instance and use it as global (with `clear()` if necessary) for the entire time the program is running to avoid memory fragmentation.