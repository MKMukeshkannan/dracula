#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

class Renderer {
public:
  Renderer();
  ~Renderer();
  void render();
  void add_pipeline(std::function<void()>);

private:
  std::vector<std::function<void()>> pipelines;
  GLFWwindow* main_window;
};
