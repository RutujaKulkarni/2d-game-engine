#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <list>
#include <cmath>
//#include "multisprite.h"
#include "twoWaySprite.h"
#include "smartButterFly.h"
#include "bulletPool.h"

class Player : public TwoWaySprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);

  virtual void draw() const;

  void collided() { collision = true; }
  void missed() { collision = false; }
  

  void right();
  void up();
  void stop();
  void down();

  void crash();


  void attach( SmartButterFly* o ) { observers.push_back(o); } 
  void detach( SmartButterFly* o );

  int getJumpSpeed() const { return jumpSpeed;}

  Player& operator=(const Player&);

  void shoot();
  virtual bool collidedWith(const Drawable*) const;

  BulletPool& getBulletPool(){ return bullets; }
  
private:
  bool collision;
  Vector2f initialVelocity;
int setYloc;
  std::list<SmartButterFly*> observers;

  const int jumpSpeed = -1;
  int vertSpeed = 0;
  const double fallingConstant = 0.00015;
  int posY;

  //bullet pool attributes
  std::string bulletName;
  BulletPool bullets;
  float minSpeed;

  bool isCrashing = false;

};
#endif
