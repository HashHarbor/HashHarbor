#include "login.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "database/database.h"

#if defined(__APPLE__)
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
using std::pair;
#include <utility>
#endif

#include <regex>

using std::string;
using std::vector;
using std::regex;

login::login()
{
}

bool login::inputValidation(string usr, string passwd)
{
    if(regex_search(usr, regex("^[A-Za-z0-9_.-]{3,40}$"))) // check if username matches requirments
    {
        if(std::regex_search(passwd, regex("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[~`!@#$%^&*()_+={}:;<>?-])[A-Za-z0-9~`!@#$%^&*()_+={}:;<>?-]{8,40}$"))) // if password is within allowed
        {
            authentication("temp");
            createUser(usr,passwd);
            authentication(usr);
            return true;
        }
    }
    return false;
}

bool login::authentication(string usr)
{
    database& db = database::getInstance();

    database::usrProfile profile;

    if(db.getUserAuth(usr, profile))
    {
        cout << "Can start Auth\n" << "USER_ID:" << profile._id << "\nUSER_PROFILE:" << profile.username << "\nUSER_Pass:" << profile.hash << "\nUSER_SALT:" << profile.salt <<endl;

    }
    // hash input with salt
    // compare
    return false;
}

bool login::createUser(string usr, string passwd)
{
    database& db = database::getInstance();

    database::usrProfile profile;
    profile.username = usr;
    profile.hash = passwd; // hash password
    profile.salt = "123456789-SALT-"; // replace with real salt

    return db.makeUser(profile);
}