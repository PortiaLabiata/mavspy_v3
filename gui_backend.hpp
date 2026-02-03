#ifndef GUI_BACKEND_H
#define GUI_BACKEND_H

#include <optional>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace ui {

class gui_t {
public:
    gui_t(ImVec2 dims);
    ~gui_t();

    void begin_frame();
    void end_frame();
    bool should_close() {
        return glfwWindowShouldClose(window);
    }

    bool is_ok() { return ok; }
    bool is_err() { return !ok; }
private:
    bool ok = false;

    GLFWwindow *window;
    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

}

#endif
