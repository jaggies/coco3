/*
 * dither.h
 *
 *  Created on: Dec 12, 2018
 *      Author: jmiller
 */

#ifndef APPS_INTRAY_DITHER_H_
#define APPS_INTRAY_DITHER_H_

// TODO: Optimize this for 8-bit compute
int dither(int inLevels, int outLevels, int x, int y, int grey);

#endif /* APPS_INTRAY_DITHER_H_ */
