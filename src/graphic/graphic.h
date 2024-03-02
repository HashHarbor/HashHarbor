#pragma once

#include "imgui.h"
#include "../imageHandler/imageHandler.h"
#include "../character/characterManager.h"
#include "../character/characterBuilder.h"
#include "../movement/movementHandler.h"
#include "textEditor/TextEditor.h"

class graphic {
public:
    int width_px = 1280;
    int height_px = 720;

    bool show_login = true;
    bool show_display = false;
    bool show_process = false;
    bool show_config = false;
    bool show_charSelector = false;

    bool characterCreated = false;

    std::string rom_file;

    //graphic functions
    void setup();

    void makeDisplay(imageHandler& image, characterManager &character, characterBuilder& charBuild);
    void makeConfig();
    void makeProcess(TextEditor &editor, const char* fileToEdit);
    void makeCharacterSelector(imageHandler& image, characterManager &character, characterBuilder& charBuild);
    void makeBackground(imageHandler background, movementHandler move, float &gridX, float &gridY);
    void makeLogIn();
    string executeCPP(string code);

private:

};