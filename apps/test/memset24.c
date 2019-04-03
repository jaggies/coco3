#include <stdio.h>
#include <stdint.h>

#define PAGE_BITS 13
#define PAGE_SIZE (1 << PAGE_BITS)

int16_t memWindow = 0x8000;

uint16_t min(uint16_t a, uint16_t b) {
    return a < b ? a : b;
}

// Copy maximum of 64k to any 32-bit page
void ms24(uint32_t addr, uint8_t value, uint16_t length) {
    uint8_t page = (uint8_t) (addr >> 13);
    uint16_t offset = (uint16_t) (addr & 0x1fffL); // initial offset in page
	printf("addr:%x page:%x offset:%x\n", addr, page, offset);
    while (length > 0) {
        uint16_t ptr = memWindow + offset;
        const uint16_t size = min(PAGE_SIZE - offset, length);
        const uint16_t end = ptr + size;
        while (ptr < end) {
			printf("%x:%x = %x\n", page, ptr, value);
            ptr++;
        }
        length -= size;
        page++;
        offset = 0;
    }
}

int main(int argc, char** argv) {
    ms24(0x60000, 0xff, 65535);
}
