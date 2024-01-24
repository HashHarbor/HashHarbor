#pragma once

#include "imgui.h"
#include "CharacterManager.h"
#include "ImageHandler.h"
#include <iostream>

class graphic {
public:
    int width_px = 1280;
    int height_px = 720;

    bool show_display = true;
    bool show_process = true;
    bool show_config = true;

    bool show_MainCharacter = true;

    std::string rom_file;

    //graphic functions
    void setup();
    void makeDisplay();
    void makeProcess();
    void makeConfig();

    void makeMainCharacter();

private:
    // Handlers and Managers
    ImageHandler imageHandler = ImageHandler();
    CharacterManager characterManager = CharacterManager();

};