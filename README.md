# libgdxpp
Porting portions of LibGDX to C++, using standard libraries and C++11/14. Use SDL2 and its dependent projects for graphics, input, etc. for maximum portability across desktop and mobile.

Very much a WIP, many pieces missing still...

Some sections such as the network piece and plugins will likely not be ported, as they can be replaced with other specialized and more fully-featured C++ projects (such as Bullet physics, ZeroMQ, and many others).

See libgdxpp-template for an example project that pulls in this repo and its dependencies and creates a toolchain and build process for developing apps.
