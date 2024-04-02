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

// This will be used to refactor the pauseMenu screen
class pauseMenu
{
    int width_px;
    int height_px;
    float windowWidth = 0.f;
    float windowHeight = 0.f;
    float paddingHeight = 0.f;
    float paddingWidth = 0.f;
    ImDrawList* draw_list = nullptr;

    bool settingsWindow = false;
    bool notebookWindow = false;
    bool characterWindow = false;
    bool logOutWindow = false;
    bool quitWindow = false;

    bool usr_Username = false;
    bool usr_Username_Success = false;
    bool usr_Username_Fail = false;

    bool usr_Password = false;
    bool usr_Password_Success = false;
    bool usr_Password_FailCmp = false;
    bool usr_Password_Fail = false;

    char createUsername[64] = "";

    char currentPasswd[64] = "";
    char createPasswd[64] = "";
    char confirmPasswd[64] = "";

    bool viewPasswd = false;

    vector<string> notebook;

    void mainControls();
    void drawSettingsWindow(imageHandler *image, characterBuilder *charBuild, characterManager *character, bool* changeScreenRes, pair<int,int>* res);
    void drawNotebookWindow();
    void drawCharacterCreatorWindow(imageHandler *image, characterBuilder *charBuild, bool *updateCharacter);
    void drawLogOutWindow(bool* reset);
    void drawQuitWindow(bool* done);

    void updateUserUsername(float profileWidth, float profileHeight);
    void updateUsernameSuccess(float profileWidth, float profileHeight);
    void updateUsernameError(float profileWidth, float profileHeight);

    void updateUserPassword(float profileWidth, float profileHeight);
    void updatePasswordSuccess(float profileWidth, float profileHeight);
    void updatePasswordError_Cmp(float profileWidth, float profileHeight); // error message for password comparison failing
    void updatePasswordError(float profileWidth, float profileHeight); // general error: either database or passwords dont match

    void settingsMain(bool* changeScreenRes, pair<int,int>* res);
    void settingsUser(imageHandler *image, characterBuilder *charBuild, characterManager *character);

public:
    pauseMenu(int width, int height);
    void drawPauseMenu(imageHandler *image,characterManager *character, characterBuilder *charBuild, bool* changeScreenRes, pair<int,int>* res, bool* updateCharacter, bool* reset, bool* done);
    void reset();
};
