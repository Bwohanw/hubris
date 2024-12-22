#include "Image.h"


void Image::lighten(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            double lum = getPixel(i,j).l;
            lum += amount;
            if (lum < 0) lum = 0;
            if (lum > 1) lum = 1;
            getPixel(i,j).l = lum;
        }
    }
}

void Image::lighten() { lighten(0.1); }

void Image::darken() { lighten(-0.1); }

void Image::darken(double amount) {lighten(-1*amount);}


void Image::saturate(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            double sat = getPixel(i,j).s;
            sat += amount;
            if (sat < 0) sat = 0;
            if (sat > 1) sat = 1;
            getPixel(i,j).s = sat;
        }
    }
}

void Image::saturate() { saturate(0.1); }

void Image::desaturate() { saturate(0.1); }

void Image::desaturate(double amount) { saturate(-1*amount); }


void Image::grayscale() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            getPixel(i,j).s = 0;
        }
    }
}


void Image::rotateColor(double degrees) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            double hue = getPixel(i,j).h;

            hue += degrees;
            
            while (hue >= 360) hue -= 360;
            while (hue < 0) hue += 360;

            getPixel(i,j).h = hue;
        }
    }
}


void Image::illinify() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            double hue = getPixel(i,j).h;

            //naive distance
            double blue_dist = std::abs(216 - hue);
            //compares to distance in the other direction
            blue_dist = std::min(blue_dist, 360 - blue_dist);

            double orange_dist = std::abs(11 - hue);
            orange_dist = std::min(orange_dist, 360 - orange_dist);

            if (blue_dist <= orange_dist) getPixel(i,j).h = 216;
            else getPixel(i,j).h = 11;
        }
    }
}


void Image::scale(double factor) {
    Image tmp(*this);

    resize(width() * factor, height() * factor);

    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            getPixel(i,j) = tmp.getPixel((int)(i/factor), (int)(j/factor));
        }
    }
}


void Image::scale(unsigned w, unsigned h) {
    double w_scale = (double)w/width();
    double h_scale = (double)h/height();

    scale(std::min(w_scale, h_scale));
}