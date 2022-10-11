#pragma once
#include "particle.hpp"
#include "vector"

// very primitive, may need some tweaking
void handleWallCollitions(particle &p, const box &bBox) {
  const auto &pos = p.shape.getPosition();

  // // si colisiona se pintara verde
  // p.shape.setFillColor({0,255,0,255});

  if ((pos.x - p.r) < bBox.x) {
    p.vel.x = fabs(p.vel.x);
    p.shape.setFillColor({0, 120, 0, 255});
  } else if ((pos.x + p.r) > (bBox.w + bBox.x)) {
    p.vel.x = -fabs(p.vel.x);
    p.shape.setFillColor({0, 255, 0, 255});
  } else {
    p.shape.setFillColor({255, 0, 0, 255});
  }

  if ((pos.y - p.r) < bBox.y) {
    p.vel.y = fabs(p.vel.y);
    p.shape.setFillColor({0, 0, 120, 255});
  } else if ((pos.y + p.r) > (bBox.h + bBox.y)) {
    p.vel.y = -fabs(p.vel.y);
    p.shape.setFillColor({0, 0, 255, 255});
  } else {
    p.shape.setFillColor({255, 0, 0, 255});
  }
}

// O(n^2)
// se puede reducir los calculos a la mitad con unas modificaciciones
void calculateCoulombForce(const std::vector<particle *> &particles,
                           std::vector<sf::Vector2f> &forces,
                           std::vector<float> &potentials,
                           float &meanDistance) {
  const std::size_t n = particles.size();
  meanDistance = 0;
  std::vector<float> means(n);
  for (size_t i = 0; i < n; i++) {
    forces[i] = {0, 0};
    potentials[i] = 0;
    means[i] = 0;
    for (size_t j = 0; j < n; j++) {
      if (j == i)
        continue;
      auto ds = particles.at(i)->shape.getPosition() -
                particles.at(j)->shape.getPosition();
      float ds_size = sqrt(ds.x * ds.x + ds.y * ds.y);
      means[i] += ds_size;
      potentials[i] += particles.at(j)->q * 1 / (ds_size);
      forces[i] += potentials[i] * 1 / (ds_size * ds_size) * ds;
    }
    means[i] /= float(n*(n-1));
    forces[i] *= particles.at(i)->q;
    potentials[i] *= particles.at(i)->q;
  }

  for (auto &mean: means)
    meanDistance += mean;
  
}

double calculateAllEnergy(const std::vector<particle *> &particles,
                          const std::vector<float> &potentials) {
  double k = 0;
  for (auto &p : particles)
    k += p->getKineticEnergy();
  for (auto &p : potentials)
    k += p;
  return k;
}

void updatePositions(std::vector<particle *> &particles, const float &dt,
                     std::vector<sf::Vector2f> &forces,
                     std::vector<float> &potentials,
                     float &meanDistance) {
  // basic euler integration (no force)
  // const auto newPos = p.shape.getPosition() + p.vel * dt;
  // p.shape.setPosition( newPos.x, newPos.y );
  // (force)
  calculateCoulombForce(particles, forces, potentials, meanDistance);

  sf::Vector2f newPos;
  for (size_t i = 0; i < particles.size(); i++) {
    auto &p = particles.at(i);
    newPos = p->shape.getPosition() + p->vel * dt +
             forces.at(i) * float(0.5 * dt * dt / (p->m));
    p->shape.setPosition(newPos);
  }
}