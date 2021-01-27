# LCDGraph
An Arduino library for drawing line graphs on alphanumeric displays using custom characters

This library uses custom characters to draw simple line graphs on Hitachi HD44780 based alphanumeric displays using the built in [LiquidCrystal](https://www.arduino.cc/en/Reference/LiquidCrystal) library.

## API
For examples, see the examples folder.
### Including and Constructor
#### Includes
Both the LiquidCystal and LCDGraph libraries need to be included as follows:
```c++
#include <LiquidCrystal.h>
#include <LCDGraph.h>
```

#### Constructor
```c++
LCDGraph<DataFormat> graph(uint8_t width, uint8_t height, uint8_t firstRegister);
```
Initialises the class.
- `DataFormat` is the data type that data will be stored in the instance's buffer. For example, `uint8_t` for a single unsinged byte or `int` for a signed, 16 bit integer.
- `width` is the width in characters.
- `height` is the height in characters on the display, but should be 1 currently as vertical tiling is not yet implemented.
- `firstRegister` is the first custom character register to use in the display. As there are only 8, they may need to be shared around.
  - For example, using 2 4 byte wide graphs would be:
    ```c++
    LCDGraph graph1(4, 1, 0);
    LCDGraph graph2(4, 1, 4);
    ```
  - An 8 char wide graph must start at 0, a 7 char wide at 0 or 1, ...

### Setup
#### Begin
```c++
void begin(LiquidCrystal *lcd)
```
Sets up the pointers to the LiquidCrystal object.
For example:
```c++
lcd.begin(20, 4);
graph.begin(&lcd);
```

### Setting the Y axis limits
The attributes `yMin` and `yMax` can be used to set the minimum and maximum values of the Y axis next time the graph is redrawn using `setRegisters`. They are of the same data type as specified in the constructor.

`yMin` defaults to 0 and `yMax` defaults to 255.

For example:
```
graph.yMin = 0;
graph.yMax = 63;
```
Alternatively, `autoRescale` can be used.

#### Automatic scaling of the Y axis
<!-- TODO: -->