#include "Canvas.h"
#include <cmath>
#include "stb_image.h"


Canvas::Canvas(int x, int y, int w, int h): Canvas_(x, y, w, h) {
    active = -1;
    selected = -1;
}

void Canvas::startPrint(TOOL tool, float r, float g, float b, int size, float x, float y) {
    prints.emplace_back(tool, r, g, b, size);
    active = prints.size() - 1;
    prints.back().addPoint(x, y);
    selected = -1;
}

void Canvas::addPointToActive(float x, float y) {
    if (active >= 0) {
        prints[active].addPoint(x, y);
    }
}

void Canvas::eraseAt(float x, float y) {
    int hp = h();
    for (int i = (int) prints.size() - 1; i >= 0; --i) {
        float thr = prints[i].getSize() * 2.0f / hp, thr2 = thr * thr;
        for (auto &p: prints[i].getPoints()) {
            float dx = p.x - x, dy = p.y - y;
            if (dx * dx + dy * dy <= thr2) {
                prints.erase(prints.begin() + i);
                active = -1;
                if (selected == i)selected = -1;
                if (selected > i)--selected;
                redraw();
                return;
            }
        }
    }
}

void Canvas::clear() {
    prints.clear();
    active = -1;
    selected = -1;
    redraw();
}

void Canvas::setLastPointActive(float x, float y) {
    if (active >= 0) {
        prints[active].setLastPoint(x, y);
    }
}

// === Canvas.cpp ===
bool Canvas::selectAt(float x, float y) {
    int hp = h();
    // iterate from topmost (last‐drawn) backwards
    for (int i = (int) prints.size() - 1; i >= 0; --i) {
        // use the same threshold logic as eraseAt
        float thr = prints[i].getSize() * 2.0f / hp;
        float thr2 = thr * thr;
        for (auto &p: prints[i].getPoints()) {
            float dx = p.x - x;
            float dy = p.y - y;
            if (dx * dx + dy * dy <= thr2) {
                selected = i;
                return true;
            }
        }
    }

    return false;
}


void Canvas::unselect() {
    selected = -1;
}

bool Canvas::hasSelected() const {
    return selected != -1;
}

void Canvas::moveSelectedBy(float dx, float dy) {
    if (selected >= 0) {
        prints[selected].moveBy(dx, dy);
    }
}

PrintObj *Canvas::getSelected() {
    return selected < 0 ? nullptr : &prints[selected];
}

void Canvas::sendSelectedToBack() {
    // if selected is -1 or 0, do nothing
    if (selected <= 0) {
        return;
    }

    // move the selected object to the back
    PrintObj obj = prints[selected];
    prints.erase(prints.begin() + selected);
    prints.insert(prints.begin(), obj);
    selected = 0;
}

void Canvas::bringSelectedToFront() {
    // if selected is -1 or last, do nothing
    if (selected < 0 || selected == (int) prints.size() - 1) {
        return;
    }

    // move the selected object to the front
    PrintObj obj = prints[selected];
    prints.erase(prints.begin() + selected);
    prints.push_back(obj);
    selected = prints.size() - 1;
}

