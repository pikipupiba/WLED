# FastLED Compatibility UserFX Wrapper

The FastLED Compatibility UserFX Wrapper (FL_Wrapper) allows for easier porting of FastLED effects to WLED.

## Functionality

FastLED has some great features that are taken for granted when writing FastLED patterns, but can't be used directly when writing WLED effects.  FL_Wrapper brings these features into WLED to make writing effects and porting FastLED effects into WLED much easier.

### Persistent Non-Lossy CRGB Storage

FastLED sketches usually store pixel data in `CRGB leds[]`, and define `NUM_LEDS` at the top of the sketch.  `leds[]` is also persistent between passes through `loop()`, so effects can be made by the data between previous frames.  This won't work with WLED, as `NUM_LEDS` isn't a constant, it's determined by the settings of the current segment.  Likewise, a fixed `leds[]` array wouldn't work as there are multiple segments that would need to access it.  Also, WLED uses `setPixelColor()` to store RGB data instead of directly accessing an array, and that data can't be directly retrieved like you would from an array, as `getPixelColor()` returns lossy data.

To make it easier to port FastLED sketches, FL_Wrapper manages an array that can be used for persistent storage of CRGB data, that can be accessed using `leds[]` in your effect, and sets a local constant `NUM_LEDS`.  For this basic functionality, use `BEGIN_FASTLED_COMPATIBILITY()` at the top of the effect.  At the end of the effect, use `END_FASTLED_COMPATIBILITY()`, which takes care of writing the `leds[]` array to WLED's buffers using `setPixelColor()`.  The more complex macros describe later also include this feature.

### 2D Functionality ("Virtual Screen")

Some FastLED sketches are made for rectangular matrices, where the width/height is set to `kMatrixWidth`/`kMatrixHeight`, `NUM_LEDS` is `kMatrixWidth*kMatrixHeight`, and an `XY()` function is defined that can be used in the sketch to set the LED matching an X/Y coordinate in the matrix: `leds[XY(x,y)]`.

The `BEGIN_FASTLED_XY_COMPATIBILITY()` macro sets up all the FastLED compatibility as `BEGIN_FASTLED_COMPATIBILITY()` does, and also creates these variables:

```
kMatrixWidth = _segmentmaps[_segment_index].matrixWidth;
kMatrixHeight = _segmentmaps[_segment_index].matrixHeight;
int NUM_LEDS (kMatrixWidth*kMatrixHeight);
```

The `XY()` function is created and depends on the above `kMatrixWidth` and `kMatrixHeight` values.  It is accessible to FastLED's `blur2d()` function.

The `END_FASTLED_XY_COMPATIBILITY()` macro writes the contents of the `leds[]` array to WLED's pixel buffer

### Allocating Persistent Memory for Variables

FastLED sketches often store data in global variables inside the sketch, or static variables inside a function like `loop()`, where the value will remain the same between calls to the function.  This doesn't work with WLED, as having dedicated memory for an effect that's not running won't scale when there's many effects to choose from.  Also, each effect could be running on multiple segments, where each variable might have different data.

WLED allows data to be allocated specific segment when the effect first runs, but it's cumbersome to use for more than one object, and we're already allocating an object for the `leds[]` array.  `FL_ALLOC()` simplifies this by allocating memory for both the `leds[]` array and multiple variables.

For example to allocate memory for `unsigned long prevMillis` use this at the top of the sketch:

```
  FL_ALLOC(
    unsigned long prevMillis;
    );
```

This does everything `BEGIN_FASTLED_COMPATIBILITY()` does, plus allocates memory for the variables described between the parentheses.  You can allocate memory for multiple variables.

The variables are allocated in a struct, and can't be used directly.  To use one of the variables, wrap it in `FL_STATICVAR()`, e.g.

```
unsigned long curMillis = millis();
if ((curMillis - FL_STATICVAR(prevMillis)) >= 100) {
  FL_STATICVAR(prevMillis) = curMillis;
}
```

Use `END_FASTLED_COMPATIBILITY()` at the end of the effect.

TODO: create macro for `FL_ALLOC_XY()`, in the meantime use `FL_ALLOC_WITH_1_ARRAY_XY()` and create a dummy array to store 1 byte.

### Allocating Persistent Memory for Arrays

Arrays need to be allocated differently than variables, as they can't be put directly into the struct if they depend on a parameter that is variable per segment, e.g. `NUM_LEDS`.  The macro `FL_ALLOC_WITH_1_ARRAY_XY()` can allocate variables and an array. To allocate memory for `byte heat[NUM_LEDS]`, use:

