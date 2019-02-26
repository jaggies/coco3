/*
 * cc3hw.c
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"

uint8_t *FIRQ_VECTOR = * (uint8_t **) 0xFFF6;
uint8_t *IRQ_VECTOR = * (uint8_t **) 0xFFF8;
uint8_t *NMI_VECTOR = * (uint8_t **) 0xFFFC;

#define MEMWINDOW 0xc000
#define PAGE MMUC000
#define PAGEBITS 13
#define PAGESIZE (1 << PAGEBITS)

// Enables 6309 Native mode for higher performance
void set6309Native() {
    asm {
        ldmd  #$01
    }
}

void setFirq(interrupt void (*fptr)()) {
    *FIRQ_VECTOR = 0x7E;  // extended JMP extension
    *(void **) (FIRQ_VECTOR + 1) = (void *) fptr;
}

void setIrq(interrupt void (*fptr)()) {
    *IRQ_VECTOR = 0x7E;  // extended JMP extension
    *(void **) (IRQ_VECTOR + 1) = (void *) fptr;
}

void setNMI(interrupt void (*fptr)()) {
    *NMI_VECTOR = 0x7E;  // extended JMP extension
    *(void **) (NMI_VECTOR + 1) = (void *) fptr;
}

uint16_t min(uint16_t a, uint16_t b) {
    return a < b ? a : b;
}

void memset1(uint32_t addr, uint8_t value, uint8_t mask) {
    disableInterrupts();
    const uint8_t oldMMU = *PAGE;
    *PAGE = (uint8_t) (addr >> 13);
    uint8_t* ptr = (uint8_t*) MEMWINDOW + (((uint16_t) addr) & 0x1fff);
    uint8_t tmp = *ptr;
    tmp &= ~mask;
    tmp |= value;
    *(uint8_t*)ptr = tmp;
    *PAGE = oldMMU;
    enableInterrupts();
}

// Set maximum of 64k-1 bytes to any 24-bit page
void memset24(uint32_t addr, uint8_t value, uint16_t length) {
    disableInterrupts();
    uint8_t page = (uint8_t) (addr >> 13);
    uint16_t offset = (uint16_t) (addr & 0x1fffL); // initial offset in page
    const uint8_t oldMMU = *PAGE;
    while (length > 0) {
        const uint16_t size = min(PAGESIZE - offset, length);
        uint16_t ptr = MEMWINDOW + offset;
        *PAGE = page;
        memset((uint8_t*)ptr, value, size); // TODO: optimize memset with 16-bit STD
        length -= size;
        page++;
        offset = 0;
    }
    *PAGE = oldMMU;
    enableInterrupts();
}
