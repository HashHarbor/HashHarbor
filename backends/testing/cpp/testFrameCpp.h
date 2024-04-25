#pragma once
#if defined(__APPLE__)
#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::ofstream;
#else
#include <bits/stdc++.h>
#endif
#include <string>
using namespace std;

class testFrameCpp {
public:
    string runTest(string code, int questionNumber, bool sampleTest);
    string testOne(bool samepleTest);
    string testTwo(bool samepleTest);
    string testThree(bool samepleTest);
    string testFour(bool samepleTest);
};