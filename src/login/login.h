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

using std::string;
using std::vector;

using std::regex;

class login
{
    int width_px;
    int height_px;
    float minWidth = 0.f;
    float minHeight = 0.f;
    float txtPos_x = 0.f;

    bool createAccount = false;
    bool errorAuth = false;
    bool errorCmp = false;
    bool errorCreate = false;

    ImDrawList* draw_list = nullptr;
    imageHandler* img;

    bool STATUS = false;

    void drawLogin() ;// screen for existing users to login
    void drawCreateUser(); // screen for new users
    void drawBackground(imageHandler* imgHandler);

    void passwordGuide(string tempPasswd);
    void error_Auth();
    void error_Comp();
    void error_Create();
public:
    string _username;
    string _id;
    login(int width, int height, imageHandler* imgHandler);

    void drawLoginScreen(imageHandler* imgHandler);
    bool checkAuth();
};