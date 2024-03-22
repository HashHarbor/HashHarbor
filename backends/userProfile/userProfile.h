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

// User profile is implemented as a singleton class
class userProfile {
    string username = "";
    string id = "";
    string joinDate = "";

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

    void setUsername(string username);
    void setId(string id);

    //----- User Data -----//
    string getJoinDate();
    void setJoinDate();
};

