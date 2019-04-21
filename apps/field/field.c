/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "coco.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "dither.h"

uint8_t field = 0;
#define XRES 640
#define YRES 225
#define DEPTH 2

uint32_t frameBase[3]; // Graphics pointer to each frame
uint8_t palette[3][16]; // palettes for each of the frames

interrupt void verticalISR() {
    *(uint16_t*) VERT_OFFSET = frameBase[field] >> 3;
    for (uint8_t i = 0; i < 4; i++) {
        PALETTE_BASE[i] = palette[field][i];
    }
    field = field < 2 ? (field + 1) : 0;
    asm {
        ldb 0xff92 // clear IRQ
        ldb 0xff93 // clear FIRQ
    }
}

interrupt void horizontalISR() {
    asm {
        ldb 0xff92 // clear IRQ
        ldb 0xff93 // clear FIRQ
    }
}

void enableVideoIRQs() {
    disableInterrupts();
    *INIT0 = 0x6c; // GIME CC3, MMU, DRAM, 16k/16k ROM, IRQ_en, FIRQ_en
    *IRQ_EN = 0x08; // DISABLE IRQs
    *FIRQ_EN = 0x00; // FIRQ for vertical ISR
    *HSYNC_CTRL &= 0xfe; // disable historic PIA Hsync IRQ
    *VSYNC_CTRL &= 0xfe; // disable historic PIA Vsync IRQ
    setIrq(verticalISR);
    setFirq(horizontalISR);
    enableInterrupts();
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    setMode(XRES, YRES, DEPTH);

    uint8_t* pixels[3];
    pixels[0] = (uint8_t*) sbrk(XRES);
    pixels[1] = (uint8_t*) sbrk(XRES);
    pixels[2] = (uint8_t*) sbrk(XRES);

    for (int frame = 0; frame < 3; frame++) {
        frameBase[frame] = frame * (uint32_t) getFrameSize();
        setGraphicsDrawBase(frameBase[frame]);
        clear(0xf);
        for (uint8_t i = 0; i < 16; i++) {
            uint8_t r = frame == 0 ? i : 0;
            uint8_t g = frame == 1 ? i : 0;
            uint8_t b = frame == 2 ? i : 0;
            palette[frame][i] = toPalette(r, g, b);
            setPalette(i, r, g, b);
        }
    }

    enableVideoIRQs();

    for (uint16_t j = 0; j < YRES; j++) {
        uint8_t b = (uint8_t) (j * 64 / YRES);
        uint8_t* red = &pixels[0][0];
        uint8_t* grn = &pixels[1][0];
        uint8_t* blu = &pixels[2][0];
        for (uint16_t i = 0; i < XRES; i++) {
            uint8_t r = (uint8_t)(i & 0x3f);
            uint8_t g = (uint8_t)(i / (XRES/64));
            *red++ = dither6x2((uint8_t) i, (uint8_t) j, r);
            *grn++ = dither6x2((uint8_t) i, (uint8_t) j, g);
            *blu++ = dither6x2((uint8_t) i, (uint8_t) j, b);
        }
        for (int k = 0; k < 3; k++) {
            setGraphicsDrawBase(frameBase[k]);
            setPixels(0, j, pixels[k], packPixels(pixels[k], pixels[k], XRES));
        }
    }

    while (1)
        ;

    return 0;
}
