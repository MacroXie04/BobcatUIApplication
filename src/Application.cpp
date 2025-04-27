#include "Application.h"
#include "Enums.h"
#include <iostream>

using namespace bobcat;

void Application::onCanvasMouseDown(bobcat::Widget *, float mx, float my) {
    // DEBUG
    if (DEBUG) {
        std::cout << "onCanvasMouseDown" << std::endl;
        std::cout << "mx: " << mx << ", my: " << my << std::endl;
        std::cout << "Tool: " << toolbar->getTool() << std::endl;
    }

    // get the current tool from the toolbar
    TOOL t = toolbar->getTool();

    // handle the eraser tool
    if (t == ERASER) {
        canvas->eraseAt(mx, my);
        canvas->redraw();
        return;
    }

    // handle the selection tool
    if (t == MOUSE) {
        bool hit = canvas->selectAt(mx, my);
        dragging = hit;

        if (hit) {
            // DEBUG
            if (DEBUG) {
                std::cout << "Hit: " << mx << ", " << my << std::endl;
            }

            // only update UI on a true hit
            auto *obj = canvas->getSelected();
            colorSelector->setColorRGB(obj->getR(), obj->getG(), obj->getB());
            sizeSelector->setSize(obj->getSize());
            lastX = mx;
            lastY = my;
        } else {
            // on miss: do nothing (selection remains as before)
            canvas->unselect();
        }

        canvas->redraw();
        return;
    }

    // all other tools start a new stroke
    auto [r, g, b] = colorSelector->getRGB();
    canvas->startPrint(t, r, g, b, sizeSelector->getSize(), mx, my);
    canvas->redraw();
}


void Application::onCanvasDrag(bobcat::Widget *, float mx, float my) {
    // DEBUG
    if (DEBUG) {
        std::cout << "onCanvasDrag" << std::endl;
        std::cout << "mx: " << mx << ", my: " << my << std::endl;
    }

    // get the current tool from the toolbar
    TOOL t = toolbar->getTool();

    // handle the pencil tool
    if (t == PENCIL) {
        canvas->addPointToActive(mx, my);
        canvas->redraw();
    } else if (t == MOUSE && dragging && canvas->hasSelected()) {
        float dx = mx - lastX, dy = my - lastY;
        lastX = mx;
        lastY = my;
        canvas->moveSelectedBy(dx, dy);
        canvas->redraw();
    }

    // handle the eraser tool
    if (t == ERASER) {
        canvas->eraseAt(mx, my);
        canvas->redraw();
        return;
    }
}

void Application::onToolbarChange(bobcat::Widget *) {
    // get the current action from the toolbar
    ACTION act = toolbar->getAction();

    // clear the canvas
    if (act == CLEAR) {
        canvas->clear();
        return;
    }

    // handle the send to back and bring to front actions
    if (!canvas->hasSelected()) {
        return;
    }
    if (act == SEND_TO_BACK) {
        canvas->sendSelectedToBack();
    } else if (act == BRING_TO_FRONT) {
        canvas->bringSelectedToFront();
    }
    canvas->redraw();
}

void Application::onSizeChange(bobcat::Widget *) {
    if (canvas->hasSelected()) {
        canvas->getSelected()->setSize(sizeSelector->getSize());
        canvas->redraw();
    }
}

void Application::onColorChange(bobcat::Widget *) {
    if (canvas->hasSelected()) {
        auto [r,g,b] = colorSelector->getRGB();
        canvas->getSelected()->setColor(r, g, b);
        canvas->redraw();
    }
}

Application::Application() {
    // initialize the application and size of each window
    window = new bobcat::Window(20, 20, 700, 500, "Programming Project");
    toolbar = new Toolbar(0, 0, 50, 600);
    canvas = new Canvas(50, 0, 650, 450);
    colorSelector = new ColorSelector(50, 450, 350, 50);
    sizeSelector = new SizeSelector(550, 450, 150, 50);
    colorSelector->box(FL_BORDER_BOX);

    // add the widgets to the window
    window->add(toolbar);
    window->add(canvas);
    window->add(colorSelector);
    window->add(sizeSelector);

    // set the canvas callback functions
    ON_MOUSE_DOWN(canvas, Application::onCanvasMouseDown);
    ON_DRAG(canvas, Application::onCanvasDrag);
    ON_CHANGE(toolbar, Application::onToolbarChange);
    ON_CHANGE(sizeSelector, Application::onSizeChange);
    ON_CHANGE(colorSelector, Application::onColorChange);

    // set the default values for the color and size selectors
    lastX = lastY = 0;
    dragging = false;

    // show the window
    window->show();
}
