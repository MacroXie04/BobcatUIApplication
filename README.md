# BobcatUIApplication

BobcatUIApplication is a complete paint application developed in C++ using the FLTK (Fast Light Toolkit) framework. It enables users to draw and manipulate graphical objects such as freeform scribbles and geometric shapes.

## Project Overview

This project was created to demonstrate interactive GUI programming, object-oriented design, and real-time rendering. The application supports a variety of tools and features that allow users to create, select, edit, and manage their drawings.

## Features and Scoring

| Feature                                                               | Points |
|-----------------------------------------------------------------------|--------|
| Paint Brush Tool — draw freeform scribbles (one drag = one object)   | 10     |
| Eraser Tool — delete selected shape or scribble                      | 5      |
| Clear Screen Tool — delete all shapes and scribbles                 | 5      |
| RGB Color Picker — choose custom colors                             | 10     |
| Four Shape Tools — e.g., Rectangle, Circle, Line, Triangle          | 10     |
| Selector Tool — click to select any shape or scribble               | 10     |
| Resize Tool — resize selected object interactively                   | 10     |
| Color Change — change color of selected shape or scribble           | 10     |
| Move Tool — drag to reposition selected shape or scribble           | 10     |
| Bring to Front — reorder selected object to the front               | 10     |
| Send to Back — reorder selected object to the back                  | 10     |

**Total: 100 Points**

Note: A "scribble" is defined as a set of points drawn during a single mouse-drag event (from mouse-down to mouse-up). Each scribble is stored and treated as a single object.

## Design Details

- All drawable objects (scribbles and shapes) are stored in a unified container (`std::vector<PrintObj>`), which allows consistent rendering and manipulation.
- Each object contains:
  - Shape type (`TOOL`)
  - Color (`r`, `g`, `b`)
  - Size (e.g., stroke width or shape size)
  - Points (`std::vector<Point>`)
- Selection and editing operations modify objects directly within this container.

## Build Instructions

### Prerequisites

- C++17 or later
- FLTK with OpenGL support

### Build

Use the provided Makefile, or run:

```
make run
```

## Usage

1. Choose a tool from the toolbar.
2. Click and drag on the canvas to draw.
3. Use the selector tool to select any object.
4. Change color, size, position, or delete selected objects.
5. Use layer tools to bring objects to the front or send them to the back.

## Code Structure

The core drawing element is defined as:

```cpp
struct PrintObj {
    TOOL tool;
    std::vector<Point> points;
    float r, g, b;
    int size;
};
```

## Author

Developed by Hongzhe Xie for the CSE programming project at UC Merced.