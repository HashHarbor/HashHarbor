#include <stdio.h>
#include <bits/stdc++.h>

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

#include <opencv2/opencv.hpp>

#include "movementHandler.h"
#include "../imageHandler/imageHandler.h"


movementHandler::movementHandler(string filepath){

    cv::Mat image = cv::imread(filepath, cv::IMREAD_UNCHANGED);

    if (image.empty()) {
        std::cerr << "Error: Could not load map obstacles." << std::endl;
    }

    // Check if image has alpha channel for transparency
    if (image.channels() == 4) {
        
        std::vector<cv::Mat> channels;
        cv::split(image, channels);

        // Alpha channel is the last one (index 3)
        cv::Mat alpha = channels[3];

        rows = image.rows/32;
        cols = image.cols/32;

        grid.resize(rows, std::vector<int>(cols));

        // Process pixels
        for (int y = 0; y < image.rows/32; ++y) {
            for (int x = 0; x < image.cols/32; ++x) {

                // Access alpha value of the pixel
                uchar alphaValue = alpha.at<uchar>(y*32, x*32);

                if (alphaValue != 0) {
                    grid[y][x] = 1;
                }
            }
        }

    } else {
        std::cerr << "Error: Image does not have an alpha channel." << std::endl;
    }
}

std::vector<std::vector<int>> movementHandler::getGrid(){
    return this->grid;
}

void movementHandler::mapMovement(int key, imageHandler map, int &gridX, int &gridY){
    switch(key)
    {
        case 1:
            gridY--;
            break;
        case 2:
            gridY++;
            break;
        case 3:
            gridX++;
            break;
        case 4:
            gridX--;
            break;
    }

    map.DrawMap(map, gridX, gridY, 960, 544);

}