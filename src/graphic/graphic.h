#pragma once

#include "imgui.h"
#include "../imageHandler/imageHandler.h"
#include "../character/characterManager.h"
#include "../character/characterBuilder.h"
#include "../movement/movementHandler.h"

class graphic {
public:
    int width_px = 1920;
    int height_px = 1072;

    bool show_display = true;
    bool show_process = true;
    bool show_config = true;
    bool show_charSelector = true;

    bool characterCreated = false;

    std::string rom_file;

    //graphic functions
    void setup();

    void makeDisplay(imageHandler& image, characterManager &character, characterBuilder& charBuild);
    void makeProcess();
    void makeConfig();
    void makeCharacterSelector(imageHandler& image, characterManager &character, characterBuilder& charBuild);
    void makeBackground(imageHandler background, movementHandler move, float &gridX, float &gridY);

private:

};