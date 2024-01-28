#pragma once

#include "imgui.h"
#include "../imageHandler/imageHandler.h"
#include "../character/characterManager.h"

class graphic {
public:
    int width_px = 1920;
    int height_px = 1080;

    bool show_display = true;
    bool show_process = true;
    bool show_config = true;

    std::string rom_file;

    //graphic functions
    void setup();
    void makeDisplay(imageHandler image, characterManager &character);
    void makeBackground(imageHandler background, int gridX, int gridY);
    void makeProcess();
    void makeConfig();

private:

};