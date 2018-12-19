#include "player.h"

Player::Player( const std::string& name) :
  TwoWaySprite(name),
  collision(false),
  initialVelocity(getVelocity()),
  setYloc(0),
  observers(),
  posY(TwoWaySprite::getY()),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets( bulletName ),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  isCrashing(false)
{ std::cout << "IN PLAYER CONSTRUCT \n";}

Player::Player(const Player& s) :
  TwoWaySprite(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  setYloc(0),
  observers( s.observers ),
  posY(s.getY()),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  isCrashing(s.isCrashing)
  { std::cout << "IN PLAYER COPY \n"; }

Player& Player::operator=(const Player& s) {
  std::cout << "IN PLAYER ASSIGN \n";
  TwoWaySprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  setYloc=s.setYloc;
  isCrashing = s.isCrashing;
  return *this;
}

void Player::stop() { 
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void Player::down() { 
  //setVelocity( Vector2f(0, 0) );
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( -initialVelocity[1] );
    setY(getY()+4);
  }
}

void Player::crash() { 
  isCrashing = true;
  //setVelocity( Vector2f(0, 0) );
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( -initialVelocity[1] );
    setY(getY()+8);
  }
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    setX(getX() + 3);
  }
}  
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -fabs(initialVelocity[1]) );
    setYloc=getY();
    setY(getY()-3);
  }

  /*vertSpeed = jumpSpeed;*/
  
} 

void Player::detach( SmartButterFly* o ) {
  std::list<SmartButterFly*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::update(Uint32 ticks) { 

  if(!isCrashing){
    TwoWaySprite::advanceFrame(ticks); //first advance frame  
  }

  int incr = getVelocityY() * static_cast<float>(ticks) * 0.0001;
  setY(getY() + incr);

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


  std::list<SmartButterFly*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    int offset = rand()%5;
    offset = offset*(rand()%2?-1:1);
    const Vector2f off(offset, offset);
    (*ptr)->setPlayerPos( getPosition()+off );
    ++ptr;
  }

  bullets.update(ticks);

}

void Player::shoot() { 
  float x = getX()+getScaledWidth()/2;
  float y = getY()+getScaledHeight()/2;
  // I'm not adding minSpeed to y velocity:
  bullets.shoot( Vector2f(x, y), 
    Vector2f(minSpeed+getVelocityX(), 0)
  );
}


bool Player::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}

void Player::draw() const { 
  TwoWaySprite::draw();
  bullets.draw();
}
