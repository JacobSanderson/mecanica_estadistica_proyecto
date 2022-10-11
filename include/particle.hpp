#pragma once

#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

// using namespace Eigen;

struct particle {
  sf::Vector2f vel;
  float r;
  float m;
  float q;
  sf::Color color;
  sf::CircleShape shape;

  particle(sf::Vector2f position, sf::Vector2f velocity, float radius = 0.1f,
           float mass = 1.0f, float charge = 1.0f, sf::Color pColor = {255, 0, 0, 255}) {
    vel = velocity;
    r = radius;
    color = pColor;
    m = mass;
    q = charge;

    shape = sf::CircleShape(radius, 20);
    shape.setPosition(position.x, position.y);
    shape.setFillColor(pColor);
    shape.setRadius(radius);
  }

  particle(const particle &other) {
    this->shape = other.shape;
    this->vel = other.vel;
    this->r = other.r;
    this->color = other.color;
  }

  ~particle() {
    delete &vel;
    delete &r;
    delete &color;
    delete &shape;
  }

  void print() {
    fmt::print("ptr = {}\t", fmt::ptr(this));
    fmt::print("vel = ({},{})\t", vel.x, vel.y);
    fmt::print("shape = [col:({},{},{},{}), radius:{}, pos:({},{})]\n",
               shape.getFillColor().r, shape.getFillColor().g,
               shape.getFillColor().b, shape.getFillColor().a,
               shape.getRadius(), shape.getPosition().x, shape.getPosition().y);
  }

  float getKineticEnergy() { return 0.5 * m * (vel.x*vel.x + vel.y*vel.y); }
};

struct box {
  float x;
  float y;
  float w;
  float h;
  sf::RectangleShape shape;

  box(float x, float y, float width, float height) : x{x}, y{y}, w{width}, h{height} {
    shape = sf::RectangleShape({w,h});
    // shape.setOrigin(x,y);
    shape.move( x, y );
    shape.setOutlineColor({255,255,255,255});
    shape.setFillColor({255,255,255,0});
    shape.setOutlineThickness( 1.0 );
  }

  box(){ }

};
