#include "LibGDXApplication.h"
#include "MyAppListener.h"
#include <memory>

int main(int argc, char* argv[])
{    
    std::unique_ptr<LibGDX_Application> app = std::make_unique<LibGDX_Application>(
        std::make_shared<MyAppListener>());
    return 0;
}
