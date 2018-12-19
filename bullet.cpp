#include <iostream>
#include <cmath>
#include "bullet.h"
#include "collisionStrategy.h"

void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  TwoWaySprite::update(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  //TODO
  //std::cout << "Bullet incr : " << incr[0] << "        " << incr[1] << "\n";
  setPosition(getPosition() + incr);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}