void Canvas::render() {
    if (!valid()) {
        glViewport(0, 0, w(), h());
    }
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float wpx = static_cast<float>(w());
    float hpx = static_cast<float>(h());
    float aspect = wpx / hpx;

    constexpr float sel_px = 4.0f;
    constexpr float sel_line_width = 2.0f;
    float selu = 2.0f * sel_px / hpx;

    for (size_t idx = 0; idx < prints.size(); ++idx) {
        auto &p = prints[idx];

        float u = 2.0f * p.getSize() / hpx;

        switch (p.getTool()) {
            case PERSON: {
                static GLuint tex = 0;
                static int texW = 0, texH = 0;
                if (tex == 0) {
                    int channels;
                    stbi_set_flip_vertically_on_load(1);          // 依旧先上下翻一次
                    unsigned char* data =
                        stbi_load("assets/person.png", &texW, &texH, &channels, STBI_rgb_alpha);
                    if (!data) { std::cerr << "image load failed\n"; break; }

                    glGenTextures(1, &tex);
                    glBindTexture(GL_TEXTURE_2D, tex);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE, data);
                    stbi_image_free(data);
                }

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                auto c = p.getPoint(0);
                float halfW = 2.0f * p.getSize() / hpx;
                float ratio = static_cast<float>(texH) / texW;
                float halfH = halfW * ratio * aspect;

                glColor3f(1.0f, 1.0f, 1.0f);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, tex);

                glBegin(GL_QUADS);                 // ← s,t 坐标恢复正常顺序
                glTexCoord2f(0.f, 0.f); glVertex2f(c.x - halfW, c.y - halfH); // 左下
                glTexCoord2f(1.f, 0.f); glVertex2f(c.x + halfW, c.y - halfH); // 右下
                glTexCoord2f(1.f, 1.f); glVertex2f(c.x + halfW, c.y + halfH); // 右上
                glTexCoord2f(0.f, 1.f); glVertex2f(c.x - halfW, c.y + halfH); // 左上
                glEnd();

                glDisable(GL_TEXTURE_2D);
                break;
            }



            case PENCIL:
                glColor3f(p.getR(), p.getG(), p.getB());
                glLineWidth(p.getSize());
                glBegin(GL_LINE_STRIP);
                for (auto &pt: p.getPoints()) glVertex2f(pt.x, pt.y);
                glEnd();
                break;

            case CIRCLE: {
                glColor3f(p.getR(), p.getG(), p.getB());
                auto c = p.getPoint(0);
                glBegin(GL_POLYGON);
                for (int i = 0; i < 64; ++i) {
                    float a = i * 2.0f * M_PI / 64.0f;
                    glVertex2f(c.x + cosf(a) * u, c.y + sinf(a) * u * aspect);
                }
                glEnd();
                break;
            }

            case RECTANGLE: {
                glColor3f(p.getR(), p.getG(), p.getB());
                auto c = p.getPoint(0);
                glBegin(GL_POLYGON);
                glVertex2f(c.x - u, c.y - u * aspect);
                glVertex2f(c.x + u, c.y - u * aspect);
                glVertex2f(c.x + u, c.y + u * aspect);
                glVertex2f(c.x - u, c.y + u * aspect);
                glEnd();
                break;
            }

            case TRIANGLE: {
                glColor3f(p.getR(), p.getG(), p.getB());
                auto c = p.getPoint(0);
                glBegin(GL_POLYGON);
                glVertex2f(c.x, c.y + u * aspect);
                glVertex2f(c.x - u, c.y - u * aspect);
                glVertex2f(c.x + u, c.y - u * aspect);
                glEnd();
                break;
            }

            case POLYGON: {
                glColor3f(p.getR(), p.getG(), p.getB());
                auto c = p.getPoint(0);
                glBegin(GL_POLYGON);
                for (int i = 0; i < 5; ++i) {
                    float a = i * 2.0f * M_PI / 5.0f - M_PI / 2.0f;
                    glVertex2f(c.x + cosf(a) * u, c.y + sinf(a) * u * aspect);
                }
                glEnd();
                break;
            }

            default: break;
        }

        if (static_cast<int>(idx) == selected) {
            glColor3f(1, 0, 0);
            glLineWidth(sel_line_width);

            if (p.getTool() == PENCIL) {
                float minx = 1e9f, maxx = -1e9f, miny = 1e9f, maxy = -1e9f;
                for (auto &pt: p.getPoints()) {
                    if (pt.x < minx) minx = pt.x;
                    if (pt.x > maxx) maxx = pt.x;
                    if (pt.y < miny) miny = pt.y;
                    if (pt.y > maxy) maxy = pt.y;
                }
                glBegin(GL_LINE_LOOP);
                glVertex2f(minx - selu, miny - selu * aspect);
                glVertex2f(maxx + selu, miny - selu * aspect);
                glVertex2f(maxx + selu, maxy + selu * aspect);
                glVertex2f(minx - selu, maxy + selu * aspect);
                glEnd();
            } else {
                auto c = p.getPoint(0);
                glBegin(GL_LINE_LOOP);
                glVertex2f(c.x - u - selu, c.y - u * aspect - selu * aspect);
                glVertex2f(c.x + u + selu, c.y - u * aspect - selu * aspect);
                glVertex2f(c.x + u + selu, c.y + u * aspect + selu * aspect);
                glVertex2f(c.x - u - selu, c.y + u * aspect + selu * aspect);
                glEnd();
            }
        }
    }

    glFlush();
    swap_buffers();
}
