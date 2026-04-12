/*
 * fonts.h
 *
 *  Created on: 12 апр. 2026 г.
 *      Author: erlbriton
 */

#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include <stdint.h>

// Структура для описания шрифта
typedef struct {
    const uint8_t width;    // Ширина в пикселях
    const uint8_t height;   // Высота в пикселях
    const uint16_t *data;   // Указатель на данные (битмап)
} FontDef;

// Объявляем наши шрифты (сами массивы будут в fonts.c)
extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;

#endif /* INC_FONTS_H_ */
