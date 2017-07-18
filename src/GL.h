#pragma once

#ifdef DESKTOP
	#include <GL/glew.h> 
    #include <SDL_opengl.h> 
	#include <GL/glu.h>
#else
    #include <GLES3/gl3.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <memory>

template<class T, class D = std::default_delete<T>>
struct shared_ptr_with_deleter : public std::shared_ptr<T>
{
  explicit shared_ptr_with_deleter(T* t = nullptr)
      : std::shared_ptr<T>(t, D()) {}
 
  void reset(T* t = nullptr) {
    std::shared_ptr<T>::reset(t, D());
  }
};

namespace SDL2 {
    struct SDL_Deleter {
      void operator()(SDL_Surface*  ptr) { if (ptr) SDL_FreeSurface(ptr); }
      void operator()(SDL_Texture*  ptr) { if (ptr) SDL_DestroyTexture(ptr); }
      void operator()(SDL_Renderer* ptr) { if (ptr) SDL_DestroyRenderer(ptr); }
      void operator()(SDL_Window*   ptr) { if (ptr) SDL_DestroyWindow(ptr); }
      void operator()(SDL_RWops*    ptr) { if (ptr) SDL_RWclose(ptr); }
    };
     
    //Shared Pointers
    using Surface  = shared_ptr_with_deleter<SDL_Surface,  SDL_Deleter>;
    using Texture  = shared_ptr_with_deleter<SDL_Texture,  SDL_Deleter>;
    using Renderer = shared_ptr_with_deleter<SDL_Renderer, SDL_Deleter>;
    using Window   = shared_ptr_with_deleter<SDL_Window,   SDL_Deleter>;
    using RWops    = shared_ptr_with_deleter<SDL_RWops,    SDL_Deleter>;
    
    //TODO: Unique Pointers too?
}

#include "graphics/Texture.h"
namespace GDXPP{
    struct GDXPP_Deleter{
      void operator()(Texture*  ptr) { if (ptr) ptr->destroy();} 
    };
    
    //Shared Pointers
    using Texture = shared_ptr_with_deleter<Texture,GDXPP_Deleter>;
    
    //TODO: Unique Pointers too?
}

char* file_read(const char* filename);
std::string fileToString(const std::string& filename);
