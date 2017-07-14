#pragma once
#include <memory>
#include <map>
#include "GL.h"
#include "RawInputProcessor.h"

class ApplicationListener{
    std::shared_ptr<RawInputProcessor> input;
public:
	virtual bool create() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void render() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void dispose() = 0;

	void setRawInputProcessor(std::shared_ptr<RawInputProcessor> input){this->input = input;};
    std::shared_ptr<RawInputProcessor> getRawInputProcessor(){return input;}
};

class Configuration{
    std::map<SDL_GLattr,int> desktopConfig,mobileConfig,universalConfig;
public:
    std::map<SDL_GLattr,int> getRawDesktopConfig(){return desktopConfig;}
    std::map<SDL_GLattr,int> getRawMobileConfig(){return mobileConfig;}
    std::map<SDL_GLattr,int> getRawUniversalConfig(){return universalConfig;}
};

class LibGDX_Application{
private:
    std::shared_ptr<ApplicationListener> listener;
    SDL_Window* window;
    SDL_GLContext glContext;
    
    void dispose();
    bool setOpenGL();
public:
	LibGDX_Application(std::shared_ptr<ApplicationListener> listener);
};
