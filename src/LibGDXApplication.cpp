#include "LibGDXApplication.h"

SDL_Window* LibGDX_Application::window = 0;
SDL_GLContext LibGDX_Application::glContext;
std::shared_ptr<ApplicationListener> LibGDX_Application::listener;

int err(const char* fmt){
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,fmt, SDL_GetError());
    //dispose();
    return 1;
}

LibGDX_Application::LibGDX_Application(std::shared_ptr<ApplicationListener> listener){
        LibGDX_Application::listener = listener;

		//Initialization flag
		bool success = true;
		SDL_Log("++START SDL++");
		SDL_Init(SDL_INIT_VIDEO);

		LibGDX_Application::window = SDL_CreateWindow(
		        "Test Window",
		        SDL_WINDOWPOS_UNDEFINED,           // initial x position
		        SDL_WINDOWPOS_UNDEFINED,           // initial y position
		        640,                               // width, in pixels
		        480,                               // height, in pixels
		        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);

		LibGDX_Application::glContext = SDL_GL_CreateContext(LibGDX_Application::window);

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
		glClearColor( 0.66f, 0.66f, 0.66f, 1.f );

		if (LibGDX_Application::window == NULL){
			err("Could not create window: %s");
		    }

        //SDL_StartTextInput();

		//Let listener know that we're created now.
        if(!LibGDX_Application::listener->create()){
            this->dispose();
            return;	
        }
        
        SDL_AddEventWatch(event_filter,NULL);
        LibGDX_Application::listener->resize(640,480);

		//Enter main loop.
		while(true){
			LibGDX_Application::listener->render();
			SDL_GL_SwapWindow(window);
        }
	}

int LibGDX_Application::event_filter(void* data,SDL_Event* event){
    SDL_Log("EVENT!");
    switch(event->type){
				case SDL_QUIT:
				    dispose();
				    break;
				case SDL_TEXTINPUT:
				    SDL_Log("text input!");
				    if(event->text.text[0] == 'q')
				        dispose();
				    break;
				case SDL_WINDOWEVENT:
				    //SDL_Log("WINDOW EVENT");
				    if (event->window.windowID == SDL_GetWindowID(window)) {
				        switch (event->window.event) {
				            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                                SDL_Log("RESIZE WINDOW EVENT: %i,%i",event->window.data1,event->window.data2);
                                    LibGDX_Application::listener->resize(event->window.data1,event->window.data2);
				                break;
				            }
				        }
				    }break;
			    }
}
    
void LibGDX_Application::dispose(){
	   LibGDX_Application::listener->dispose();
	   SDL_Log("~~STOP SDL~~");
       SDL_GL_DeleteContext(LibGDX_Application::glContext);
	   SDL_DestroyWindow(LibGDX_Application::window);
	   SDL_Quit();
}