# LCDGraph API <!-- omit in toc -->
## Table of Contents <!-- omit in toc -->
- [Including](#including)
- [Constructor](#constructor)
- [Attributes](#attributes)
- [Methods](#methods)
  - [`void begin(LCDLibrary *lcd)`](#void-beginlcdlibrary-lcd)
  - [`void add(DataFormat value)`](#void-adddataformat-value)
  - [`void clear()`](#void-clear)
  - [`void setRegisters()`](#void-setregisters)
  - [`void display(uint8_t x, uint8_t y)`](#void-displayuint8_t-x-uint8_t-y)
  - [`void autoRescale(bool force0 = false, bool allowSmallerRange = true)`](#void-autorescalebool-force0--false-bool-allowsmallerrange--true)
  - [`uint8_t length()`](#uint8_t-length)
  - [`void end()`](#void-end)

# Including
Add these lines to the top of your main project file:
```c++
// Libraries to include
#include <LiquidCrystal.h>
#include <LCDGraph.h>
```


# Constructor
```c++
LCDGraph<DataFormat=int16_t, LCDLibrary=LiquidCrystal>(uint8_t width, uint8_t height, uint8_t firstRegister)
```
Initialises the class.
Currently, the height must be 1 as vertical tiling is not yet implemented.
- `DataFormat` is a template and is the data format that will be expected when calling `add` and setting and reading `yMax` and `yMin`. This can be changed to a smaller data type such as a `byte` to save memory if needed or a larger one such as an `unsigned long` or `float` if larger or floating point numbers need to be displayed. Because each data type will require its own copy of the class, it is recommended that if possible, only a single data type is used accross all instances to save program memory.
- `LCDLibrary` is a template for the library used to drive the display. By default it is set to the built in `LiquidCrystal` library, but has also been tested to work with the [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) library.
- `width` is the width in characters.
- `height` is the height in characters on the display, but should be 1 currently.
- `firstRegister` is the first register to use in the display. As there are only 8, they may need to be shared around.

For example, using 2 4 character wide graphs would be:
```c++
LCDGraph<int> graph1(4, 1, 0);
LCDGraph<int> graph2(4, 1, 4);
```

An 8 char wide graph must start at register 0 in the display, a 7 char wide at 0 or 1, ...

The `height` parameter can be left out if desired to default to a height of 1:
```c++
LCDGraph<DataFormat=int16_t, LCDLibrary=LiquidCrystal>(uint8_t width, uint8_t firstRegister)
```

# Attributes
| Name         | Data Type                               | Comments                                                                                                                                                                                                            | Default |
| ------------ | --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :-----: |
| `yMin`       | `DataFormat` (specified in constructor) | The current minimum of the Y axis. Can be written to to set it manually                                                                                                                                             |    0    |
| `yMax`       | `DataFormat` (specified in constructor) | The current maximum of the Y axis. Can be written to to set it manually                                                                                                                                             |   255   |
| `filled`     | `bool`                                  | Whether everything under the line will be filled in                                                                                                                                                                 | `true`  |
| `showXAxis`  | `bool`                                  | Whether to draw the X axis as a solid line.                                                                                                                                                                         | `true`  |
| `showYAxis`  | `bool`                                  | Whether to draw the Y axis as a solid line.                                                                                                                                                                         | `true`  |
| `intercepts` | `bool`                                  | If `true`, makes any x and y axis intercepts display as an off pixel when axis are displayed so that points on axis do not disappear. Can be a bit misleading if the x axis is on the top or bottom of the display. | `false` |


# Methods
## `void begin(LCDLibrary *lcd)`
Gives the library the pointer to the lcd object to use.
For example:
```c++
// Set up the lcd
lcd.begin(20, 4);
graph.begin(&lcd);
```

## `void add(DataFormat value)`
Adds a data point to the internal graph circular buffer.
Chucks out the earlist data point if the buffer is full.

## `void clear()`
Removes all data from the circular buffer

## `void setRegisters()`
Generates the custom characters from the data currently in the circular buffer and sends it to the custom character registers in the lcd. If any of the special characters or this graph are already on the display, they will be updated. Otherwise, `display(uint8_t x, uint8_t y);` needs to be called to draw the graph from the registers.

## `void display(uint8_t x, uint8_t y)`
Displays the graph in the correct location on the display. setRegisters needs to be called beforehand to display the latest data. In all displays I have used, `display` only needs to be called once if the graph is going to be continually be updated in the same location.

## `void autoRescale(bool force0 = false, bool allowSmallerRange = true)`
Rescales the graph to fit all data.
- `force0` will make sure that 0 is included as either the minimum or maximum if true. 0 will also be included as a limit if there is only a single value.
- `allowSmallerRange` will allow `yMin` to increase and `yMax` to decrease to fit all current data. If false, the range can only expand (`yMin` decrease and `yMax` increase).

## `uint8_t length()`
Returns the number of points in the circular buffer.

## `void end()`
Deallocates the internal circular buffer from memory.
It is preferable to only create the instance and use it as global (with `clear()` if necessary) for the entire time the program is running to avoid memory fragmentation issues.