#include <sstream>
#include "viewport.h"
#include "ioMod.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  gdata(Gamedata::getInstance()),
  viewPos(0, 0),
  msgPos(
    Vector2f( gdata.getXmlInt("view/loc/x"), gdata.getXmlInt("view/loc/y") )
  ),
  namePos(
  	Vector2f( gdata.getXmlInt("view/namePos/x"), gdata.getXmlInt("view/namePos/y") )
  ),
  nameColor({0xff, 0, 0, 0}),
  fpsPos(
    Vector2f( gdata.getXmlInt("view/fpsPos/x"), gdata.getXmlInt("view/fpsPos/y") ) 
  ),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")), 
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL)
{
	//pick name color from XML and put it in nameColor
	nameColor.r = Gamedata::getInstance().getXmlInt("nameFont/red");
  	nameColor.g = Gamedata::getInstance().getXmlInt("nameFont/green");
  	nameColor.b = Gamedata::getInstance().getXmlInt("nameFont/blue");
  	nameColor.a = Gamedata::getInstance().getXmlInt("nameFont/alpha");

	std::cout << "worldwidth : "<< worldWidth<<" \n";
	std::cout << "worldheight : "<< worldHeight<<" \n";
	std::cout << "viewwidth : "<< viewWidth<<" \n";
	std::cout << "viewheight : "<< viewHeight<<" \n";
}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj; 
  std::cout << "Tracking object \n";
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() const {
  IoMod::getInstance().
    //IoMod::getInstance().writeText("Tracking: "+objectToTrack->getName(), msgPos[0], msgPos[1], nameColor);
    //pass color for name
    IoMod::getInstance().writeText("Rutuja Kulkarni ", namePos[0], namePos[1], nameColor);
    //TODO:Write FPS in upper left corner
    
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  viewPos[0] = (x + objWidth/2) - viewWidth/2;
  viewPos[1] = (y + objHeight/2) - viewHeight/2;

  if (viewPos[0] < 0) viewPos[0] = 0;
  if (viewPos[1] < 0) viewPos[1] = 0;
  if (viewPos[0] > (worldWidth - viewWidth)) {
    viewPos[0] = worldWidth-viewWidth;
  }
  if (viewPos[1] > (worldHeight - viewHeight)) {
    viewPos[1] = worldHeight-viewHeight;
  }
}

/*void Viewport::update(int _fps) {

  update();
}*/
