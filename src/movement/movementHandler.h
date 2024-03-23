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