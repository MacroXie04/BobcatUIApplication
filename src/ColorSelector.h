#ifndef COLOR_SELECTOR_H
#define COLOR_SELECTOR_H
#include "../bobcat_ui/all.h"
#include "Enums.h"
#include <tuple>
#include <algorithm>

class ColorSelector : public bobcat::Group {
    bobcat::Slider *rSlider, *gSlider, *bSlider;
    float r{0.f}, g{0.f}, b{0.f};

    void refresh() {
        r = rSlider->value() / 255.f;
        g = gSlider->value() / 255.f;
        b = bSlider->value() / 255.f;
        redraw();
        if (onChangeCb) onChangeCb(this);
    }

    void draw() override {
        bobcat::Group::draw();

        int side = this->h();
        fl_color(fl_rgb_color(r * 255, g * 255, b * 255));
        fl_rectf(x(), y(), side, side);
        fl_color(FL_BLACK);
        fl_rect(x(), y(), side, side);
    }

public:
    ColorSelector(int x, int y, int w, int h) : Group(x, y, w, h) {
        this->box(FL_NO_BOX);

        int side = h;
        int sx = x + side;
        int sw = w - side;
        int sh = h / 3;

        rSlider = new bobcat::Slider(sx, y, sw, sh, "R");
        gSlider = new bobcat::Slider(sx, y + sh, sw, sh, "G");
        bSlider = new bobcat::Slider(sx, y + 2 * sh, sw, sh, "B");

        for (auto *s: {rSlider, gSlider, bSlider}) {
            s->type(FL_HORIZONTAL);
            s->range(0, 255);
            s->step(1);
            s->box(FL_THIN_UP_BOX);
        }
        rSlider->selection_color(fl_rgb_color(255, 128, 128));
        gSlider->selection_color(fl_rgb_color(128, 255, 128));
        bSlider->selection_color(fl_rgb_color(128, 128, 255));

        rSlider->onChange([this](bobcat::Widget *) { refresh(); });
        gSlider->onChange([this](bobcat::Widget *) { refresh(); });
        bSlider->onChange([this](bobcat::Widget *) { refresh(); });

        refresh();
    }

    std::tuple<float, float, float> getRGB() const { return {r, g, b}; }

    void setColorRGB(float R, float G, float B) {
        r = R;
        g = G;
        b = B;
        rSlider->value(R * 255.f);
        gSlider->value(G * 255.f);
        bSlider->value(B * 255.f);
        redraw();
    }
};

#endif
