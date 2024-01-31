#pragma once

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
#include <vector>
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
#endif

using std::string;
using std::vector;


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
    void DrawImage(imageHandler _image);
    void DrawImage(imageHandler& _image, float scaleFactor);
    void DrawAniamtionFrame(imageHandler _image, pair<ImVec2,ImVec2> cords, float scaleFactor);
};