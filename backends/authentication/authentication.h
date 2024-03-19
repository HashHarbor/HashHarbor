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

class authentication
{
    // Keep as private functions, only call if input has been validated
    bool auth(string usr, string passwd);
    bool createUser(string usr, string passwd);

    string saltGenerator();
    string hash(string passwd, string salt);
public:
    string username;
    string _id;
    authentication();

    bool inputValidation(string usr, string passwd, bool mode);
    // TRUE -> Authentication
    // FALSE -> Create Account

    void getUser(string& usr, string& id);
};