#ifndef SMARTBUTTERFLY__H
#define SMARTBUTTERFLY__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
//#include "multisprite.h"

class ExplodingSprite;

class SmartButterFly : public Drawable {
public:
  SmartButterFly(const std::string&, const Vector2f& pos, int w, int h, int count);
  SmartButterFly(const SmartButterFly&);
  virtual ~SmartButterFly() { } 

  //virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

    virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

  virtual void explode();
  int getChunkCount();
  bool isDead();

  ExplodingSprite* getExplosion(){
    return explosion;
  }

private:

  std::vector<Image *> images;
  
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  SmartButterFly& operator=(const SmartButterFly&);

  Vector2f makeVelocity(int, int) const;

  enum MODE {NORMAL, EVADE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();

  bool dead;
};
#endif