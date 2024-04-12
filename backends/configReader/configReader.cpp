#include "configReader.h"

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

#include "cpptoml.h"
#include "../src/imageHandler/imagePath.h"

using namespace std;

configReader::configReader(){};

// string configReader::mapCheck(string configPath, string gridLocation)
// {

//     // gridlocation must be in the "56,25" format to find grid location
//     auto config = cpptoml::parse_file(configPath);
//     auto location = config->get_table(gridLocation);

//     return *location->get_as<string>("next");
// }

string configReader::check(string configPath, string gridLocation)
{
    auto config = cpptoml::parse_file(configPath);
    auto location = config->get_table(gridLocation);

    gridX = stod(*location->get_as<string>("coordX"));
    gridY = stod(*location->get_as<string>("coordY"));

    return *location->get_as<string>("next");
}   

