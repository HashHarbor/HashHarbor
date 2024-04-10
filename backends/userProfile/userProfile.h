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

#include <map>

using std::string;
using std::vector;
using std::map;

// User profile is implemented as a singleton class
class userProfile {
    string username = "";
    string db_id = ""; // used to access the db
    string profile_id = ""; // the user profile number for display

    string joinDate = "";
    string lastActive = "";
    int streak = 0;

    int character[8];

    map<int, bool> problems;
    int totalComplete = 0;

    pair<int,int> resolution = {0,0};

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
    string getLastActive();
    int getStreak();
    pair<int,int>* getResolution();
    int getTotalProbComp();

    void setJoinDate(string date);
    void setLastActive(string date);
    void setStreak(int s);
    void setCharacter(int i, int val);
    void setSettings(int w, int h);
    void setTotalProbComp(int i);

    void updateSettings(int w, int h);

    void clear();
};

