/*
 * cc3hw.c
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"

uint8_t *irqVector = * (uint8_t **) 0xFFF8;
uint8_t *firqVector = * (uint8_t **) 0xFFF6;
uint8_t *hsyncCtrl = (uint8_t *) 0xff01;
uint8_t *vsyncCtrl = (uint8_t *) 0xff03;
uint8_t *init0 = (uint8_t *) 0xff90;
uint8_t *irqEn = (uint8_t *) 0xff92;
uint8_t *firqEn = (uint8_t *) 0xff93;
uint8_t *vertScroll = (uint8_t*) 0xff9c;
uint8_t *palCtrl = (uint8_t *) 0xffb0; // entry of palette 0
uint16_t *vertOffset = (uint16_t*) 0xff9d; // vertical offset, bits [18:3]
uint8_t *videoMode = (uint8_t*) 0xff98; // video resolution register
uint8_t *videoResolution = (uint8_t*) 0xff99; // video resolution register

#define MEMWINDOW 0x8000
#define PAGE MMU8000
#define PAGEBITS 13
#define PAGESIZE (1 << PAGEBITS)

// Enables 6309 Native mode for higher performance
void set6309Native() {
    asm {
        ldmd  #$01
    }
}

void setFirq(interrupt void (*fptr)()) {
    *firqVector = 0x7E;  // extended JMP extension
    *(void **) (firqVector + 1) = (void *) fptr;
}

void setIrq(interrupt void (*fptr)()) {
    *irqVector = 0x7E;  // extended JMP extension
    *(void **) (irqVector + 1) = (void *) fptr;
}

uint16_t min(uint16_t a, uint16_t b) {
    return a < b ? a : b;
}

// Set maximum of 64k-1 bytes to any 24-bit page
void memset24(uint32_t addr, uint8_t value, uint8_t mask, uint16_t length) {
    disableInterrupts();
    uint8_t page = (uint8_t) (addr >> 13);
    uint16_t offset = (uint16_t) (addr & 0x1fffL); // initial offset in page
    const uint8_t oldMMU = *PAGE;
    while (length > 0) {
        const uint16_t size = min(PAGESIZE - offset, length);
        uint16_t ptr = MEMWINDOW + offset;
        *PAGE = page;
        if (mask) {
            const uint16_t end = ptr + size;
            while (ptr < end) {
                uint8_t tmp = *(uint8_t*)ptr;
                tmp &= ~mask;
                tmp |= value;
                *(uint8_t*)ptr++ = tmp;
            }
        } else {
            memset((uint8_t*)ptr, value, size); // TODO: optimize memset with 16-bit STD
        }
        length -= size;
        page++;
        offset = 0;
    }
    *PAGE = oldMMU;
    enableInterrupts();
}
