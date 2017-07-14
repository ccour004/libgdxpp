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
public:
    int depth = 16,stencil = 0;
    int gl_major_version = 3,gl_minor_version = 0;
    int multiSampleBuffer = 0,multiSampleSamples = 0;
    void setAntiAlias(){multiSampleBuffer = 1;multiSampleSamples = 2;}
    void setBetterAntiAlias(){multiSampleBuffer = 1;multiSampleSamples = 4;}
};

class DesktopConfiguration: public Configuration{
public:
    int width = 640,height = 480;
    int x = -1,y = -1;
    bool fullscreen = false,resizable = true;
    std::string title = "Test Window";
    DesktopConfiguration(){}
};

class MobileConfiguration: public Configuration{
public:
    MobileConfiguration(){}
};

class LibGDX_Application{
private:
    std::shared_ptr<ApplicationListener> listener;
    SDL_Window* window;
    SDL_GLContext glContext;
    
    void dispose();
    bool setAttributes(std::shared_ptr<DesktopConfiguration> desktop,std::shared_ptr<MobileConfiguration> mobile);
public:
	LibGDX_Application(std::shared_ptr<ApplicationListener> listener):
        LibGDX_Application(std::make_shared<DesktopConfiguration>(),std::make_shared<MobileConfiguration>(),listener){};
	LibGDX_Application(std::shared_ptr<DesktopConfiguration> desktop,std::shared_ptr<ApplicationListener> listener):
        LibGDX_Application(desktop,std::make_shared<MobileConfiguration>(),listener){};
	LibGDX_Application(std::shared_ptr<MobileConfiguration> mobile,std::shared_ptr<ApplicationListener> listener):
        LibGDX_Application(std::make_shared<DesktopConfiguration>(),mobile,listener){};
	LibGDX_Application(std::shared_ptr<DesktopConfiguration> desktop,std::shared_ptr<MobileConfiguration> mobile,
        std::shared_ptr<ApplicationListener> listener);
};
