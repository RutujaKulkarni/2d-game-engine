#include "hud.h"

Hud& Hud::getInstance(){
	std::cout << "getting HUD instance \n";
	static Hud instance;
	return instance;
}

Hud::Hud() :
	_l1 (Gamedata::getInstance().getXmlStr("hud/l1")),
	_l2 (Gamedata::getInstance().getXmlStr("hud/l2")),
	_l3 (Gamedata::getInstance().getXmlStr("hud/l3")),
	_l4 (Gamedata::getInstance().getXmlStr("hud/l4")),
	_l5 (Gamedata::getInstance().getXmlStr("hud/l5")),
	_l6 (Gamedata::getInstance().getXmlStr("hud/l6")),
	_l7 (Gamedata::getInstance().getXmlStr("hud/l7")),
	_h (Gamedata::getInstance().getXmlInt("hud/h")),
	_w (Gamedata::getInstance().getXmlInt("hud/w")),
	_r (Gamedata::getInstance().getXmlInt("hud/color/r")),
	_g (Gamedata::getInstance().getXmlInt("hud/color/g")),
	_b (Gamedata::getInstance().getXmlInt("hud/color/b")),
	_t (Gamedata::getInstance().getXmlInt("hud/color/t")),
	_x (Gamedata::getInstance().getXmlInt("hud/loc/x")),
	_y (Gamedata::getInstance().getXmlInt("hud/loc/y")),
	trans (Gamedata::getInstance().getXmlBool("hud/transparency")),
	Rect({_x,_y,_w,_h}),
    renderer( RenderContext::getInstance()->getRenderer() ),
    hudTextColor({106,90,205,255})
  {
  	std::cout << "Initializing HUD... \n";
}

void Hud::draw() const {
    
    std::stringstream sStream;
    sStream << "FPS: " << Clock::getInstance().getFps();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    SDL_SetRenderDrawColor( renderer, _r, _g, _b, _t );
    
    SDL_RenderFillRect( renderer, &Rect );
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderDrawRect( renderer, &Rect);

    
    
    IoMod::getInstance().writeText(_l1, _x+15, _y+15*5, hudTextColor);
    IoMod::getInstance().writeText(_l2, _x+15, _y+15*9, hudTextColor);  
    IoMod::getInstance().writeText(sStream.str(),_x+15,_y+15*13, hudTextColor);                             
    IoMod::getInstance().writeText(_l5, _x+15, _y+15*17, hudTextColor);
    IoMod::getInstance().writeText(_l4, _x+15, _y+15*21, hudTextColor);
    IoMod::getInstance().writeText(_l3, _x+15, _y+15*25, hudTextColor);
    
    IoMod::getInstance().writeText(_l6, _x+15, _y+15*29, hudTextColor);
    IoMod::getInstance().writeText(_l7, _x+15, _y+15*33, hudTextColor);

}
