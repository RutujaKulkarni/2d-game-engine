#include <iostream>
#include "world.h"
#include "imageFactory.h"

World::World(const std::string& name, int fact) : 
  image( ImageFactory::getInstance()->getImage(name) ),
  factor(fact),
    worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
    imageWidth( image->getWidth() ),
    viewX(0.0), viewY(0.0), 
    view(Viewport::getInstance()) 
{ 
	//std::cout<<"creating world "<< name <<" "<<factor<<"\n";
}

void World::update() {
	//std::cout<<"updating world "<<factor<<"\n";
  viewX = static_cast<int>(view.getX() / factor) % imageWidth;
  viewY = view.getY();
}

void World::draw() const { 
	//std::cout<<"drawing world "<<factor<<"\n";
  image->draw(0,0,-viewX,-viewY);
  image->draw(0,0,imageWidth-viewX,-viewY);
}

