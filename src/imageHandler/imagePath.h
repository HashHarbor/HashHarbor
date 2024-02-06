#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

class imagePath
{
    /*----------BOB-----------*/
    std::map<string, vector<string>> Bob = {
            {"Idle",{"assets/characters/Bob/Bob_idle_1.png",
                            "assets/characters/Bob/Bob_idle_2.png",
                            "assets/characters/Bob/Bob_idle_3.png",
                            "assets/characters/Bob/Bob_idle_4.png"}},
            {"Up", {"assets/characters/Bob/Bob_Up_walk_1.png",
                            "assets/characters/Bob/Bob_Up_walk_2.png",
                            "assets/characters/Bob/Bob_Up_walk_3.png",
                            "assets/characters/Bob/Bob_Up_walk_4.png",
                            "assets/characters/Bob/Bob_Up_walk_5.png",
                            "assets/characters/Bob/Bob_Up_walk_6.png"}},
            {"Down", {"assets/characters/Bob/Bob_Down_walk_1.png",
                            "assets/characters/Bob/Bob_Down_walk_2.png",
                            "assets/characters/Bob/Bob_Down_walk_3.png",
                            "assets/characters/Bob/Bob_Down_walk_4.png",
                            "assets/characters/Bob/Bob_Down_walk_5.png",
                            "assets/characters/Bob/Bob_Down_walk_6.png"}},
            {"Right", {"assets/characters/Bob/Bob_Right_walk_1.png",
                            "assets/characters/Bob/Bob_Right_walk_2.png",
                            "assets/characters/Bob/Bob_Right_walk_3.png",
                            "assets/characters/Bob/Bob_Right_walk_4.png",
                            "assets/characters/Bob/Bob_Right_walk_5.png",
                            "assets/characters/Bob/Bob_Right_walk_6.png"}},
            {"Left", {"assets/characters/Bob/Bob_Left_walk_1.png",
                            "assets/characters/Bob/Bob_Left_walk_2.png",
                            "assets/characters/Bob/Bob_Left_walk_3.png",
                            "assets/characters/Bob/Bob_Left_walk_4.png",
                            "assets/characters/Bob/Bob_Left_walk_5.png",
                            "assets/characters/Bob/Bob_Left_walk_6.png"}}
    };



public:
    imagePath(){}
    /*-----IMPORTANT-----*/
        // In testing, it required an absolute path to import images
        // To test relative paths set absolutePath to "../"
        //todo -  Pleases change the absolute path to
        // /Users/david/CLionProjects/HashHarbor/
#if defined(__APPLE__)
// USER DEPENDENT
    string absolutePath = "/Users/david/CLionProjects/HashHarbor/";
#else
    string absolutePath = "../";
#endif


    map<string, map<string, vector<string>>> index = {
            {"Bob", Bob}
    };
    map<string, string> charIndex = {
            {"Bob", "assets/characters/Bob.png"},
            {"1", "assets/characters/Jake.png"},
            {"2", "assets/characters/2.png"},
            {"3", "assets/characters/3.png"},
            {"4", "assets/characters/4.png"},
            {"5", "assets/characters/5.png"},
            {"6", "assets/characters/6.png"},
            {"7", "assets/characters/7.png"},
            {"8", "assets/characters/8.png"},
            {"9", "assets/characters/9.png"},
            {"10", "assets/characters/10.png"},
            {"11", "assets/characters/11.png"},
            {"12", "assets/characters/12.png"},
            {"13", "assets/characters/13.png"},
            {"14", "assets/characters/14.png"},
            {"15", "assets/characters/15.png"},
            {"16", "assets/characters/16.png"},
            {"17", "assets/characters/17.png"},
            {"18", "assets/characters/18.png"},
            {"19", "assets/characters/19.png"},
            {"20", "assets/characters/20.png"},
            {"21", "assets/characters/21.png"},
            {"22", "assets/characters/22.png"},
            {"23", "assets/characters/23.png"}
    };
};