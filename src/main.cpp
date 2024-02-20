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

#if defined(__APPLE__)
#include <iostream>
using std::string;
using std::vector;
using std::cout;
using std::endl;
#else
#include <bits/stdc++.h>
#endif

#include "graphic/graphic.h"

#include <opencv2/opencv.hpp>

int main(int argc, char **argv) 
{
    graphic window;

    //setup graphics
    window.setup();

    return 0;
}