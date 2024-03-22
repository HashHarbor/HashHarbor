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
    return id;
}

void userProfile::setUsername(std::string username)
{
    this->username = username;
}
void userProfile::setId(std::string id)
{
    this->id = id;
}