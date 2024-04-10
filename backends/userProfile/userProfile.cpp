#include "userProfile.h"

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

string userProfile::getId()
{
    return db_id;
}

string userProfile::getUserId()
{
    return profile_id;
}

void userProfile::setUsername(string username)
{
    this->username = username;
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

void userProfile::setCharacter(int i, int val)
{
    character[i] = val;
}

int *userProfile::getCharacter()
{
    return character;
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