#pragma once
#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;

class ImagePath
{
    /*----------BOB-----------*/
    map<string, vector<string>> Bob = {
            {"Idle",{"Sprites/Bob_idle_1.png",
                            "Sprites/Bob_idle_2.png",
                            "Sprites/Bob_idle_3.png",
                            "Sprites/Bob_idle_4.png"}},
            {"Up", {"Sprites/Bob_Up_walk_1.png",
                            "Sprites/Bob_Up_walk_2.png",
                            "Sprites/Bob_Up_walk_3.png",
                            "Sprites/Bob_Up_walk_4.png",
                            "Sprites/Bob_Up_walk_5.png",
                            "Sprites/Bob_Up_walk_6.png"}},
            {"Down", {"Sprites/Bob_Down_walk_1.png",
                            "Sprites/Bob_Down_walk_2.png",
                            "Sprites/Bob_Down_walk_3.png",
                            "Sprites/Bob_Down_walk_4.png",
                            "Sprites/Bob_Down_walk_5.png",
                            "Sprites/Bob_Down_walk_6.png"}},
            {"Right", {"Sprites/Bob_Right_walk_1.png",
                            "Sprites/Bob_Right_walk_2.png",
                            "Sprites/Bob_Right_walk_3.png",
                            "Sprites/Bob_Right_walk_4.png",
                            "Sprites/Bob_Right_walk_5.png",
                            "Sprites/Bob_Right_walk_6.png"}},
            {"Left", {"Sprites/Bob_Left_walk_1.png",
                            "Sprites/Bob_Left_walk_2.png",
                            "Sprites/Bob_Left_walk_3.png",
                            "Sprites/Bob_Left_walk_4.png",
                            "Sprites/Bob_Left_walk_5.png",
                            "Sprites/Bob_Left_walk_6.png"}}
    };



public:
    ImagePath(){}
    /*-----IMPORTANT-----*/
        // In testing, it required an absolute path to import images
        // To test relative paths set absolutePath to "../"
        //todo -  Pleases change the absolute path to
    string absolutePath = "/Users/david/CLionProjects/HashHarbor/";

    map<string, map<string, vector<string>>> index = {
            {"Bob", Bob}
    };
};
