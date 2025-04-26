#ifndef SIZE_SELECTOR_H
#define SIZE_SELECTOR_H

// if you are using steamplug environment please use this to load bobcat_ui
// #include <bobcat_ui.h/all.h>

// if you are using local environment please use this to load bobcat_ui
#include "../bobcat_ui/all.h"
#include <iostream>
#include "Enums.h"

class SizeSelector : public bobcat::Group {
    bobcat::Button *minusButton;
    bobcat::TextBox *valueBox;
    bobcat::Button *plusButton;

    int val;
    int minVal;
    int maxVal;
    int step;

    void update() {
        valueBox->label(std::to_string(val));
        valueBox->redraw();
    }

    void minus(bobcat::Widget *) {
        if (val - step >= minVal) val -= step;
        update();
        if (DEBUG) std::cout << "Size: " << val << std::endl;
        if (onChangeCb) onChangeCb(this);
    }

    void plus(bobcat::Widget *) {
        if (val + step <= maxVal) val += step;
        update();
        if (DEBUG) std::cout << "Size: " << val << std::endl;
        if (onChangeCb) onChangeCb(this);
    }

public:
    SizeSelector(int x, int y, int w, int h, int initial = 10, int minV = 1, int maxV = 100, int stepV = 1)
        : Group(x, y, w, h), val(initial), minVal(minV), maxVal(maxV), step(stepV) {
        minusButton = new bobcat::Button(x, y, h, h, "-");
        valueBox = new bobcat::TextBox(x + h, y, w - 2 * h, h, "");
        plusButton = new bobcat::Button(x + w - h, y, h, h, "+");
        valueBox->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
        update();
        ON_CLICK(minusButton, SizeSelector::minus);
        ON_CLICK(plusButton, SizeSelector::plus);
    }

    int getSize() const {
        return val;
    }

    void setSize(int v) {
        if (v < minVal) v = minVal;
        if (v > maxVal) v = maxVal;
        val = v;
        update();
    }
};

#endif
