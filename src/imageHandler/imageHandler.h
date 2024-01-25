#pragma once

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif



class imageHandler{
public:
    int width;
    int height;
    GLuint texture;
    int channels = 4;
    const char* filepath;

    
    imageHandler();
    imageHandler(const char* filepath);

    bool loadTexture();
    bool CreateAnimation(vector<string>& paths, vector <image*> &frames);
    void DrawImage(image _image);

private:

    

};