#include "cpptoml.h"
#include "configReader.h"

#include <bits/stdc++.h>

using namespace std;

configReader::configReader(){};

string configReader::check(string configPath, string world, string room, string gridLocation){
    auto config = cpptoml::parse_file("../assets/map/" + world + room + "config.toml");
    string value = config->get_as<string>(gridLocation);
    
    return value;
}

