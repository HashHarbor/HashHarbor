#pragma once

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#if defined(__APPLE__)
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
#include <utility>
using std::pair;
#endif

#include <regex>
#include "database/database.h"
#include "../imageHandler/imageHandler.h"
#include "../character/characterManager.h"
#include "../character/characterBuilder.h"

using std::string;
using std::vector;

using std::regex;

// This will be used to refactor the pause screen
class pause
{
    int width_px;
    int height_px;
    float windowWidth = 0.f;
    float windowHeight = 0.f;
    float paddingHeight = 0.f;
    float paddingWidth = 0.f;
    ImDrawList* draw_list = nullptr;

    bool settingsWindow = false;
    bool userProfileWindow = false;
    bool characterWindow = false;
    bool logOutWindow = false;
    bool quitWindow = false;

    bool usr_Username = false;
    bool usr_Password = false;

    char createUsername[64] = "";

    char currentPasswd[64] = "";
    char createPasswd[64] = "";
    char confirmPasswd[64] = "";

    void mainControls();
    void drawSettingsWindow();
    void drawNotebookWindow();
    void drawCharacterCreatorWindow(imageHandler *image, characterBuilder *charBuild, bool *updateCharacter);
    void drawLogOutWindow(bool* reset);
    void drawQuitWindow(bool* done);

    void updateUserUsername(float profileWidth, float profileHeight) ;
    void updateUserPassword(float profileWidth, float profileHeight);

public:
    pause(int width, int height);
    void drawPauseMenu(imageHandler *image,characterManager *character, characterBuilder *charBuild, bool* resetPauseScreen, bool* updateCharacter, bool* reset, bool* done);
    void reset();
};
