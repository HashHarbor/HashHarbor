#include <stdio.h>

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

#include <opencv2/opencv.hpp>

#include "movementHandler.h"
#include "../imageHandler/imageHandler.h"


movementHandler::movementHandler(string filepath, int width, int height){

    width_px = width;
    height_px = height;

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

void movementHandler::adjustResolution(int width, int height){
    width_px = width;
    height_px = height;
}

void movementHandler::mapMovement(int key, imageHandler map, double &gridX, double &gridY, int rows, int cols, int &lastAction){
    
    static float speed = 0.2;
    const double tolerance = 0.00001;

    if(adjust){
        if(lastAction == 1 && gridY == floor(gridY) + speed){
        gridY = floor(gridY);
        }
        else if(lastAction == 2 && gridY == ceil(gridY) - speed){
            gridY = ceil(gridY);
        }
        else if(lastAction == 3 && gridX == ceil(gridX) - speed){
            gridX = ceil(gridX);
        }
        else if(lastAction == 4 && gridX == floor(gridX) + speed){
            gridX = floor(gridX);
        }

        adjust = false;
    }
    else{
        if (std::abs(gridY - std::floor(gridY)) > tolerance) {
        //if gridX needs to be on a whole numebr grid this is entered

        //cout << lastAction << " " << gridX << ", " << gridY << " " << gridY - floor(gridY) <<" entered Y" << endl;
        
        if (std::abs(gridY - std::floor(gridY)) < 1 - tolerance) {
            if (lastAction == 1) {
                gridY = gridY - speed;
            } else if (lastAction == 2) {
                gridY = gridY + speed;
            }
        }
        else{
            if((int)gridY < gridY){
                gridY = (int)gridY + 1; 
            }
            else{
                gridY = (int)gridY - 1;
            }
            
            //cout << "rounded Y" << gridY << endl;
            
        }
        } else if (std::abs(gridX - std::floor(gridX)) > tolerance) {
            //if gridY needs to be on a whole number grid this is entered

            //cout << lastAction << " " << gridX << ", " << gridY << " " << gridX - floor(gridX) << " entered X" << endl;

            if (std::abs(gridX - std::floor(gridX)) < 1 - tolerance) {
                if (lastAction == 3) {
                    gridX = gridX + speed;
                } else if (lastAction == 4) {
                    gridX = gridX - speed;
                }
            }
            else{
                if((int)gridX < gridX){
                    gridX = (int)gridX + 1;
                }
                else{
                    gridX = (int)gridX - 1;
                }
                
                // cout << "rounded X " << gridX << endl;
            }
        }
        else {
            
            switch(key)
            {
                case 1:
                    if(! (grid[gridY - 1][gridX] == 1)){
                        //cout << "enter up 1 at " << gridY << endl;
                        gridY = gridY - speed;
                        lastAction = 1;
                        
                    }
                    else{
                        //cout << "upwards " << gridY << endl;
                    }
                    
                    break;
                case 2:
                    if(! (grid[gridY + 1][gridX] == 1 )){
                        //cout << "enter down 2 at " << gridY << endl;
                        gridY = gridY + speed;
                        lastAction = 2;

                    }
                    else{
                        //cout << "downwards " << gridY << endl;
                    }
                    
                    break;
                case 3:
                    if(! (grid[gridY][gridX + 1] == 1 )){
                        //cout << "enter right 3 at " << gridX << endl;
                        gridX = gridX + speed;
                        lastAction = 3;
                        
                    }
                    else{
                        //cout << "right " << gridX << endl;
                    }
                    
                    break;
                case 4:
                    if(! (grid[gridY][gridX - 1] == 1)){
                        //cout << "enter left 4 at " << gridX << endl;
                        gridX = gridX - speed;
                        lastAction = 4;
                        
                    }
                    else{
                        //cout << "left " << gridX << endl;
                    }
                    
                    break;
            }
        }

        if((lastAction == 1 && gridY == floor(gridY) + speed) || (lastAction == 2 && gridY == ceil(gridY) - speed) || (lastAction == 3 && gridX == ceil(gridX) - speed) || (lastAction == 4 && gridX == floor(gridX) + speed)){
            adjust = true;
        }
    }

    map.DrawMap(map, gridX, gridY, (width_px / 2), (height_px / 2), rows, cols);

}