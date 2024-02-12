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
        // load images from file and store as texture
    void DrawImage(imageHandler _image);
        // draw the whole image as loaded
    void DrawImage(imageHandler& _image, float scaleFactor);
        // draw image as stored but alter the scale drawn on the screen
    void DrawAnimationFrame(imageHandler _image, pair<ImVec2,ImVec2> cords, float scaleFactor);
        // used for drawing animations from sprite sheet by drawing only a portion of the loaded image
    pair<ImVec2,ImVec2> generateCords(int animation, int frame, float spriteWidth, float spriteHeight, float imageWidth, float imageHeight);
        // generate coordinates to make animations
        // DO NOT USE FOR CHARACTERS - there are hardcoded values as all NPC and Characters have the save size sprite sheet
};