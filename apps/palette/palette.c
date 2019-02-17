/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cocogfx.h"

const char GFXMODE = HS320x192x16;

#define DYNPAL 16  // number of dynamic palette entries
#define LINES 200

uint8_t *irqVector = * (uint8_t **) 0xFFF8;
uint8_t *firqVector = * (uint8_t **) 0xFFF6;
uint8_t *hsyncCtrl = (uint8_t *) 0xff01;
uint8_t *vsyncCtrl = (uint8_t *) 0xff03;
uint8_t *init0 = (uint8_t *) 0xff90;
uint8_t *firqEn = (uint8_t *) 0xff93;
uint8_t *palCtrl = (uint8_t *) 0xffb0; // entry of palette 0

// Enables 6309 Native mode for higher speed
void set6309Native() {
    asm {
        ldmd  #$01
    }
}

uint8_t* pal;
static uint8_t *data;

void simpleRGB() {
    for (uint8_t i = 0; i < 4; i++) {
        paletteRGB(i, i, 0, 0); // red
        paletteRGB(i+4, 0, i, 0); // green
        paletteRGB(i+8, 0, 0, i); // blue
        paletteRGB(i+12, i, i, i); // white
    }
}

interrupt void horizontalISR() {
    asm {
        ldb #DYNPAL
        ldu data
        ldx palCtrl
    loop:
        lda ,u+
        sta ,x+
        decb
        bne loop
        stu data
    }
}

interrupt void verticalISR() {
    if (*firqEn & 0x08) { // VBORD irq
        data = &pal[0];
    }
}

void enableVideoIRQs() {
    disableInterrupts();

    // Horizontal setup
    *hsyncCtrl = 0xf7; // enable Horizontal IRQ
    *irqVector = 0x7E;  // extended JMP extension
    *(void **) (irqVector + 1) = (void *) horizontalISR;

    // Vertical setup
    *firqVector = 0x7E;  // extended JMP extension
    *(void **) (firqVector + 1) = (void *) verticalISR;
    *init0 |= 0x10; // GIME FIRQ output enabled
    *firqEn = 0x08; // vertical boarder firq enabled

    enableInterrupts();
}

uint8_t toPal(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t result = 0;
    result = (result | ((r >> 1) & 1)) << 1;
    result = (result | ((g >> 1) & 1)) << 1;
    result = (result | ((b >> 1) & 1)) << 1;
    result = (result | (r & 1)) << 1;
    result = (result | (g & 1)) << 1;
    result = (result | (b & 1));
    return result;
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    hscreen(GFXMODE);
    simpleRGB();

    /* Draw pixels */
    for (int j = 0; j < 200; j++) {
        for (int i = 0; i < 320; i++) {
            hset(i, j, (uint8_t) (i / 20));
        }
    }

    /* Load per-line palette */
    enableVideoIRQs();
    pal = sbrk(DYNPAL*LINES);
    uint8_t* ptr = pal;
    for (uint8_t l = 0; l < LINES; l++) {
        for (uint8_t p = 0; p < DYNPAL; p++) {
            uint8_t r = (p & 4) ? (l & 3) : 0;
            uint8_t g = (p & 2) ? (l & 3) : 0;
            uint8_t b = (p & 1) ? (l & 3) : 0;
            *ptr++ = toPal(r, g, b);
        }
    }

    while (1)
        ;

    return 0;
}
