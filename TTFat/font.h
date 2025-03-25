//
// Created by Alife Zithu on 3/24/25.
//

#ifndef FONT_H
#define FONT_H

typedef struct FONT FONT;

FONT *TTFat_OpenFont(const char *dst);
void  TTFat_DestroyFont(FONT *font);

#endif //FONT_H
