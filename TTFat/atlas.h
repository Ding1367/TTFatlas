//
// Created by Alife Zithu on 3/24/25.
//

#ifndef ATLAS_H
#define ATLAS_H

typedef struct atlas_data {
    int width, height, characterCode;
    void *data;
} atlas_data_t;

typedef struct atlas {
    atlas_data_t *data;
    int characters;
} atlas_t;

atlas_data_t *TTFat_GetCharacterInAtlas(atlas_t *atlas, int characterCode);

#endif //ATLAS_H
