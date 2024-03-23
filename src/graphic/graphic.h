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
    int width_px = 1760;
    int height_px = 1024;

    // int width_px = 1320;
    // int height_px = 768;

    bool allowMovement = false;
    bool show_login = true;
    bool show_display = false;
    bool show_codeEditor = false;
    bool show_config = false;
    bool show_charSelector = false;
    bool show_blur = false;
    bool show_userProfile = false;

    // bool allowMovement = false;
    // bool show_login = false;
    // bool show_display = true;
    // bool show_codeEditor = false;
    // bool show_config = true;
    // bool show_charSelector = true;
    // bool show_blur = true;
    // bool show_userProfile = true;
    

    bool characterCreated = false;

    std::string rom_file;

    string result = "";

    //graphic functions
    void setup();

    void makeBackground(imageHandler background, vector<vector<int>> grid, double gridX, double gridY, bool canMove);
    void makeConfig(vector<string> &codeStarter, TextEditor &editor);
    void makeCodeEditor(TextEditor &editor, const char* fileToEdit);
    void makeCharacter(imageHandler& image, imageHandler& overlap, double &gridX, double &gridY, movementHandler move, int &lastAction, characterManager &character, characterBuilder& charBuild, bool canMove);
    void makeCharacterSelector(imageHandler& image, characterManager &character, characterBuilder& charBuild);
    void makeLogIn(login& Login, imageHandler& image);
    void makeQuestion();

    void makeBlur();

    void makeUserProfile();
    string executeCPP(string code);

private:

};