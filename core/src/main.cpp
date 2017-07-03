#include "LibGDXApplication.h"
#include "MyAppListener.h"

int main(int argc, char* argv[])
{    
    LibGDX_Application* app = new LibGDX_Application(new MyAppListener());
    return 0;
}
