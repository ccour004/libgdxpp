#pragma once
#include <memory>
#include "GL.h"
#include "InputProcessor.h"

class ApplicationListener{
    std::shared_ptr<InputProcessor> input;
public:
	virtual bool create() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void render() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void dispose() = 0;

	//Note: These are under the GDX singleton in LibGDX.
	void setInputProcessor(const std::shared_ptr<InputProcessor> input){this->input = input;};
};

int err(const char* fmt);

class LibGDX_Application{
private:
    static std::shared_ptr<ApplicationListener> listener;
    static int event_filter(void* data,SDL_Event* event);
    static SDL_Window* window;
    static SDL_GLContext glContext;
    static void dispose();
public:
	LibGDX_Application(std::shared_ptr<ApplicationListener> listener);
};
