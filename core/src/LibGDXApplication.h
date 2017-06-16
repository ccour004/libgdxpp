#pragma once

class ApplicationListener{
public:
	virtual bool create() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void render() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void dispose() = 0;
};

int err(const char* fmt){
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,fmt, SDL_GetError());
    //dispose();
    return 1;
}

class LibGDX_Application{
private:
    ApplicationListener* listener;
    SDL_Window* window;
    SDL_GLContext glContext;
    void dispose(){
	   listener->dispose();
	   SDL_Log("~~STOP SDL~~");
           SDL_GL_DeleteContext(glContext);
	   SDL_DestroyWindow(window);
	   SDL_Quit();
	}
public:
	LibGDX_Application(ApplicationListener* listener){
		this->listener = listener;
		//Initialization flag
		bool success = true;
		SDL_Log("++START SDL++");
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow(
		        "Test Window",
		        SDL_WINDOWPOS_UNDEFINED,           // initial x position
		        SDL_WINDOWPOS_UNDEFINED,           // initial y position
		        640,                               // width, in pixels
		        480,                               // height, in pixels
		        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);

		glContext = SDL_GL_CreateContext(window);

		#ifdef DESKTOP
		GLenum glewError = glewInit(); 
		if( glewError != GLEW_OK ) { 
		SDL_Log( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) ); }
		#endif

		//Use OpenGL 3.0 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		// Turn on double buffering with a 24bit Z buffer.
		// You may need to change this to 16 or 32 for your system
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);


		//Use Vsync
		if( SDL_GL_SetSwapInterval( 1 ) < 0 )
		{
		    SDL_Log( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
		}

		//Initialize OpenGL
		SDL_Log("++INIT GL++");

		//Setup gl settings.
		glEnable(GL_DEPTH_TEST);
		glClearDepthf(1.0f);
		glDepthFunc(GL_LEQUAL);
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearColor( 0.f, 0.f, 0.f, 1.f );

		if (window == NULL){
			err("Could not create window: %s");
		    }

		    //Main loop.
		    SDL_Event e;
		    bool quit = false;
		    //SDL_StartTextInput();

		//Let listener know that we're created now.
        if(!listener->create()){
            this->dispose();
            return;	
        }
		listener->resize(640,480);

		//Enter main loop.
		while(!quit){
			while(SDL_PollEvent(&e) != 0){
			    switch(e.type){
				case SDL_QUIT:
				    quit = true;
				    break;
				case SDL_TEXTINPUT:
				    SDL_Log("text input!");
				    if(e.text.text[0] == 'q')
				        quit = true;
				    break;
				case SDL_WINDOWEVENT:
				    SDL_Log("WINDOW EVENT");
				    if (e.window.windowID == SDL_GetWindowID(window)) {
				        switch (e.window.event) {
				            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                                                listener->resize(e.window.data1,e.window.data2);
				                break;
				            }
				        }
				    }break;
			    }
			}

			listener->render();
			SDL_GL_SwapWindow(window);
		    }
		this->dispose();
	}
};
