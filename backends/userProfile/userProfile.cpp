#include "userProfile.h"
#include <iostream>
#include <utility>
#include "database/database.h"

using std::pair;

userProfile::userProfile() {}
userProfile::~userProfile() {}

userProfile& userProfile::getInstance()
{
    static userProfile instance;
    return instance;
}

string userProfile::getUsername()
{
    return username;
}
void userProfile::setUsername(string username)
{
    this->username = username;
}

string userProfile::getId()
{
    return db_id;
}
string userProfile::getUserId()
{
    return profile_id;
}
void userProfile::setId(std::string id)
{
    this->db_id = id;

    for(int i = 0; i < id.size(); i++)
    {
        if (isdigit(id[i]))
        {
            profile_id += id[i];
        }
    }
}

string userProfile::getJoinDate()
{
    return joinDate;
}
void userProfile::setJoinDate(string date)
{
    joinDate = date;
}

int *userProfile::getCharacter()
{
    return character;
}
void userProfile::setCharacter(int i, int val)
{
    character[i] = val;
}

void userProfile::setSettings(int w, int h)
{
    resolution = {w, h};
}
pair<int, int>* userProfile::getResolution()
{
    if(resolution.first == 0) // if unable to load the settings return default
    {
#if defined(__APPLE__)
        resolution = {1320,768};
#else
        resolution = {1760,1024};
#endif
    }
    return &resolution;
}
void userProfile::updateSettings(int w, int h)
{
    resolution = {w,h};
    database& db = database::getInstance();
    db.updateSettings();

}

void userProfile::clear()
{
    username.clear();
    db_id.clear();
    profile_id.clear();
    joinDate.clear();
    character[0] = 0;
    character[0] = 5;
    character[0] = 8;
    character[0] = 0;
    character[0] = 2;
    character[0] = 2;
    character[0] = 0;
    character[0] = 3;
}