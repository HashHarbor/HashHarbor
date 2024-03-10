#include "authentication.h"

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
                return auth(usr, passwd);
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

    if(db.getUserAuth(usr, profile)) // if profile is in database
    {
        string authHash = hash(passwd, profile.salt);
        if(!authHash.empty())
        {
            if(profile.hash == authHash)
            {
                profile.completeAuth(); // clears password and salt to not store after auth
                _id = profile._id;
                username = profile.username;
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
    return db.makeUser(profile);
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

void authentication::getUser(std::string &usr, std::string &id)
{
    usr = username;
    id = _id;
}