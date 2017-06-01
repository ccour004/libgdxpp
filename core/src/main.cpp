#include <SDL.h>
#include "BulletSim.hpp"
#include "ShapeCreator.hpp"

//#include <GLES/gl.h>

MeshBuilder modelBuilder;
Shader shader;
Mesh mesh;
MeshInstance* instance;

class SDL_Manager{
public:
    SDL_Window* window;
    SDL_GLContext glContext;

    SDL_Manager(){
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
                SDL_WINDOW_OPENGL                  // flags - see below
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

        //Init meshes.
        shader.init();
        mesh = modelBuilder.build(2,2,2,20,20);SDL_Log("OK2");
        mesh.init();

        //Setup instance.
        shader.setModelMatrix(0,0,0);
        shader.setProjectionMatrix(0.1f,100.0f,0.6182f,67.0f);
        shader.setViewMatrix(btVector3(0,0,3.0f),
                      btVector3(0,0,0),
                      btVector3(0,1.0f,0));
        instance = new MeshInstance(&mesh,&shader,btVector3(0,0,0));

    }

    ~SDL_Manager(){
        SDL_Log("~~STOP SDL~~");
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
} *sdl;

void dispose(){
    delete sdl;
}

int err(const char* fmt){
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,fmt, SDL_GetError());
    dispose();
    return 1;
}

void render()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    instance->render();
}

int main(int argc, char* argv[])
{
    //Setup.
    sdl = new SDL_Manager;
    if (sdl->window == NULL){
        err("Could not create window: %s");
    }

    //Main loop.
    SDL_Event e;
    bool quit = false;
    //SDL_StartTextInput();

    //!!! THIS WORKS !!!
    BulletSim* sim = new BulletSim;
    sim->init();
    sim->runSim();

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
                    if (e.window.windowID == SDL_GetWindowID(sdl->window)) {
                        switch (e.window.event) {
                            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                                instance->resize(e.window.data1,e.window.data2);
                                break;
                            }
                        }
                    }break;
            }
        }

        render();
        SDL_GL_SwapWindow(sdl->window);
    }

    //Clean up.
    delete sdl;
    return 0;
}
