#include <vector>
#include <SDL.h>
#include <SDL2/SDL_mixer.h>

class SDLSound {
public:
  SDLSound();
  ~SDLSound();

  void startMusic();
  void stopMusic();      
  void toggleMusic();    
  void operator[](int);  
private:
  int volume;
  int currentSound;

  int audioRate;
  int audioChannels;
  int audioBuffers;
  std::vector<Mix_Chunk*> sounds;
  std::vector<int> channels;
  SDLSound(const SDLSound&);
  SDLSound& operator=(const SDLSound&);
};

/*
/home/rutu/myProjects/6/rutujak/flappyBird/assets/sound/sfx_die.wav
/home/rutu/myProjects/6/rutujak/flappyBird/assets/sound/sfx_hit.wav
/home/rutu/myProjects/6/rutujak/flappyBird/assets/sound/sfx_point.wav
/home/rutu/myProjects/6/rutujak/flappyBird/assets/sound/sfx_swooshing.wav
/home/rutu/myProjects/6/rutujak/flappyBird/assets/sound/sfx_wing.wav
/home/rutu/myProjects/6/rutujak/flappyBird/assets/sound/redbird_yell01.wav
*/