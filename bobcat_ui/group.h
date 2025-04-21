#ifndef BOBCAT_UI_GROUP
#define BOBCAT_UI_GROUP

#include "bobcat_ui.h"

#include <FL/Enumerations.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <GL/gl.h>

#include <string>
#include <functional>

namespace bobcat {

class Group : public Fl_Group {
private:
    std::string caption;

    void init(){
        onChangeCb = nullptr;
        onEnterCb = nullptr;
        onLeaveCb = nullptr;
    }
protected:
    std::function<void(bobcat::Widget *)> onChangeCb;
    std::function<void(bobcat::Widget *)> onEnterCb;
    std::function<void(bobcat::Widget *)> onLeaveCb;

public:
    Group(int x, int y, int w, int h, std::string title = "") : Fl_Group(x, y, w, h, title.c_str()) { 
        init();
        caption = title;
        Fl_Widget::copy_label(caption.c_str());
    }

    void onChange(std::function<void(bobcat::Widget *)> cb) {
        onChangeCb = cb;
    }

    void onEnter(std::function<void(bobcat::Widget *)> cb){
        onEnterCb = cb;
    }

    void onLeave(std::function<void(bobcat::Widget *)> cb){
        onLeaveCb = cb;
    }

    std::string label() const {
        return caption;
    }

    void label(std::string s){
        Fl_Widget::copy_label(s.c_str());
        caption = s;
    }

    // void show() override {
    //     Fl_Group::show();
    //     // wait_for_expose();          // Supposedly makes show() synchronous
    //     Fl::flush();                // Make sure to draw what needs to be drawn
    // }

    friend struct ::AppTest;
};



}

#endif