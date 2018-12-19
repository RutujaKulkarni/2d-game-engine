#ifndef HUD_H
#define HUD_H

#include <iostream>
#include <sstream>
#include <string>
#include <SDL.h>
#include "ioMod.h"
#include "clock.h"
#include "renderContext.h"
#include "drawable.h"
#include "gameData.h"

class Hud : public IoMod{
public:
	static Hud& getInstance();
	
	void draw() const;
	
	~Hud(){}

private:
	Hud& operator=(const Hud&)=delete;
	Hud(const Hud&)=delete;
	Hud();

	std::string _l1;
	std::string _l2;
	std::string _l3;
	std::string _l4;
	std::string _l5;

	std::string _l6;
	std::string _l7;

	int _h;
	int _w;

	int _r;
	int _g;
	int _b;
	int _t;

	int _x;
	int _y;

	bool trans;

	SDL_Rect Rect;
  	SDL_Renderer* renderer;

  	SDL_Color hudTextColor;
};

#endif