#include "twoWaySprite.h"
#include "gameData.h"
#include "imageFactory.h"

void TwoWaySprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    frameCounter = (frameCounter+1) % (numberOfFrames/2);
    
    if(getVelocityX() > 0){
      currentFrame = frameCounter + (numberOfFrames/2);
    }else{
      currentFrame = (numberOfFrames/2) - (frameCounter) -1;
    }
    timeSinceLastFrame = 0;
  }
}

TwoWaySprite::TwoWaySprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance()->getImages(name) ),
  currentFrame(0),
  frameCounter(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwoWaySprite::TwoWaySprite(const TwoWaySprite& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  frameCounter(s.frameCounter),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

TwoWaySprite& TwoWaySprite::operator=(const TwoWaySprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void TwoWaySprite::draw() const {
    images[currentFrame]->draw(getX(), getY(), getScale()); 
}

void TwoWaySprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    //setVelocityX( -fabs( getVelocityX() ) );
  }  
  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    //setVelocityY( -fabs( getVelocityY() ) );
  }

}

bool TwoWaySprite::reachedEnd(){
  return ( getX() > worldWidth-getScaledWidth());
}

