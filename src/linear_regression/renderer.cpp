#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui.h"
#include "implot.h"
#include "renderer.h"
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Renderer::Renderer() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return;

  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

  this->main_window = glfwCreateWindow(1280, 720, "C++ MACHINE LEARNING", nullptr, nullptr);
  if (this->main_window == nullptr) return ;
  glfwMakeContextCurrent(this->main_window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsLight();

  ImGui_ImplGlfw_InitForOpenGL(this->main_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

};

Renderer::~Renderer() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  glfwDestroyWindow(this->main_window);
  glfwTerminate();
};

void Renderer::render() {
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  while (!glfwWindowShouldClose(this->main_window)) {

    glfwPollEvents();
    if (glfwGetWindowAttrib(this->main_window, GLFW_ICONIFIED) != 0)
      continue;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // DRIVER CODE {
      
    for (auto render_function: this->pipelines) {
      render_function();
    };

    // } END DRIVER CODE

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(this->main_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->main_window);
  }
};

void Renderer::add_pipeline(std::function<void()> render_function) {
  this->pipelines.push_back(render_function);
};
