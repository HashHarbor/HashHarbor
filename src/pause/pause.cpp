#include "pause.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "database/database.h"

#if defined(__APPLE__)
#include <iostream>
#include <vector>
#include <filesystem>
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
using std::pair;
#include <utility>
#endif

#include <regex>
#include "../assets/font/IconsFontAwesome6.h"
#include "../backends/authentication/authentication.h"
#include "../backends/database/database.h"
#include "../imageHandler/imageHandler.h"

using std::string;
using std::vector;
using std::regex;
