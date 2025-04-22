#include "Application.h"
#include "Enums.h"
#include <iostream>

using namespace bobcat;

void Application::onCanvasMouseDown(bobcat::Widget*, float mx, float my) {
    TOOL t = toolbar->getTool();
    if (t == ERASER) {
        canvas->eraseAt(mx, my);
        canvas->redraw();
        return;
    }

    if (t == MOUSE) {
        bool hit = canvas->selectAt(mx, my);
        dragging = hit;

        if (hit) {
            // only update UI on a true hit
            auto *obj = canvas->getSelected();
            colorSelector->setColorRGB(obj->getR(), obj->getG(), obj->getB());
            sizeSelector->setSize(obj->getSize());
            lastX = mx; 
            lastY = my;
        }
        // on miss: do nothing (selection remains as before)
        // canvas->unselect();

        canvas->redraw();
        return;
    }

    // all other tools start a new stroke
    auto [r, g, b] = colorSelector->getRGB();
    canvas->startPrint(t, r, g, b, sizeSelector->getSize(), mx, my);
    canvas->redraw();
}




void Application::onCanvasDrag(bobcat::Widget*,float mx,float my){
    TOOL t=toolbar->getTool();
    if(t==PENCIL){canvas->addPointToActive(mx,my);canvas->redraw();}
    else if(t==MOUSE&&dragging&&canvas->hasSelected()){
        float dx=mx-lastX,dy=my-lastY;lastX=mx;lastY=my;
        canvas->moveSelectedBy(dx,dy);canvas->redraw();
    }
    if (t == ERASER) { canvas->eraseAt(mx, my); canvas->redraw(); return; }
}

void Application::onToolbarChange(bobcat::Widget*){
    ACTION act=toolbar->getAction();
    if(act==CLEAR){canvas->clear();return;}
    if(!canvas->hasSelected())return;
    if(act==SEND_TO_BACK)canvas->sendSelectedToBack();
    else if(act==BRING_TO_FRONT)canvas->bringSelectedToFront();
    canvas->redraw();
}

void Application::onSizeChange(bobcat::Widget*){
    if(canvas->hasSelected()){
        canvas->getSelected()->setSize(sizeSelector->getSize());
        canvas->redraw();
    }
}

void Application::onColorChange(bobcat::Widget*){
    if(canvas->hasSelected()){
        auto[r,g,b]=colorSelector->getRGB();
        canvas->getSelected()->setColor(r,g,b);
        canvas->redraw();
    }
}

Application::Application(){
    window=new Window(20,20,700,500,"Programming Project");
    toolbar=new Toolbar(0,0,50,600);
    canvas=new Canvas(50,0,650,450);
    colorSelector=new ColorSelector(50,450,350,50);
    sizeSelector=new SizeSelector(550,450,150,50);
    colorSelector->box(FL_BORDER_BOX);
    window->add(toolbar);window->add(canvas);window->add(colorSelector);window->add(sizeSelector);
    ON_MOUSE_DOWN(canvas,Application::onCanvasMouseDown);
    ON_DRAG(canvas,Application::onCanvasDrag);
    ON_CHANGE(toolbar,Application::onToolbarChange);
    ON_CHANGE(sizeSelector,Application::onSizeChange);
    ON_CHANGE(colorSelector,Application::onColorChange);
    lastX=lastY=0;dragging=false;
    window->show();
}
