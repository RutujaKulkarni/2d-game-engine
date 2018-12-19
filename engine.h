#include <vector>
#include <SDL.h>
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "smartButterFly.h"
#include "hud.h"
#include "sound.h"
#include "menuEngine.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();
  Engine(const Engine&)=delete;
  Engine& operator=(const Engine&)=delete;

  unsigned getButterFliesCount () const {return butterFliesCount;}
  void setButterFliesCount (unsigned i) {butterFliesCount = i;
    std::cout << "Setting count : " << butterFliesCount << "\n";}

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;
  SDL_Renderer * const renderer;  
  World layer1;
  World layer2;
  World layer3;
  World layer4;
  World layer5;
  Viewport& viewport;
  Player * player;  
  Hud& hud;
  std::vector<Drawable *> poleSprites;  
  std::vector<SmartButterFly * > smartButters;
  std::vector<CollisionStrategy * > strategies;
  int currentSprite;
  int currentStrategy;
  bool collision;
  bool makeVideo;
  bool isGameOver = false;
  void draw();
  void update(Uint32);
  void printScales() const;
  void checkForCollisions();
  bool showHud;
  bool scaleToggle = false;
  SDLSound sound;
  int scaleToggleCount = 0;

  bool godMode = false;

  MenuEngine menuEngine;
  Vector2f menuMsgLoc;

  unsigned butterFliesCount;
  bool won;
};
