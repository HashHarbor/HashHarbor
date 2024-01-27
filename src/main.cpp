#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

//#include <bits/stdc++.h>

#include "graphic/graphic.h"


int main(int argc, char **argv) 
{
    graphic window;

    //setup graphics
    window.setup();

    return 0;
}