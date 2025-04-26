#ifndef CANVAS_H
#define CANVAS_H

// if you are using steamplug environment please use this to load bobcat_ui
// #include <bobcat_ui.h/all.h>

// if you are using local environment please use this to load bobcat_ui
#include "../bobcat_ui/all.h"
#include <vector>
#include "Print_obj.h"

class Canvas : public bobcat::Canvas_ {
    // the vector to storage PrintObj(contain color, postion, tool)
    std::vector<PrintObj> prints;

    //
    int active;
    int selected;

public:
    Canvas(int x, int y, int w, int h);

    void startPrint(TOOL tool, float r, float g, float b, int size, float x, float y);

    void addPointToActive(float x, float y);

    void setLastPointActive(float x, float y);

    void eraseAt(float x, float y);

    void clear();

    bool selectAt(float x, float y);

    void unselect();

    bool hasSelected() const;

    void moveSelectedBy(float dx, float dy);

    PrintObj *getSelected();

    void sendSelectedToBack();

    void bringSelectedToFront();

    void render() override;
};

#endif
