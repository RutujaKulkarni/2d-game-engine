#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "twoWaySprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"

Engine::~Engine() { 
  for(auto e : poleSprites) 
  {
    delete e;
  }
  for(auto e : smartButters) 
  {
    delete e;
  }
  for(auto e : strategies) 
  {
    delete e;
  }  
  delete player;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  layer1("layer1", Gamedata::getInstance().getXmlInt("layer1/factor") ),
  layer2 ("layer2", Gamedata::getInstance().getXmlInt("layer2/factor") ),
  layer3 ("layer3", Gamedata::getInstance().getXmlInt("layer3/factor") ),
  layer4 ("layer4", Gamedata::getInstance().getXmlInt("layer4/factor") ),
  layer5("layer5", Gamedata::getInstance().getXmlInt("layer5/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player( ("red") )),
  hud( Hud::getInstance() ),
  poleSprites(),
  smartButters(),
  strategies(),
  currentSprite(0),
  currentStrategy(0),
  collision(false),
  makeVideo( false ),
  isGameOver(false),
  showHud(false),
  scaleToggle(false),
  sound(),
  scaleToggleCount(0),
  godMode(false),
  menuEngine(),
  menuMsgLoc(
    Vector2f( Gamedata::getInstance().getXmlInt("menu/msg/x"), 
              Gamedata::getInstance().getXmlInt("menu/msg/y") )
  ),
  butterFliesCount(0),
  won(false)
{ 
  int poleCountFromXML = Gamedata::getInstance().getXmlInt("poles/count");
  std :: cout <<"Pole count : " << poleCountFromXML << "\n";
  poleSprites.reserve(poleCountFromXML); //add to XML
  strategies.reserve(1); //add to XML
  int multiplicationFactor = viewport.getWorldWidth() / poleCountFromXML;

  for(int poles = 0; poles < poleCountFromXML; poles++){
    int poleCount = (poles % 15) +1 ;
    std::string s = "P"+ std::to_string(poleCount);
    std::cout << " ... " << s << "\n";
    Sprite* tempSprite = new Sprite((s));
    tempSprite->setX(multiplicationFactor*(poles+1));
    poleSprites.emplace_back(tempSprite);
    //delete tempSprite;
    //TODO: delete tempSprite
  }

  strategies.emplace_back( new PerPixelCollisionStrategy );
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();

    smartButters.reserve(Gamedata::getInstance().getXmlInt("ButterFly/count")); //add to XML
    for (int i = 0; i < Gamedata::getInstance().getXmlInt("ButterFly/count"); i++){ //read number from XML
      smartButters.push_back(new SmartButterFly("ButterFly", pos, w, h, i%7)); //because, number of frames in butterfly sprites are 7
      player->attach(smartButters[i]);
    }

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() /*const*/ {

  layer1.draw();
  layer2.draw();
  layer3.draw();
  layer4.draw();
  layer5.draw();
for(auto* e: poleSprites){
  e->draw();
}
player->draw();
std::stringstream stream;
SDL_Color color ;
  color.r = Gamedata::getInstance().getXmlInt("font/red");
  color.g = Gamedata::getInstance().getXmlInt("font/green");;
  color.b = Gamedata::getInstance().getXmlInt("font/blue");
  color.a = Gamedata::getInstance().getXmlInt("font/alpha");
  stream << "Enemies: " << smartButters.size();
  IoMod::getInstance().
    writeText(stream.str(), 1100, 90, color);
  stream.clear();
  stream.str("");
  //stream << "Bullet pool: " << freeList.size();
  //IoMod::getInstance().writeText(stream.str(), 500, 60);
for(auto* s: smartButters){
  s->draw();
}
  if(showHud){
    hud.draw();
  }
  if(isGameOver){
    stream << "GAME OVER";
    IoMod::getInstance().
      writeText(stream.str(), 750, 270, color);
    stream.clear();
    stream.str("");
    clock.pause();
  }
  else if(won){
    stream << "GAME WON";
    IoMod::getInstance().
      writeText(stream.str(), 750, 270, color);
    stream.clear();
    stream.str("");
    clock.pause();
  }
  io.writeText("Press 'm' for menu", menuMsgLoc[0], menuMsgLoc[1], color);
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  if(!isGameOver){
    checkForCollisions();
  }
  if(!showHud){
    player->update(ticks);
  }

  for(auto* s: smartButters){
    s->update(ticks);
  }
  layer1.update();
  layer2.update();
  layer3.update();
  layer4.update();
  layer5.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 7;
  Viewport::getInstance().setObjectToTrack(player);
}

bool Engine::play() {

  if ( clock.isPaused() ) clock.unpause();
	//std::cout << "In engine Play ..\n" ;
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] && !isGameOver) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_M] || keystate[SDL_SCANCODE_O] ) {
          clock.pause();
          menuEngine.play();
          if ( menuEngine.starsOptionChosen() ) {
            butterFliesCount = menuEngine.getNumStars();
            std::cout << "No of enemies: " 
                      << menuEngine.getNumStars() 
                      << std::endl;
          }
          if(butterFliesCount != smartButters.size()){
            std::cout << "Need to make count same \n";
            if(butterFliesCount < smartButters.size()){
                auto itr = smartButters.begin();
                SmartButterFly* butterfly;
                while ( itr != smartButters.end() && (butterFliesCount != smartButters.size()) ) {
                  butterfly = *itr;
                  player->detach(butterfly);
                  itr = smartButters.erase(itr);                  
                  delete butterfly;
                }
            }
            else{
                //add butterflies
              Vector2f pos = player->getPosition();
              int w = player->getScaledWidth();
              int h = player->getScaledHeight();
                for (unsigned i = smartButters.size(); i < (butterFliesCount); i++){ 
                  smartButters.push_back(new SmartButterFly("ButterFly", pos, w, h, i%7)); //because, number of frames in butterfly sprites are 7
                  player->attach(smartButters[i]);
                }
            }
          }

          clock.unpause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_R] ) {//for restarting the game
          //std::cout << "returning : " << butterFliesCount << "\n";
          return true;
        }//setScale
        if ( keystate[SDL_SCANCODE_S] ) {//for restarting the game
        	scaleToggle = !scaleToggle;
          sound[3]; 
          player->setScale(scaleToggle ? 0.5 : 1.2 );
        }
        if(scaleToggle){
          scaleToggleCount++;
          if(scaleToggleCount > 40){
            std::cout << "RESETTING THE SCALE \n";
            sound[3]; 
            scaleToggle = false;
            player->setScale(scaleToggle ? 0.5 : 1.2 ); 
            scaleToggleCount = 0;
          }
        }

        if ( keystate[SDL_SCANCODE_G] ) {
          godMode = !godMode;
        }

        if ( keystate[SDL_SCANCODE_SPACE] && !isGameOver) {
          player->shoot();
          sound[5];//golden egg
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }

        if (keystate[SDL_SCANCODE_F1] ) {
          //std::cout << "Displaying HUD" << std::endl;
          showHud = !showHud;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if(!showHud){
        if(!isGameOver){
          if (keystate[SDL_SCANCODE_W]) {
            static_cast<Player*>(player)->right();
            static_cast<Player*>(player)->up();
          }else{
            static_cast<Player*>(player)->down();
          }
        }else{
          static_cast<Player*>(player)->crash();
        }
        if((ticks % 100) > 90){
          scaleToggle = !scaleToggle; 
          player->setScale(scaleToggle ? 0.5 : 1.2 );
        }
      }

      //check if bird reached the lower end
      draw();
      update(ticks);
      //after certain ticks, set bird scale to normal
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  std::cout << "Reached till end ... \n";
  return false; //do not restart
}

