//
// Created by Alife Zithu on 3/24/25.
//

#include "font.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct glyph {
    int16_t numContours;
    int16_t xMin, yMin, xMax, yMax;
} _glyph_t;

typedef struct glyph_list {
    _glyph_t glyf;
    struct glyph_list *next;
} _glyph_list_t;

typedef struct FONT_TABLE {
    char tag[4];
    uint32_t checksum;
    uint32_t offset;
    uint32_t length;
    char *data;
} FONT_TABLE;

typedef struct font_directory {
    struct offset_subtable {
        uint32_t scalar;
        uint16_t nTables;
        uint16_t searchRange, entrySelector, rangeShift;
    } offsetSubtable;
    FONT_TABLE *tables;
} _font_directory_t;

typedef struct offset_subtable _offset_subtable_t;

struct FONT {
    FILE *f;
    _glyph_list_t *glyphs_head;
    _font_directory_t dir;
};

typedef FONT _FONT;

#define be_le_16(be) (((be) >> 8) | ((be) << 8))
#define be_le_32(be) (((be) >> 24) | (((be) >> 8) & 0xFF00) | (((be) << 8) & 0xFF0000) | ((be) << 24))
#define var_be_le_16(expr) expr = be_le_16(expr)
#define var_be_le_32(expr) expr = be_le_32(expr)

#define sizeof_FONT_TABLE (sizeof(FONT_TABLE) - sizeof(char*))

static int _InitDirectory(FONT *font) {
    FILE *f = font->f;
    _font_directory_t *dir = &font->dir;
    _offset_subtable_t *offset_sub = &dir->offsetSubtable;
    if (fread(offset_sub, sizeof(_offset_subtable_t), 1, f) < 1) return 0;

    var_be_le_32(offset_sub->scalar);
    var_be_le_16(offset_sub->nTables);
    var_be_le_16(offset_sub->searchRange);
    var_be_le_16(offset_sub->entrySelector);
    var_be_le_16(offset_sub->rangeShift);

    dir->tables = NULL;
    FONT_TABLE *tables = malloc(sizeof(FONT_TABLE) * offset_sub->nTables);
    if (!tables) return 0;
    // read contiguous array of nTables FONT_TABLEs
    if (fread(tables, sizeof_FONT_TABLE, offset_sub->nTables, f) < offset_sub->nTables) {
        free(tables);
        return 0;
    }

    // align structs (the horrors of inconsistent sizing)
    struct ___16 {
        uint64_t a[2];
    };
    for (int i = offset_sub->nTables; i; i--) {
        // what the fuck
        tables[i] = *(FONT_TABLE *)&((struct ___16*)tables)[i]; // move the 16-byte TTF tables to the first 16 bytes of one of the 24-byte elements
    }


    // load data for each table
    for (int i = 0; i < offset_sub->nTables; i++) {
        FONT_TABLE *table = &tables[i];
        // just to save a loop; convert structure data to little endian types
        var_be_le_32(table->checksum);
        var_be_le_32(table->offset);
        var_be_le_32(table->length);

        // position file pointer
        if (fseek(f, table->offset, SEEK_SET) == -1) {
            free(tables);
            return 0;
        }
        // mallocate data and read
        char *table_data = malloc(table->length);
        if (table_data == NULL) {
            free(tables);
            return 0;
        }
        if (fread(table_data, 1, table->length, f) < table->length) {
            free(tables);
            free(table_data);
            return 0;
        }
        table->data = table_data;
    }
    dir->tables = tables;
    return 1;
}

static FONT_TABLE *_GetFontTable(FONT *font, char *tag){
    for (int i = 0; i < font->dir.offsetSubtable.nTables; i++){
        FONT_TABLE *t = &font->dir.tables[i];
        if (strncmp(t->tag, tag, 4) == 0){
            return t;
        }
    }
    return NULL;
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
            free(font);
            return NULL;
        }
        font->glyphs_head = NULL;
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
    FONT_TABLE *tables = font->dir.tables;
    if (tables) {
        for (int i = 0; i < font->dir.offsetSubtable.nTables; i++) {
            free(tables[i].data);
        }
        free(tables);
    }
    fclose(font->f);
    free(font);
}
