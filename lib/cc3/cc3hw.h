/*
 * cc3hw.h
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#ifndef LIB_COCO3_CC3HW_H_
#define LIB_COCO3_CC3HW_H_

extern const uint8_t *NMI_VECTOR; // MC6809E NMI vector in memory
extern const uint8_t *IRQ_VECTOR; // MC6809E IRQ vector in memory
extern const uint8_t *FIRQ_VECTOR; // MC6809E FIRQ vector in memory

#define HSYNC_CTRL ((uint8_t *) 0xff01)
#define VSYNC_CTRL ((uint8_t *) 0xff03)
#define INIT0 ((uint8_t *) 0xff90)
#define IRQ_EN ((uint8_t *) 0xff92) // IRQ enable register in GIME chip
#define FIRQ_EN ((uint8_t *) 0xff93) // FIRQ enable register in GIME chip
#define VERT_SCROLL ((uint8_t*) 0xff9c)  // vertical scroll register
#define PALETTE_BASE ((uint8_t *) 0xffb0) // entry of palette 0
#define HORIZ_OFFSET ((uint8_t *) 0xff9f) // vertical offset, bits [18:3]
#define VERT_OFFSET ((uint16_t*) 0xff9d) // vertical offset, bits [18:3]
#define VIDEO_MODE ((uint8_t*) 0xff98) // video mode control register
#define VIDEO_RES ((uint8_t*) 0xff99) // video resolution register

#define MMU0000 ((uint8_t*) 0xffa0)
#define MMU2000 ((uint8_t*) 0xffa1)
#define MMU4000 ((uint8_t*) 0xffa2)
#define MMU6000 ((uint8_t*) 0xffa3)
#define MMU8000 ((uint8_t*) 0xffa4)
#define MMUA000 ((uint8_t*) 0xffa5)
#define MMUC000 ((uint8_t*) 0xffa6)
#define MMUE000 ((uint8_t*) 0xffa7)

extern void set6309Native(); // Enables 6309 Native mode for higher performance
extern void memset1(uint32_t addr, uint8_t value, uint8_t mask);
extern void memset24(uint32_t addr, uint8_t value, uint16_t length);
extern void memcpy24(uint32_t dst, uint8_t* src, uint16_t length);
extern void setFirq(interrupt void (*fptr)());
extern void setIrq(interrupt void (*fptr)());
extern void setNMI(interrupt void (*fptr)());

#endif /* LIB_COCO3_CC3HW_H_ */
