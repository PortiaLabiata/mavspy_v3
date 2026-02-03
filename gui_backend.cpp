#include "gui_backend.hpp"

namespace ui {

gui_t::gui_t(ImVec2 dims) {
    if (!glfwInit()) {
        return;
    }

    window = glfwCreateWindow(dims.x, dims.y, 
            "MAVSpy v3.0", nullptr, nullptr);
    if (!window)
        return;
    glfwMakeContextCurrent(window);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void gui_t::begin_frame() {
    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	glfwPollEvents();  
}

void gui_t::end_frame() {
    ImGui::Render();

	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	glClearColor(clear_color.x*clear_color.w, 
            clear_color.y*clear_color.w, 
            clear_color.z*clear_color.w, 
            clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

gui_t::~gui_t() {
    ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

}
