//
// Created by Alife Zithu on 3/24/25.
//

#include "font.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct glyph {
    int16_t numContours;
    int16_t xMin, yMin, xMax, yMax;
} _glyph_t;

typedef struct glyph_list {
    _glyph_t glyf;
    struct glyph_list *next;
} _glyph_list_t;

typedef struct font_directory {
    uint32_t scalar;
    uint16_t nTables;
    uint16_t searchRange, entrySelector, rangeShift;
} _font_directory_t;

struct FONT {
    FILE *f;
    FONT_INFO info;
    _glyph_list_t *glyphs_head;
    _font_directory_t *dir;
};

typedef FONT _FONT;

#define be_le_16(be) (((be) >> 8) | ((be) << 8))
#define be_le_32(be) (((be) >> 24) | (((be) >> 8) & 0xFF00) | (((be) << 8) & 0xFF0000) | ((be) << 24))
#define var_be_le_16(expr) expr = be_le_16(expr)
#define var_be_le_32(expr) expr = be_le_32(expr)

static int _InitDirectory(FONT *font) {
    // first, get the info
    FILE *f = font->f;
    if (fread(&font->info, sizeof(FONT_INFO), 1, f) < 1) {
        return 0;
    }
    FONT_INFO *info = &font->info;
    var_be_le_32(info->signature);
    var_be_le_32(info->version);
    var_be_le_32(info->directoryOffset);

    if (fseek(f, info->directoryOffset, SEEK_SET) == -1) {
        return 0;
    }
    font->dir = malloc(sizeof(_font_directory_t));
    if (font->dir == NULL) return 0;
    if (fread(font->dir, sizeof(_font_directory_t), 1, f) < 1) {
        return 0;
    }
    var_be_le_32(font->dir->scalar);
    var_be_le_16(font->dir->nTables);
    var_be_le_16(font->dir->searchRange);
    var_be_le_16(font->dir->entrySelector);
    var_be_le_16(font->dir->rangeShift);
    return 1;
}

FONT* TTFat_OpenFont(const char* dst) {
    FONT *font = malloc(sizeof(FONT));
    if (font != NULL) {
        FILE *file = fopen(dst, "rb");
        if (file == NULL) {
            free(font);
            return NULL;
        }
        font->f = file;
        if (!_InitDirectory(font)) {
            fclose(file);
            free(font->dir);
            free(font);
            return NULL;
        }
        font->glyphs_head = NULL;
        font->dir = NULL;
    }
    return font;
}

void TTFat_DestroyFont(FONT* font) {
    _glyph_list_t *node = font->glyphs_head;
    for(;;) {
        if (node == NULL) break;
        _glyph_list_t *next = node->next;
        free(node);
        node = next;
    }
    fclose(font->f);
    free(font);
}

FONT_INFO* TTFat_GetInfo(FONT* font) {
    return &font->info;
}
