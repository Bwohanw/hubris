#include "StickerSheet.h"

StickerSheet::StickerSheet(const Image& picture): base(picture) {}


int StickerSheet::addSticker(Image& sticker, int x, int y) {
    for (unsigned i = 0; i < stickers.size(); i++) {
        if (stickers[i] == NULL) {
            stickers[i] = &sticker;
            x_coors[i] = x;
            y_coors[i] = y;
            return i;
        }
    }
    stickers.push_back(&sticker);
    x_coors.push_back(x);
    y_coors.push_back(y);
    return stickers.size() - 1;
}

int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y) {
    if (layer >= stickers.size()) {
        stickers.push_back(&sticker);
        x_coors.push_back(x);
        y_coors.push_back(y);
        return stickers.size() - 1;
    }
    stickers[layer] = &sticker;
    x_coors[layer] = x;
    y_coors[layer] = y;
    return layer;
}

bool StickerSheet::translate(unsigned index, int x, int y) {
    if (index >= stickers.size() || stickers[index] == NULL) return false;

    x_coors[index] = x;
    y_coors[index] = y;
    return true;
}

void StickerSheet::removeSticker(unsigned index) {
    if (index >= stickers.size()) return;
    stickers[index] = NULL;
}

Image* StickerSheet::getSticker(unsigned index) {
    if (index >= stickers.size()) return NULL;
    return stickers[index];
}

int StickerSheet::layers() const {
    return stickers.size();
}


Image StickerSheet::render() const {
    int left = 0;
    int up = 0;
    int right = base.width();
    int down = base.height();

    //finds maximum change in the directions
    //gets final size first to avoid unnecessary resize operations when rendering

    for (unsigned i = 0; i < stickers.size(); i++) {
        if (stickers[i] == NULL) continue;

        if (x_coors[i] < left) left = x_coors[i];
        if (y_coors[i] < up) up = y_coors[i];
        if (x_coors[i] + (int)(stickers[i]->width()) > right) right = x_coors[i] + stickers[i]->width();
        if (y_coors[i] + (int)(stickers[i]->height()) > down) down = y_coors[i] + stickers[i]->height();
    }

    Image rendered(right - left, down - up);//left and up are <= 0

    for (unsigned i = 0; i < base.width(); i++) {
        for (unsigned j = 0; j < base.height(); j++) {
            if (base.getPixel(i,j).a == 0) continue;
            rendered.getPixel(i - left, j - up) = base.getPixel(i,j);
        }
    }

    for (unsigned int i = 0; i < stickers.size(); i++) {
        Image* sticker = stickers[i];
        if (sticker == NULL) continue;

        for (unsigned x = 0; x < sticker->width(); x++) {
            for (unsigned y = 0; y < sticker->height(); y++) {
                if (sticker->getPixel(x,y).a == 0) continue;
                rendered.getPixel(x - left + x_coors[i], y - up + y_coors[i]) = sticker->getPixel(x,y);
            }
        }
    }

    return rendered;
}