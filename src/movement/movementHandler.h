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
    std::vector<std::vector<int>> intGrid;
    int rows;
    int cols;
    int tileSize = 32;
    string filepath = "";

    bool adjust = false;

    int width_px = 0;
    int height_px = 0;

    string arrowsPath = "../assets/map/arrows.png";
    imageHandler arrows = imageHandler(arrowsPath.c_str()); 

    int frameCount_4 = 0; // animation control for 4 frame
    int frameCount_6 = 0; // animation control for 6 frame

    std::vector<std::pair<ImVec2,ImVec2>> arrowUp = {
            {ImVec2(0.1f / 192.f, 0.1f/128.f),ImVec2(31.99f/192.f, 31.99f/128.f)},
            {ImVec2(32.1f / 192.f, 0.1f/128.f),ImVec2(63.99f/192.f, 31.99f/128.f)},
            {ImVec2(64.1f / 192.f, 0.1f/128.f),ImVec2(95.99f/192.f, 31.99f/128.f)},
            {ImVec2(96.1f / 192.f, 0.1f/128.f),ImVec2(127.99f/192.f, 31.99f/128.f)},
            {ImVec2(128.1f / 192.f, 0.1f/128.f),ImVec2(159.99f/192.f, 31.99f/128.f)},
             {ImVec2(160.1f / 192.f, 0.1f/128.f),ImVec2(191.99f/192.f, 31.99f/128.f)}
    };


    movementHandler(string obspath, string intpath, int width, int height);
    std::vector<std::vector<int>> getGrid();
    std::vector<std::vector<int>> getIntGrid();
    void adjustResolution(int width, int height);
    void mapMovement(int key, imageHandler image, double &gridX, double &gridY, int rows, int cols, int &lastAction, int &interact);
    void drawArrows(float frameTimer, int key);

private:
    

};