#include "imgui-SFML.h"
#include "imgui.h"
#include "implot/implot.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


#include "dinamics.hpp"
#include "particle.hpp"
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/format.h>
#include <random>
#include <string>
#include <vector>

// globals
std::size_t noOfParticles = 200;
std::vector<particle *> particles;
std::vector<sf::Vector2f> forces(noOfParticles);
std::vector<float> potentials(noOfParticles);

std::vector<float> meansDsts;
std::vector<float> times;
float coeffDifusion = 1.0;
float temperature = 293.0;
const float kB = 1.0;
float dt = 0.1;
const float pad = 15.0f;
const float r = 3.0;

box bBox;

auto out = fmt::output_file("output.txt"); // fmt/os.h

// del dia 15 de septiembre 2022
void update(const sf::RenderWindow &w) {
  float meanDistance = 0;
  updatePositions(particles, dt, forces, potentials, meanDistance);
  meansDsts.push_back(meanDistance);
  if (times.size() == 0) times.push_back(0);
  else times.push_back( times.at( times.size()-1 ) + dt);

  for (auto &p : particles)
    handleWallCollitions(*p, bBox);
}

std::random_device rd{};
std::mt19937 generator{rd()};

std::vector<float> getRandomNumGauss(const std::size_t size,
                                     const float min = 0.0f,
                                     const float max = 1.0f) {
  // std::normal_distribution<float> ndist(mean, std);
  std::uniform_real_distribution<float> ndist(min, max);
  if (size < 1)
    throw "we only allow for size > 0";

  std::vector<float> res(size);
  for (auto &element : res)
    element = static_cast<float>(ndist(generator));
  return res;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(1366, 768), "Tarea Estadistica",
                          sf::Style::Fullscreen);
  // window.
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  // inicializando caja
  const auto size = window.getSize();
  bBox = box(pad, pad, (size.x - pad * 2.0) * 0.5, size.y - pad * 2.0);

  // inicializando las particulas
  const float margin = 5.0;
  for (size_t i = 0; i < noOfParticles; i++) {
    std::vector<float> random = getRandomNumGauss(2, 0.0, 1.0);
    sf::Vector2f ipos =
        sf::Vector2f(random.at(0) * bBox.w * 0.95 * 0.5 + bBox.x + margin,
                     random.at(1) * bBox.h * 0.95 + bBox.y + margin);

    particles.push_back(new particle(
        ipos, {random[0] * 2. - 1., random[1] * 2 - 1}, r, 1.0, 1.0));
    particles.at(particles.size() - 1)
        ->shape.setOrigin(r, r); 
  }

  // otras cosas
  bool actualizar = true;
  sf::Clock deltaClock;
  auto iTime = deltaClock.getElapsedTime();
  sf::Time fTime;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    ImGui::SFML::Update(window, deltaClock.getElapsedTime());

    // ImGui::ShowDemoWindow();
    ImGui::Text("Tamaño de la pantalla : (%d, %d)", size.x, size.y);
    ImGui::Text("Numero de Particulas : %ld", noOfParticles);
    ImGui::Text("time elapsed : %f s",
                deltaClock.getElapsedTime().asSeconds());

    ImGui::SliderFloat("dt", &dt, 0.01, 3.0);
    ImGui::Checkbox("update", &actualizar);

    if (ImGui::TreeNode("particles")) {
      static ImGuiTableFlags flags =
          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
      const int noOfCols = 4;
      if (ImGui::BeginTable("particles all", noOfCols, flags)) {
        // Display headers so we can inspect their interaction with borders.
        // (Headers are not the main purpose of this section of the demo, so we
        // are not elaborating on them too much. See other sections for details)
        ImGui::TableSetupColumn("pointer");
        ImGui::TableSetupColumn("position");
        ImGui::TableSetupColumn("velocity");
        ImGui::TableSetupColumn("color");
        ImGui::TableHeadersRow();

        for (const auto &p : particles) {
          ImGui::TableNextRow();
          std::string s;

          ImGui::TableSetColumnIndex(0);
          s = fmt::format("{}", fmt::ptr(p));
          ImGui::TextUnformatted(s.c_str());
          ImGui::TableSetColumnIndex(1);
          s = fmt::format("({:08.5}, {:08.5})", p->shape.getPosition().x,
                          p->shape.getPosition().y);
          ImGui::TextUnformatted(s.c_str());
          ImGui::TableSetColumnIndex(2);
          s = fmt::format("({:08.5}, {:08.5})", p->vel.x, p->vel.y);
          ImGui::TextUnformatted(s.c_str());
          ImGui::TableSetColumnIndex(3);
          const auto &col = p->color;
          s = fmt::format("({},{},{},{})", col.r, col.g, col.b, col.a);
          ImGui::TextUnformatted(s.c_str());
        }
        ImGui::EndTable();
      }
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("forces")) {
      static ImGuiTableFlags flags =
          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
      if (ImGui::BeginTable("forces all", 2, flags)) {
        ImGui::TableSetupColumn("index");
        ImGui::TableSetupColumn("force");
        ImGui::TableHeadersRow();
        for (size_t i = 0; i < forces.size(); i++) {
          ImGui::TableNextRow();
          std::string s;

          ImGui::TableSetColumnIndex(0);
          s = fmt::format("{}", i);
          ImGui::TextUnformatted(s.c_str());
          ImGui::TableSetColumnIndex(1);
          s = fmt::format("({:08.5}, {:08.5})", forces.at(i).x, forces.at(i).y);
          ImGui::TextUnformatted(s.c_str());
        }
        ImGui::EndTable();
      }
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Estadisticas")) {
      ImPlot::CreateContext();
      ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit;
      if (ImPlot::BeginPlot("Line Plots")) {
          ImPlot::SetupAxes("time","average distance", flags, flags);
          ImPlot::SetupAxesLimits(0, 500, 0 , 400);
          ImPlot::PlotLine("f(x)", times.data(), meansDsts.data(), meansDsts.size());
          ImPlot::EndPlot();
      }
      ImPlot::DestroyContext();
      ImGui::TreePop();
    }

    double energy;
    if (actualizar || ImGui::Button("Step")) {
      update(window);
      energy = calculateAllEnergy(particles, potentials);
    }
    ImGui::Text("Total energy : %f", energy);

    fTime = deltaClock.getElapsedTime();
    auto dtime = fTime - iTime;
    ImGui::Text("fps : %.2f", 1 / float(dtime.asSeconds()));
    iTime = fTime;

    // dibujando todo
    window.clear();
    window.draw(bBox.shape);
    for (auto &pt : particles) {
      window.draw(pt->shape);

      // esto estaba para debuggear
      // auto pos = pt->shape.getPosition();
      // sf::Vertex line[] = {sf::Vertex(pos),
      //                      sf::Vertex(sf::Vector2f(pos.x + pt->r, pos.y))};
      // window.draw(line, 2, sf::Lines);
    }
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}