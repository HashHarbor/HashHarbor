#pragma once

#include "imgui.h"
#include "../imageHandler/imageHandler.h"
#include "../character/characterManager.h"

class graphic {
public:
    int width_px = 1280;
    int height_px = 640;

    bool show_display = true;
    bool show_process = true;
    bool show_config = true;

    std::string rom_file;

    //graphic functions
    void setup();
    void makeDisplay(imageHandler image, characterManager &character);
    void makeProcess();
    void makeConfig();

private:

};