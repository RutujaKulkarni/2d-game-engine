// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

ImageFactory*  ImageFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
   bool restart = true;
   
      try {
         while(restart){
            Engine engine;
            restart = engine.play();
         }
         delete RenderContext::getInstance();
      }
      catch (const string& msg) { std::cout << msg << std::endl; }
      catch (...) {
         std::cout << "Oops, someone threw an exception!" << std::endl;
      }
   return 0;
}
