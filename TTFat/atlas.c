//
// Created by Alife Zithu on 3/24/25.
//

#include "atlas.h"

#include <stddef.h>

atlas_data_t* TTFat_GetCharacterInAtlas(atlas_t *atlas, int characterCode) {
    for (int i = 0; i < atlas->characters; i++) {
        atlas_data_t *data = &atlas->data[i];
        if (data->characterCode == characterCode) return data;
    }
    return NULL;
}
