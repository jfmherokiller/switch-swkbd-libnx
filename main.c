#include <string.h>
#include <stdio.h>

#include <switch.h>
#include <unistd.h>

#include "swkbd.h"


void FlushGraphics() {
    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();
}

int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(NULL);
    Initkeyboard();
    FlushGraphics();
    sleep(5);

    // Main loop
    if (0) {
        while (appletMainLoop()) {
            //Scan all the inputs. This should be done once for each frame
            hidScanInput();

            // Your code goes here
            printf("Hello World!");
            //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

            if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

            gfxFlushBuffers();
            gfxSwapBuffers();
            gfxWaitForVsync();
        }
    }

    gfxExit();
    return 0;
}

