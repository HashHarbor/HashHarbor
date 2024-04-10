#pragma once

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

using std::string;
using std::vector;

// User profile is implemented as a singleton class
class userProfile {
    string username = "";
    string db_id = ""; // used to access the db
    string profile_id = ""; // the user profile number for display

    string joinDate = "";
    int character[8];

    userProfile();
    ~userProfile();
    userProfile(const userProfile&) = delete;
    userProfile& operator=(const userProfile&) = delete;
    static userProfile* instance;
public:
    static userProfile& getInstance();

    //----- User -----//
    string getUsername();
    string getId();
    string getUserId();

    void setUsername(string username);
    void setId(string id);

    //----- User Data -----//
    string getJoinDate();
    int* getCharacter();

    void setJoinDate(string date);
    void setCharacter(int i, int val);

    void clear();
};

