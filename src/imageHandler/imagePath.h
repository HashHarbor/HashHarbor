#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

class imagePath
{
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

    map<string, string> charIndex = {
            {"Bob", "assets/characters/Bob.png"},
            {"1", "assets/npc/Jake.png"},
            {"2", "assets/npc/2.png"},
            {"3", "assets/npc/3.png"},
            {"4", "assets/npc/4.png"},
            {"5", "assets/npc/5.png"},
            {"6", "assets/npc/6.png"},
            {"7", "assets/npc/7.png"},
            {"8", "assets/npc/8.png"},
            {"9", "assets/npc/9.png"},
            {"10", "assets/npc/10.png"},
            {"11", "assets/npc/11.png"},
            {"12", "assets/npc/12.png"},
            {"13", "assets/npc/13.png"},
            {"14", "assets/npc/14.png"},
            {"15", "assets/npc/15.png"},
            {"16", "assets/npc/16.png"},
            {"17", "assets/npc/17.png"},
            {"18", "assets/npc/18.png"},
            {"19", "assets/npc/19.png"},
            {"20", "assets/npc/20.png"},
            {"21", "assets/npc/21.png"},
            {"22", "assets/npc/22.png"},
            {"23", "assets/npc/23.png"},
    };
};