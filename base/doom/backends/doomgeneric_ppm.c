// doomgeneric_ppm.c - FreeLinX headless render-verification backend.
//
// This is a FreeLinX port addition (not upstream).  It renders the
// doomgeneric 32-bit framebuffer to uncompressed PPM files instead of a real
// framebuffer device, so the Doom engine's render/game core can be verified
// without /dev/fb0 (which the current FreeLinX kernel does not provide, since
// CONFIG_FB / CONFIG_DRM_FBDEV_EMULATION are disabled).
//
// Build target: `doombench`.  Writes /tmp/doomframe_NNNN.ppm.
#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_FRAMES 300
static unsigned int frameCount = 0;
static struct timespec t0;

static unsigned int nowMs(void) {
    struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t);
    return (unsigned int)((t.tv_sec - t0.tv_sec) * 1000 + (t.tv_nsec - t0.tv_nsec) / 1000000);
}

void DG_Init(void) {
    clock_gettime(CLOCK_MONOTONIC, &t0);
    fprintf(stderr, "[doombench] framebuffer %dx%d initialized (no /dev/fb0 needed)\n",
            DOOMGENERIC_RESX, DOOMGENERIC_RESY);
}

void DG_DrawFrame(void) {
    if (frameCount < MAX_FRAMES && (frameCount % 25 == 0 || frameCount == 0)) {
        char path[64];
        snprintf(path, sizeof path, "/tmp/doomframe_%04u.ppm", frameCount);
        FILE *f = fopen(path, "wb");
        if (f) {
            fprintf(f, "P6\n%d %d\n255\n", DOOMGENERIC_RESX, DOOMGENERIC_RESY);
            for (int y = 0; y < DOOMGENERIC_RESY; y++) {
                for (int x = 0; x < DOOMGENERIC_RESX; x++) {
                    unsigned int px = DG_ScreenBuffer[y * DOOMGENERIC_RESX + x];
                    unsigned char r = (px >> 16) & 0xff;
                    unsigned char g = (px >> 8) & 0xff;
                    unsigned char b = px & 0xff;
                    fputc(r, f); fputc(g, f); fputc(b, f);
                }
            }
            fclose(f);
            fprintf(stderr, "[doombench] wrote %s\n", path);
        }
    }
    frameCount++;
    if (frameCount >= MAX_FRAMES) {
        fprintf(stderr, "[doombench] done: rendered %u frames\n", frameCount);
        _exit(0);
    }
}

void DG_SleepMs(uint32_t ms) { usleep(ms * 1000); }

uint32_t DG_GetTicksMs(void) { return nowMs(); }

int DG_GetKey(int *pressed, unsigned char *doomKey) { (void)pressed; (void)doomKey; return 0; }

void DG_SetWindowTitle(const char *t) { (void)t; }

int main(int argc, char **argv) {
    doomgeneric_Create(argc, argv);
    while (1) doomgeneric_Tick();
    return 0;
}
