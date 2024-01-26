#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

class imagePath
{
    /*----------BOB-----------*/
    std::map<string, vector<string>> Bob = {
            {"Idle",{"assets/Bob_idle_1.png",
                            "assets/Bob_idle_2.png",
                            "assets/Bob_idle_3.png",
                            "assets/Bob_idle_4.png"}},
            {"Up", {"assets/Bob_Up_walk_1.png",
                            "assets/Bob_Up_walk_2.png",
                            "assets/Bob_Up_walk_3.png",
                            "assets/Bob_Up_walk_4.png",
                            "assets/Bob_Up_walk_5.png",
                            "assets/Bob_Up_walk_6.png"}},
            {"Down", {"assets/Bob_Down_walk_1.png",
                            "assets/Bob_Down_walk_2.png",
                            "assets/Bob_Down_walk_3.png",
                            "assets/Bob_Down_walk_4.png",
                            "assets/Bob_Down_walk_5.png",
                            "assets/Bob_Down_walk_6.png"}},
            {"Right", {"assets/Bob_Right_walk_1.png",
                            "assets/Bob_Right_walk_2.png",
                            "assets/Bob_Right_walk_3.png",
                            "assets/Bob_Right_walk_4.png",
                            "assets/Bob_Right_walk_5.png",
                            "assets/Bob_Right_walk_6.png"}},
            {"Left", {"assets/Bob_Left_walk_1.png",
                            "assets/Bob_Left_walk_2.png",
                            "assets/Bob_Left_walk_3.png",
                            "assets/Bob_Left_walk_4.png",
                            "assets/Bob_Left_walk_5.png",
                            "assets/Bob_Left_walk_6.png"}}
    };



public:
    imagePath(){}
    /*-----IMPORTANT-----*/
        // In testing, it required an absolute path to import images
        // To test relative paths set absolutePath to "../"
        //todo -  Pleases change the absolute path to
    string absolutePath = "../";

    map<string, map<string, vector<string>>> index = {
            {"Bob", Bob}
    };
};