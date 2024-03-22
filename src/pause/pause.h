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

// This will be used to refactor the pause screen
class pause
{
    int width_px;
    int height_px;

    bool settingsWindow = false;
    bool userProfileWindow = false;
    bool characterWindow = false;
    bool logOutWindow = false;
    bool quitWindow = false;
public:
    pause(int width, int height, imageHandler* imgHandler);
};
