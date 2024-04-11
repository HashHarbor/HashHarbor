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

string configReader::check(string configPath, string world, string room, string gridLocation)
{
    imagePath imgPth = imagePath();

    // gridlocation must be in the "56,25" format to find grid location
    auto config = cpptoml::parse_file(imgPth.absolutePath + "assets/map/" + world + room + "/config.toml");
    auto location = config->get_table(gridLocation);

    return *location->get_as<string>("room");
}

