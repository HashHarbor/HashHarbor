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
using std::string;
using std::vector;
using std::cout;
using std::endl;
#else
#include <bits/stdc++.h>
#endif
#include <opencv2/opencv.hpp>

#include "../imageHandler/imageHandler.h"

using namespace std;


class movementHandler{
public:
    std::vector<std::vector<int>> grid;
    int rows;
    int cols;
    int tileSize = 32;
    string filepath = "";

    movementHandler(string filepath);
    std::vector<std::vector<int>> getGrid();
    void mapMovement(int key, imageHandler image, float &gridX, float &gridY);

private:
    

};