void Engine::checkForCollisions(/*Uint32 ticks*/){
  //check collision with butterflies
  auto itr = smartButters.begin();
  //get out og the loop once collision is detected
  SmartButterFly* butterfly;
  while ( itr != smartButters.end() ) {
	butterfly = *itr;

	if(player->getBulletPool().collidedWith(*itr)){
        player->detach(butterfly);
      	butterfly->explode();
    }

	if(butterfly->isDead()){
    	itr = smartButters.erase(itr);
      sound[2];
      delete butterfly;
      //std::cout << "erased butterfly \n";
    }
    else {++itr; }    
  }

  //check for collision with pipes
  auto poleItr = poleSprites.begin();
  while ( poleItr != poleSprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **poleItr) ) {
      //std::cout << "GAME OVER\n";
      if(!godMode){sound[6];}
      isGameOver = godMode ? false : true;
      draw();
      break;
    }
    else ++poleItr;
  }

  if(player->getY() > (viewport.getWorldHeight() - player->getScaledHeight() )){
    std::cout << "Bird reached end \n";
    isGameOver = godMode ? false : true;
    //clock.pause();
  }

  if(player->reachedEnd()){
    //clock.pause();
    if(godMode){
      won = true;
      std::cout << "GAME WON!!!!! \n";
    }else{
      if(smartButters.size() == 0){
        won = true;
      }else{
        isGameOver = true;
      }
    }
    
    //GAME WON
  }
  //check for collisions with enemies = butterflies
}