#pragma once

#include "imgui.h"
#include "../imageHandler/imageHandler.h"
#include "../character/characterManager.h"
#include "../character/characterBuilder.h"
#include "../movement/movementHandler.h"
#include "textEditor/TextEditor.h"
#include "../login/login.h"
#include "database/database.h"

class graphic {
public:
    int width_px = 1280;
    int height_px = 720;

    bool show_login = false;
    bool show_display = true;
    bool show_process = true;
    bool show_config = true;
    bool show_charSelector = true;
    bool show_settings = false;

    bool characterCreated = false;

    std::string rom_file;

    //graphic functions
    void setup();

    void makeDisplay(imageHandler& image, characterManager &character, characterBuilder& charBuild);
    void makeConfig();
    void makeProcess(TextEditor &editor, const char* fileToEdit);
    void makeCharacterSelector(imageHandler& image, characterManager &character, characterBuilder& charBuild);
    void makeBackground(imageHandler background, movementHandler move, float &gridX, float &gridY);
    void makeLogIn(login& Login, imageHandler& image);
    string executeCPP(string code);
    void makeSettings(login& Login, imageHandler& image, characterManager& character, characterBuilder& charBuild);

private:
    bool resetPauseScreen = false;
};