```
  FL_ALLOC_WITH_1_ARRAY_XY(
    unsigned long prevMillis;
    , byte, heat, NUM_LEDS
    );
```

Note you can leave the first argument of the macro (where you'd put one or more variables to allocate) empty if you only need an array and no persistent variables.

Use the array just like you would a persistent variable, by wrapping it in `FL_STATICVAR()` e.g.

```
for (int mh = 0; mh < kMatrixHeight; mh++) {
  FL_STATICVAR(heat)[mw*kMatrixWidth+mh] = qsub8( FL_STATICVAR(heat)[mw*kMatrixWidth+mh],  random16(0, ((COOLING * 10) / kMatrixHeight) + 2));
}
```

TODO: create macro for `FL_ALLOC_WITH_1_ARRAY()` (no XY) and options for multiple arrays

### Setup()

If your FastLED sketch needs to run some code once during `Setup()`, add it to `FL_SETUP() { }` e.g.

```
FL_SETUP() {
  FL_STATICVAR(counterTriggered) = 0;
}
```

### Triggers (e.g. `EVERY_N_MILLIS()`)

FastLED has a really handy feature called triggers which allows code to be run periodically without having to keep track of the passage of time manually.  This doesn't work with WLED as is because behind the scenes, the trigger macro creates a static variable to hold a trigger timer object.  We can create the trigger timer object manually as we normally do for static variables, and then call it with a modified macro: `FL_EVERY_N()`.  To create a milliseconds trigger set to trigger every 100ms and use it:

```
FL_ALLOC(
  CEveryNMillis flMillisTrigger1;
);

FL_EVERY_N_MILLIS(flMillisTrigger1, 100) {
  // do something every 100ms here
}
```

These macros are defined for use with triggers:

```
FL_EVERY_N_MILLIS()   // use with CEveryNMillis
FL_EVERY_N_SECONDS()  // use with CEveryNSeconds
FL_EVERY_N_BSECONDS() // use with CEveryNBSeconds
FL_EVERY_N_MINUTES()  // use with CEveryNMinutes
FL_EVERY_N_HOURS()    // use with CEveryNHours
```

If you want to do more advanced things with the trigger timer, use it as you would another static variable. e.g.

```
FL_STATICVAR(triggerTimer).setPeriod(200);
```

### Nested Functions

Often a FastLED sketch will use multiple functions to simplify the code, but this isn't easy to port into WLED.  Some of the FL_Wrapper features described above use local variables which won't be accessible from another function.  The `FL_FUNCTIONS_BEGIN`, `FL_FUNCTIONS_END`, and `FL_FUNCTION()` macros allow for nesting functions and calling function inside the WLED effect.  An example showing two nested functions:

```
FL_FUNCTIONS_BEGIN // vvvvvvvvv Local Functions go here
    void brightenOrDarkenEachPixel( fract8 fadeUpAmount, fract8 fadeDownAmount)
    {
     for( uint16_t i = 0; i < NUM_LEDS; i++) {
        if( getPixelDirection(i) == GETTING_DARKER) {
          // This pixel is getting darker
          leds[i] = makeDarker( leds[i], fadeDownAmount);
        }
    }
    bool getPixelDirection( uint16_t i) {
      uint16_t index = i / 8;
      uint8_t  bitNum = i & 0x07;
      return bitRead( FL_STATICVAR(directionFlags)[index], bitNum);
    }
FL_FUNCTIONS_END // ^^^^^^^^^ Local Functions go here

FL_FUNCTION(brightenOrDarkenEachPixel)( FADE_IN_SPEED, FADE_OUT_SPEED);
```

Note that to call a nested function from the main effect function, you need to wrap the function name in `FL_FUNCTION()`.  To call a nested function from another nested function, you can't wrap the function name in `FL_FUNCTION()`.  Nested functions need to wrap static variables/arrays in `FL_STATICVAR()`

TODO: make it so `FL_FUNCTION()` use is consistent across the main and nested functions?

## Usage

Define `ENABLE_FL_WRAPPER`.  It's not useful as is without UserFX, so define `ENABLE_USERFX` at the same time.  Add to myconfig.h:

```
#define ENABLE_USERFX
#define ENABLE_FL_WRAPPER
```

or Add to PlatformIO Config:

```
-D ENABLE_USERFX
-D ENABLE_FL_WRAPPER
```

Now add one or more effects using FL_Wrapper, each one in its own UserFX slot.  To include the SWIRL_2D example in UserFX slot 1, add to myconfig.h:

```
#define USERFX1_H "../usermods/FastLED_Compatibility/FL_Example_Swirl2D.h"
#define USERFX1_NAME SWIRL2D
```

or Add to PlatformIO Config:

```
'-D USERFX1_H="../usermods/FastLED_Compatibility/FL_Example_Swirl2D.h"'
-D USERFX1_NAME=SWIRL2D
```

Compile and Upload and you should see a new "*2D Swirl" effect at the top of the Effects list.

### 2D Effects

These features are only enabled with `ENABLE_FL_MAPPING` defined, add to myconfig.h:

```
#define ENABLE_FL_MAPPING
```

or Add to PlatformIO Config:

```
-D ENABLE_FL_MAPPING
```

2D Mapping Values are loaded from JSON files stored in the LittleFS filesystem, just like the WLED config and presets.  You can open up the filesystem browser that WLED serves by adding `/edit` to the end of your WLED device URL.

Map files must start with `map_` and end with `.json` and only use lowercase characters.  The minimum contents for a map file is an array, containing 2-element arrays with X,Y coordinates as integers or floats.  

e.g. `[[0,0],[1,0],[0,1],[1,1]]` for a 2x2 matrix with "progressive" (not serpentine) layout.

The  units used for the coordinates do not matter, they will be internally scaled and converted to integers ranging from from 0 to `mapScaleMax` (currently 255, but this will likely change to another value that fits in a `uint16_t` in the future).  If your map starts above zero or uses negative numbers, the map will be shifted and scaled to fit within the 0-`mapScaleMax` range. 

The above example and this example (`[[-1.0,0.0],[1.0,0.0],[-1.0,1.0],[1.0,1.0]]` will both result in the following map file after being processed:

```
{"numElements":4,"coordinates":[[0,0],[255,0],[0,255],[255,255]]}
```

Currently the map files are all scaled to a square, ignoring any non 1:1 aspect ratio you may want in your map.  That will be improved in the future.

Making the mapping coordinates the same scale isn't good for effects that use a "virtual screen" (e.g. patterns that depend on the FastLED `XY()` function), as these effects likely need to compute pixel values for every pixel in the range `mapScaleMax*mapScaleMax`.  For these effects, we'll need to scale the map down to the minimum value to reduce RAM and CPU overhead to compute the effect.  

To add a map file that supports a Virtual Screen effect, your map file needs to add values for `"minVirtualScreenWidth"` and `"minVirtualScreenHeight"`.  e.g.:

```
{
	"minVirtualScreenWidth": 2,
	"minVirtualScreenHeight": 2,
	"coordinates": [
		[0, 0],
		[1, 0],
		[0, 1],
		[1, 1]
	]
}
```

This tells the effect that the Virtual Screen only needs to be 2 pixels wide and 2 pixels tall.  The above map will look like this after processing:

`{"numElements":4,"idealScaleX":0.007843,"idealScaleY":0.007843,"minVirtualScreenWidth":2,"minVirtualScreenHeight":2,"coordinates":[[0,0],[255,0],[0,255],[255,255]]}`

`"idealScaleX"` and `"idealScaleY"` may be removed from the map file in the future, as this is still a work in progress.

Map files don't do anything on their own, there's another file that describes how to optionally apply a map to each segment in WLED.  `mapnames.json` contains an array with an optional string for each segment with the filename of the map to apply to the segment. e.g.

`["/map_16x16serpentine.json","/map_dotstardisk17x17.json","/map_16x16progressive.json"]`

The leading slash for the root directory is required in the name.  To skip a segment, use a 0-length string: `""`. `mapnames.json` is extremely simple right now, but later will have a more complex layout to enable more functionality.

The onboard filesystem editor (`/edit`) has some limitations, but is pretty easy to use.  You can't create or access any folders besides root.  You can't rename.  Delete a file by right clicking on the name.  Make sure the name includes a leading slash for the root folder.

After uploading/editing your maps and `mapnames.json`, reboot to have the maps get cleaned up and loaded.  You can open the map files in the filesystem editor to see how they were changed.  If you compile with `WLED_DEBUG` defined, you'll see the result of the map cleanup and what was loaded.  If you compile with `WLED_DEBUG` and `JSON_MAP_DEBUG` defined, you'll see an overwhelming amount of information that might be useful for debugging.

This tool can be used to make map files for simple matrices:
https://editor.p5js.org/laskater/sketches/mbb4x9Po6

Some example maps are included in the `usermods/FastLED_Compatibility/maps/` directory.

The DotStar Disk mapping was adapted from: https://github.com/evilgeniuslabs/sol-v2/blob/master/Map.h

## Improvements

This UserFX concept is just a prototype, and the API will likely change in the future as the prototyping continues

Note some of the "TODO"s above marking incomplete features

