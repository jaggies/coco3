/*
 * dither.h
 *
 *  Created on: Dec 12, 2018
 *      Author: jmiller
 */

#ifndef APPS_INTRAY_DITHER_H_
#define APPS_INTRAY_DITHER_H_

uint8_t dither(uint8_t inBits, uint8_t outBits, uint8_t x, uint8_t y, uint8_t grey);
uint8_t dither6x2(uint8_t x, uint8_t y, uint8_t grey);

#endif /* APPS_INTRAY_DITHER_H_ */
