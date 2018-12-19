#ifndef VIEWPORT__H
#define VIEWPORT__H
#include "drawable.h"
#include "gameData.h"

class Viewport {
public:
  static Viewport& getInstance();
  void draw() const;
  void update();
  /*void update(int _fps);*/

  Vector2f getPosition() const { return viewPos; }
  float getX() const  { return viewPos[0]; }
  void  setX(float x) { viewPos[0] = x; }
  float getY() const  { return viewPos[1]; }
  void  setY(float y) { viewPos[1] = y; }

  int getWorldHeight() const { return worldHeight;}
  int getWorldWidth() const { return worldWidth;}

  int getViewHeight() const { return viewHeight;}
  int getViewWidth() const { return viewWidth;}

  void setObjectToTrack(const Drawable *obj);
  const Drawable* getObjectToTrack() const { return objectToTrack; } 


private:
  const Gamedata& gdata;
  Vector2f viewPos;
  Vector2f msgPos;

  //vector for my name's position
  Vector2f namePos;
  SDL_Color nameColor;

  Vector2f fpsPos;

  int worldWidth;
  int worldHeight;
  int viewWidth;
  int viewHeight;
  int objWidth;
  int objHeight;
  
  

  const Drawable *objectToTrack;

  Viewport();
  Viewport(const Viewport&);
  Viewport& operator=(const Viewport&);
};
#endif
