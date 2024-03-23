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
    string id = "";

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

    void setUsername(string username);
    void setId(string id);

    //----- User Data -----//
    string getJoinDate();
    int* getCharacter();

    void setJoinDate(string date);
    void setCharacter(int i, int val);

    void clear();
};

