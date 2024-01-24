#pragma once

#include "imgui.h"

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
    void makeDisplay();
    void makeProcess();
    void makeConfig();

private:

};