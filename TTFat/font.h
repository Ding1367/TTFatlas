//
// Created by Alife Zithu on 3/24/25.
//

#ifndef FONT_H
#define FONT_H

typedef struct FONT FONT;
typedef struct FONT_INFO {
    unsigned int signature, version, directoryOffset;
} FONT_INFO;

FONT *TTFat_OpenFont(const char *dst);
void  TTFat_DestroyFont(FONT *font);

FONT_INFO *TTFat_GetInfo(FONT *font);

#endif //FONT_H
