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

using std::string;
using std::vector;

using std::regex;

class login
{
    bool authentication();
        // Keep as private function, only call if input has been validated
public:
    string username;
    string _id;
    login();

    bool inputValidation(string usr, string passwd);
};