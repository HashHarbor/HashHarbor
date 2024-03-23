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
#include <filesystem>
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
using std::pair;
#include <utility>
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

    int width_px = 0;
    int height_px = 0;

    movementHandler(string filepath, int width, int height);
    std::vector<std::vector<int>> getGrid();
    void adjustResolution(int width, int height);
    void mapMovement(int key, imageHandler image, double &gridX, double &gridY, int rows, int cols, int &lastAction);

private:
    

};