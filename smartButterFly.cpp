#include <cmath>
#include <random>
#include <functional>
#include "smartButterFly.h"
#include "gameData.h"
#include "renderContext.h"
#include "explodingSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void SmartButterFly::goLeft()  { setVelocityX( -abs(getVelocityX()) );  }
void SmartButterFly::goRight() { setVelocityX( fabs(getVelocityX()) );  }
void SmartButterFly::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartButterFly::goDown()  { setVelocityY( fabs(getVelocityY()) );  }


SmartButterFly::SmartButterFly(const std::string& name, const Vector2f& pos, 
  int w, int h, int count) :
  /*MultiSprite(name, count),*/
Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance()->getImages(name) ),
  explosion(nullptr),
  currentFrame(count),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance")),
  dead(false)
{}


SmartButterFly::SmartButterFly(const SmartButterFly& s) : 
  /*MultiSprite(s),*/
  Drawable(s), 
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance),
  dead(s.dead)
{}

void SmartButterFly::update(Uint32 ticks) { 
//  std::cout << "Updating butterfly :  \n";
  //MultiSprite::update(ticks);

  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      std::cout << "Chunk count is zero ... \n";
      dead = true;
      delete explosion;
      explosion = NULL;
      //delete this;
    }
    return;
  }
  else{
    advanceFrame(ticks);
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);

    if ( getY() < 0) {
      setVelocityY( std::abs( getVelocityY() ) );
    }
    if ( getY() > worldHeight-getScaledHeight()) {
      setVelocityY( -std::abs( getVelocityY() ) );
    }

    if ( getX() < 0) {
      setVelocityX( std::abs( getVelocityX() ) );
    }
    if ( getX() > worldWidth-getScaledWidth()) {
      setVelocityX( -std::abs( getVelocityX() ) );
    }
    float x= getX()+getImage()->getWidth();
    float y= getY()+getImage()->getHeight();
    float ex= playerPos[0]+playerWidth/2;
    float ey= playerPos[1]+playerHeight/2;
    float distanceToEnemy = ::distance( x, y, ex, ey );

    if  ( currentMode == NORMAL ) {
      //std::cout << "NORMAL :  \n";
      if(distanceToEnemy < safeDistance) currentMode = EVADE;
    }
    else if  ( currentMode == EVADE ) {
      //std::cout << "EVADE :  \n";
      if(distanceToEnemy > safeDistance) currentMode=NORMAL;
      else {
        if ( x < ex ) goLeft();
        if ( x > ex ) goRight();
        if ( y < ey ) goUp();
        if ( y > ey ) goDown();
      }
    }
  }

}

void SmartButterFly::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

Vector2f SmartButterFly::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

SmartButterFly& SmartButterFly::operator=(const SmartButterFly& s) {
  Drawable::operator=(s);
  images = (s.images);
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );

  playerPos = (s.playerPos);
  playerWidth = (s.playerWidth);
  playerHeight = (s.playerHeight);
  currentMode = (s.currentMode);
  safeDistance = (s.safeDistance);
  dead = s.dead;

  return *this;
}

int SmartButterFly::getChunkCount(){ 
  return explosion->chunkCount();
}

bool SmartButterFly::isDead(){ 
  return dead;
}

void SmartButterFly::draw() const { 
  if ( explosion ) {
    //std::cout<< "Drawing explosion ... \n";
    explosion->draw();
  }else{
      images[currentFrame]->draw(getX(), getY(), getScale());
  }
}

void SmartButterFly::explode() {
  if ( !explosion ) {
    //std::cout << "Exploding ..." << std::endl;
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}
