#pragma once
#include "particle.hpp"

// very primitive, may need some tweaking
void handleWallCollitions(particle &p, const sf::Vector2f &size){
    const auto &pos = p.shape.getPosition();
    if ( pos.x <= 0 || pos.x >= size.x)
        p.vel.x *= -1;
    if ( pos.y <= 0 || pos.y >= size.y)
        p.vel.y *= -1;
        // p.vel = p.vel * Eigen::Vector2f( 1.0,-1.0);
}

void updatePosition(particle &p, const float &dt){
    const auto newPos = p.shape.getPosition() + p.vel * dt;
    p.shape.setPosition( newPos.x, newPos.y );
}