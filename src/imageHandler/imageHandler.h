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

#include <bits/stdc++.h>
using namespace std;


class imageHandler{
public:
    int width;
    int height;
    GLuint texture;
    int channels = 4;
    const char* filepath;

    
    imageHandler();
    imageHandler(const char* filepath);
    imageHandler(GLuint texture, int width, int height);

    bool loadTexture(const char *filename, imageHandler* image);
    bool CreateAnimation(vector<string>& paths, vector <imageHandler*> &frames);
    void DrawImage(imageHandler _image);
    void DrawMap(imageHandler _image, int tileX, int tileY, float width, float height);

private:

    

};