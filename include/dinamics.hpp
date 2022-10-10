#pragma once
#include "particle.hpp"
#include "vector"

// very primitive, may need some tweaking
void handleWallCollitions(particle &p, const sf::Vector2f &size){
    const auto &pos = p.shape.getPosition();
    if ( (pos.x - p.r) <= 0 ) p.vel.x = fabs(p.vel.x);
    else if ((pos.x + p.r) >= size.x) p.vel.x = -fabs(p.vel.x);
    if ( (pos.y - p.r) <= 0) p.vel.y = fabs(p.vel.y);
    else if ((pos.y + p.r) >= size.y) p.vel.y = -fabs(p.vel.y);

    // p.vel = p.vel * Eigen::Vector2f( 1.0,-1.0);
}



// O(n^2)
// se puede reducir los calculos a la mitad con unas modificaciciones
void calculateCoulombForce(const std::vector<particle*> &particles, std::vector<sf::Vector2f> &forces){
    const std::size_t n = particles.size();

    for (size_t i = 0; i < n; i++){
        forces[i] = {0, 0};
        for (size_t j = 0; j < n; j++){
            if (j == i) continue;
            auto ds = particles.at(i)->shape.getPosition() - particles.at(j)->shape.getPosition();
            float ds_size = sqrt(ds.x*ds.x + ds.y*ds.y);
            forces[i] += particles.at(j)->q * 1/( ds_size*ds_size*ds_size ) * ds;
        }
        forces[i] *= particles.at(i)->q;
    }
}


void updatePositions(std::vector<particle*> &particles, const float &dt, std::vector<sf::Vector2f> &forces){
    // basic euler integration (no force)
    // const auto newPos = p.shape.getPosition() + p.vel * dt;
    // p.shape.setPosition( newPos.x, newPos.y );
    // (force)
    calculateCoulombForce(particles, forces);

    sf::Vector2f newPos;
    for (size_t i = 0; i < particles.size(); i++){
        auto &p = particles.at(i); 
        newPos = p->shape.getPosition() + p->vel*dt + forces.at(i) * float(0.5*dt*dt/(p->m));
        p->shape.setPosition( newPos );
    }
}