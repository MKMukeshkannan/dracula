#include <filesystem>
#include <imgui.h>
#include <implot.h>
#include "renderer.h"
#include <rapidcsv.h>
#include <functional>

int main(int, char **) {
  Renderer main_renderer;

  rapidcsv::Document lin_reg(std::filesystem::path("examples/housing.csv"));
  std::vector<double> ages = lin_reg.GetColumn<double>(0);
  std::vector<double> prices = lin_reg.GetColumn<double>(1);

  double O1 = -22, O2 = 900;
  std::function<double(double)> hypothesis = [&](double x) { return O1 * x + O2; };


  std::vector<double> epoch_list, error_list;
  int epoch_number = 0;

  main_renderer.add_pipeline([&] {
    epoch_number++;
    double error = 0;
    if (epoch_number < 100000) {
        error = 0;
        for (int i = 0; i < ages.size(); ++i) error += pow((hypothesis(ages[i]) - prices[i]), 2);
        error /= 2 * ages.size();

        double t1 = 0, t2 = 0;
        for (int i = 0; i < ages.size(); ++i) {
          t1 += (hypothesis(ages[i]) - prices[i]) * ages[i];
          t2 += (hypothesis(ages[i]) - prices[i]);
        };
        t1 = t1 / ages.size();
        t2 = t2 / ages.size();

        O1 -= 0.001 * t1;
        O2 -= 0.001 * t2;

        epoch_list.push_back(epoch_number);
        error_list.push_back(error);
    };

    double x[] = {0, -(O2 / O1)};
    double y[] = {O2, 0};


    ImGui::Begin("Metrics");

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("EPOCH: %d", epoch_number);
    ImGui::Text("ERROR: %.1f", error);
    ImGui::Text("WEIGH: %.1f", O1);
    ImGui::Text("BIAS : %.1f", O2);

    ImGui::End();

    if (ImGui::Begin("DATA PLOT")) {
      if (ImPlot::BeginPlot("My Plot", ImVec2(600, 400))) {
        ImPlot::PlotScatter("HOUSING PLOT", ages.data(), prices.data(), ages.size());
        ImPlot::PlotLine("Line", x, y, 2);
        ImPlot::EndPlot();
      }
      ImGui::End();
    }

    if (ImGui::Begin("LOSS FUNCTION")) {
      if (ImPlot::BeginPlot("LOSS", ImVec2(600, 400))) {
        ImPlot::PlotLine("LOSS", epoch_list.data(), error_list.data(), epoch_list.size());
        ImPlot::EndPlot();
      }
      ImGui::End();
    }
  });

  main_renderer.render();

  return 0;
}
