#include "LibGDXApplication.h"

int err(const char* fmt){
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,fmt, SDL_GetError());
    //dispose();
    return 1;
}

	LibGDX_Application::LibGDX_Application(std::shared_ptr<ApplicationListener> listener){
        this->listener = listener;
		//Initialization flag
		bool success = true;
		SDL_Log("++START SDL++");
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO);

        #ifdef DESKTOP
             window = SDL_CreateWindow(
                    "Test Window",
                    SDL_WINDOWPOS_UNDEFINED,           // initial x position
                    SDL_WINDOWPOS_UNDEFINED,           // initial y position
                    640,                               // width, in pixels
                    480,                               // height, in pixels
                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
            );
            glContext = SDL_GL_CreateContext(window);         

            GLenum glewError = glewInit(); 
            if( glewError != GLEW_OK ) { 
            SDL_Log( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) ); }  
        #else
            glContext = SDL_GL_CreateContext(Android_Window);

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
		glClearColor( 0.66f, 0.66f, 0.66f, 1.f );

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
        bool isPaused = false;
		while(!quit){
			while(SDL_PollEvent(&e) != 0){
                //System events first.
                if(e.type == SDL_QUIT){quit = true; continue;}
			    switch(e.type){
                    case SDL_WINDOWEVENT:
                        if (e.window.windowID == SDL_GetWindowID(window)) {
                            switch (e.window.event) {
                                case SDL_WINDOWEVENT_SIZE_CHANGED:
                                    SDL_Log("RESIZE WINDOW EVENT: %i,%i",e.window.data1,e.window.data2);
                                                    listener->resize(e.window.data1,e.window.data2);
                                    break;
                                case SDL_WINDOWEVENT_MINIMIZED:
                                    listener->pause();
                                    SDL_GL_DeleteContext(glContext);
                                    SDL_DestroyWindow(window);
                                    isPaused = true;
                                    break;
                                case SDL_WINDOWEVENT_RESTORED:
                                    listener->resume();
                                    window = SDL_CreateWindow(
                                            "Test Window",
                                            SDL_WINDOWPOS_UNDEFINED,           // initial x position
                                            SDL_WINDOWPOS_UNDEFINED,           // initial y position
                                            640,                               // width, in pixels
                                            480,                               // height, in pixels
                                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                                    );

                                    glContext = SDL_GL_CreateContext(window);
                                    isPaused = false;
                                    break;
                            }
                        }
                        break;
			    }
                
                //Now touch events, if an input processor is set.
                if(listener->getRawInputProcessor().get() != nullptr)
                switch(e.type){
                    case SDL_CONTROLLERAXISMOTION:listener->getRawInputProcessor()->controllerAxisEvent((const SDL_ControllerAxisEvent&)e); break;
                    case SDL_CONTROLLERBUTTONDOWN:
                    case SDL_CONTROLLERBUTTONUP:
                        listener->getRawInputProcessor()->controllerButtonEvent((const SDL_ControllerButtonEvent&)e); break;
                    case SDL_CONTROLLERDEVICEADDED:
                    case SDL_CONTROLLERDEVICEREMOVED:
                    case SDL_CONTROLLERDEVICEREMAPPED:
                        listener->getRawInputProcessor()->controllerDeviceEvent((const SDL_ControllerDeviceEvent&)e); break;
                    case SDL_FINGERMOTION: 
                    case SDL_FINGERDOWN:
                    case SDL_FINGERUP:
                        listener->getRawInputProcessor()->touchFingerEvent((const SDL_TouchFingerEvent&)e); break;
                    case SDL_KEYDOWN: 
                    case SDL_KEYUP:
                        listener->getRawInputProcessor()->keyboardEvent((const SDL_KeyboardEvent&)e); break;
                    case SDL_MOUSEMOTION: listener->getRawInputProcessor()->mouseMotionEvent((const SDL_MouseMotionEvent&)e); break;
                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                        listener->getRawInputProcessor()->mouseButtonEvent((const SDL_MouseButtonEvent&)e); break;
                    case SDL_MOUSEWHEEL: listener->getRawInputProcessor()->mouseWheelEvent((const SDL_MouseWheelEvent&)e); break;
                    case SDL_MULTIGESTURE: listener->getRawInputProcessor()->multiGestureEvent((const SDL_MultiGestureEvent&)e); break;
                    case SDL_TEXTINPUT:
                        if(e.text.text[0] == 'q') quit = true;
                        break;
                }
			}

			if(!isPaused){
                listener->render();
                SDL_GL_SwapWindow(window);
            }
        }
		this->dispose();
	}
    
void LibGDX_Application::dispose(){
	   listener->dispose();
	   SDL_Log("~~STOP SDL~~");
       SDL_GL_DeleteContext(glContext);
	   SDL_DestroyWindow(window);
	   SDL_Quit();
}
