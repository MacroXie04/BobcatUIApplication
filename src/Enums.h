#ifndef ENUMS_H
#define ENUMS_H

extern bool DEBUG;

enum TOOL {
    MOUSE,
    PENCIL,
    ERASER,

    CIRCLE,
    TRIANGLE,
    RECTANGLE,
    POLYGON,

    PERSON,
};

enum ACTION {
    NONE,
    CLEAR,
    SEND_TO_BACK,
    BRING_TO_FRONT,
};


#endif
