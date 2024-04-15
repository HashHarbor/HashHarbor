#pragma once

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

#include "../src/character/characterManager.h"

using namespace std;

class configReader{
public:
    string location = "";
    double gridX, gridY;

    configReader();
    string check(string configPath, string gridLocation);
    void getNpc(std::string configPath, characterManager *character);

};