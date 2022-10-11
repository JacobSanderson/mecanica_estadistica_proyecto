#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

// #include "dinamics.hpp"
// #include "particle.hpp"

#include <implot/implot.h>
#include <random>
#include <string>
#include <vector>

// globals


int main() {
  sf::RenderWindow window(sf::VideoMode(1366, 768), "Tarea Estadistica",
                          sf::Style::Fullscreen);
  // window.
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  // otras cosas
  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    ImGui::SFML::Update(window, deltaClock.getElapsedTime());

    const std::size_t num = 100;
    // static float xs1[num], ys1[num];

    std::vector<float> xs1,ys1;
    xs1.resize(num);
    ys1.resize(num);

    for (size_t i = 0; i < num; i++) {
      xs1[i] = float(i)*0.5;
      ys1[i] = sin( xs1[i] + 0.1*deltaClock.getElapsedTime().asSeconds() );
    }
    
    ImPlot::CreateContext();
    ImPlot::ShowDemoWindow();
    ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit;
    if (ImPlot::BeginPlot("Line Plots")) {
        ImPlot::SetupAxes("x","y", flags, flags);
        ImPlot::SetupAxesLimits(0,num*0.5,-1.0,1.0);
        ImPlot::PlotLine("f(x)", xs1.data(), ys1.data(), num);
        ImPlot::EndPlot();
    }
    ImPlot::DestroyContext();


    window.clear();

    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}