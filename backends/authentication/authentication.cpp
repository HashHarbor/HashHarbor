#include "authentication.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "database/database.h"
#include "userProfile/userProfile.h"

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

#include <iomanip>
#include <regex>
#include <random>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <sstream>

using std::string;
using std::vector;
using std::regex;
using std::stringstream;

authentication::authentication()
{
}

bool authentication::inputValidation(string usr, string passwd, bool mode)
{
    if(regex_search(usr, regex("^[A-Za-z0-9_.-]{3,48}$"))) // check if username matches requirments
    {
        if(std::regex_search(passwd, regex("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[~`!@#$%^&*()_+={}:;<>?-])[A-Za-z0-9~`!@#$%^&*()_+={}:;<>?-]{8,48}$"))) // if password is within allowed
        {
            if(mode)
            {
                if(auth(usr, passwd))
                {
                    database& db = database::getInstance();
                    db.getUserData();
                    return true;
                }
            }
            else
            {
                return createUser(usr, passwd);
            }
        }
    }
    return false;
}

bool authentication::auth(string usr, string passwd)
{
    database& db = database::getInstance();
    database::usrProfile profile;

    userProfile& usrProfile = userProfile::getInstance();

    if(db.getUserAuth(usr, profile)) // if profile is in database
    {
        string authHash = hash(passwd, profile.salt);
        if(!authHash.empty())
        {
            if(profile.hash == authHash)
            {
                profile.completeAuth(); // clears password and salt to not store after auth
                usrProfile.setUsername(profile.username);
                usrProfile.setId(profile._id);
                return true;
            }
        }
    }
    return false;
}

bool authentication::createUser(string usr, string passwd)
{
    database& db = database::getInstance();

    database::usrProfile profile; // create a profile to store new user data
    profile.username = usr;

    string salt = saltGenerator();
    string hashPasswd = hash(passwd, salt);

    if(hashPasswd.empty())
    {
        return false;
    }

    profile.hash = hashPasswd;
    profile.salt = salt;
    if(db.makeUser(profile))
    {
        userProfile& usrProfile = userProfile::getInstance();
        usrProfile.setId(profile._id);
        usrProfile.setUsername(profile.username);

        auto time_point = std::chrono::system_clock::time_point(std::chrono::system_clock::now());
        auto time = std::chrono::system_clock::to_time_t(time_point);
        std::tm* tmPtr = std::localtime(&time);
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << tmPtr->tm_mon + 1 << "/" << std::setw(2) << tmPtr->tm_mday << "/" << std::setw(4) << tmPtr->tm_year + 1900;
        usrProfile.setJoinDate(ss.str());

        return true;
    }
    return false;
}

string authentication::saltGenerator()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 63);

    string salt = "";

    static const char alphaNum[] ="./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for(int i = 0; i < 32; i++)
    {
        int j = dis(gen);
        salt += alphaNum[j];
    }
    return salt;
}

string authentication::hash(string passwd, string salt)
{

    string hashPassword = salt + passwd;

    EVP_MD_CTX* digestContext = EVP_MD_CTX_new();
    if(digestContext == nullptr)
    {
        return "";
    }
    if(!EVP_DigestInit_ex(digestContext, EVP_sha256(), nullptr))
    {
        EVP_MD_CTX_free(digestContext);
        return "";
    }
    if(!EVP_DigestUpdate(digestContext, hashPassword.c_str(), hashPassword.length()))
    {
        EVP_MD_CTX_free(digestContext);
        return "";
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned int hashLen = 0;
    if(!EVP_DigestFinal_ex(digestContext, hash, &hashLen))
    {
        EVP_MD_CTX_free(digestContext);
        return "";
    }

    EVP_MD_CTX_free(digestContext);
    stringstream hashConverted;
    for(unsigned int i = 0; i < hashLen; i++)
    {
        hashConverted << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return hashConverted.str();
}

bool authentication::changePassword(string oldPasswd, string newPasswd)
{
    if(std::regex_search(oldPasswd, regex("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[~`!@#$%^&*()_+={}:;<>?-])[A-Za-z0-9~`!@#$%^&*()_+={}:;<>?-]{8,48}$"))) // if old password is within allowed parameters
    {
        if(std::regex_search(newPasswd, regex("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[~`!@#$%^&*()_+={}:;<>?-])[A-Za-z0-9~`!@#$%^&*()_+={}:;<>?-]{8,48}$"))) // if new password is within allowed params
        {
            userProfile& usrProfile = userProfile::getInstance();

            if(auth(usrProfile.getUsername(), oldPasswd))
            {
                database &db = database::getInstance();
                database::usrProfile profile;

                string newSalt = saltGenerator();
                string authHash = hash(newPasswd, newSalt);

                profile._id = usrProfile.getId();
                profile.hash = authHash;
                profile.salt = newSalt;

                return db.updatePassword(profile);
            }
        }
    }
    return false;
}

bool authentication::changeUsername(string newUsr, string id)
{
    if(regex_search(newUsr, regex("^[A-Za-z0-9_.-]{3,48}$")))
    {
        database &db = database::getInstance();
        return db.updateUsername(newUsr);
    }
    return false;
}