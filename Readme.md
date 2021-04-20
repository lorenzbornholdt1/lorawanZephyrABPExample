## Errors
### 1:
    *** Multiple ways to build the same target were specified for: C:\Users\abn069\Documents\PlatformIO\Projects\lorawanDemoZephyrOS\.pio\build\particle_xenon\loramac-node\zephyr\subsys\lorawan\sx1276.c.o   

#### Windows:
Go the the path of the LoRa-Driver in Zephyr:

    %HOMEPATH%/.platformio/packages/framework-zephyr-loramac-node/src/radio/sx1276
Rename the file sx1276.c to radio_sx1276.c

Edit the file 

    %HOMEPATH%\.platformio\packages\framework-zephyr-loramac-node\zephyr\CMakeLists.txt
And change sx1276.c to radio_sx1276.c

Save. Clean project and compile again.