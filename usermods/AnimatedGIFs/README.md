# AnimatedGIFs Usermod

## Compiling

- Depends on UserFX and FL_Wrapper to be enabled
- define `USERMOD_ANIMATEDGIFS` in my_config.h or PlatformIO config
- Add the AnimatedGIFs_Effect using UserFX, e.g.

```
#define USERFX2_H "../usermods/AnimatedGIFs/AnimatedGIFs_Effect.h"
#define USERFX2_NAME AGIFS_TEST
```

- Add this line to `my_config.h`
	- `#include "../usermods/AnimatedGIFs/animatedgifs_common.h"`
	- This is a bit of a hack right now, the UserFX AGIFS_TEST effect needs to access a usermod header file, but usermod header files aren't normally exposed to FX.h
- Edit usermod_animatedgifs.h adding GIFs in C header file format, and adding references to the GIF content in the lists (see top of the file)
	- There's limited space, not much room for many GIFs
	- This will shortly be replaced by the ability to add GIFs to the LittleFS filesystem and an external SD card
- You should see a "*2D AnimatedGIFs" effect added towards the top of the effects list once compiled and uploaded

## Usage

- On first boot, the usermod adds config values to cfg.JSON
- defaults:

```
	"um": {
		"agif": {
			"vsW": 64,
			"vsH": 64,
			"dtSec": 10,
			"numCyc": 100,
			"playMode": 0
		}
	}
```

- "vsW" - virtual screen width, the width of the virtual screen buffer used to the decode the GIF
	- Set to the max width of the 2D segment(s) displaying the AGIFS_TEST effect
	- A larger than necessary value just costs more memory and potentially more CPU (more pixels can be decoded)
- "vsW" - virtual screen height, the height of the virtual screen buffer used to the decode the GIF
	- Set to the max height of the 2D segment(s) displaying the AGIFS_TEST effect
- "dtSec" - display time in seconds - how long do you want to display the GIF before cycling to the next GIF?
- "numCyc" - number of cycles to play the GIF before playing the next GIF
- "playMode" - the logic that will determine when to play the next GIF
	- playModeFirst = 0 - play the next GIF after "dtSec" OR "numCyc" is reached, whichever comes first
	- playModeBoth = 1 - play the next GIF after "dtSec" AND "numCyc" is reached
- Edit cfg.json (using the `/edit` filesystem editor feature), save, and reboot for settings to have effect

### Using Effect

The intensity slider can be used to select the index of the next GIF.  When the slider changes, the new index is sent to the usermod, and a flag is set to change the gif on the next frame

