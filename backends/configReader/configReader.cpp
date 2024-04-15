#include "configReader.h"

#if defined(__APPLE__)
#include <iostream>
#include <vector>
#include <map>
using std::cout;
using std::endl;
using std::pair;
using std::map;
#else
#include <bits/stdc++.h>
#include <utility>
using std::pair;
#endif

#include "cpptoml.h"
#include "../src/imageHandler/imagePath.h"
#include "../src/character/characterManager.h"

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

void configReader::getNpc(std::string configPath, characterManager *character)
{
    auto config = cpptoml::parse_file(configPath);
    auto npcArray = config->get_table_array("npc");

    if(npcArray)
    {
        for (const auto& iter : *npcArray)
        {
            auto cordX = iter->get_as<int>("cordsX");
            auto cordY = iter->get_as<int>("cordsY");
            auto hasQues = iter->get_as<bool>("hasQuestion");
            auto ques = iter->get_as<int>("questionNum");
            auto charArr = iter->get_array_of<int64_t>("character");

            int arr[8];
            int i = 0;
            for (const auto& val : *charArr)
            {
                //cout << val << endl;
                arr[i] = val;
                i++;
            }

            character->getNpc()->emplace(make_pair(*cordX,*cordY), npc(*cordX,*cordY,*hasQues,*ques, arr));
        }
    }
}

