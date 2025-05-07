#ifndef BOBCAT_UI_TEXTURE
#define BOBCAT_UI_TEXTURE

// Base on the environment, include the corresponding header files
#if defined(_WIN32)
    #include <FL/Enumerations.H>
    #include <FL/Fl_Double_Window.H>
    #include <FL/Fl_PNG_Image.H>
#elif defined(__APPLE__)
    #include <FL/Enumerations.H>
    #include <FL/Fl_Double_Window.H>
    #include <FL/Fl_PNG_Image.H>
#else
    #include <FL/Enumerations.H>
    #include <FL/Fl_Double_Window.H>
    #include <FL/Fl_PNG_Image.H>
#endif

#include <string>

namespace bobcat {

    class Texture {
        GLuint id{};
        int w{}, h{};
    public:
        explicit Texture(const std::string& file);
        void draw(float cx, float cy, float pxSize, float hpx, float aspect);
        ~Texture();
    };

}

#endif
