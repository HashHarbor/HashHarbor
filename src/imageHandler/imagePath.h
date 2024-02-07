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

    map<string, string> accessories ={
            {"backpack_5","assets/characters/accessories/backpack_5.png"},
            {"beanie_1","assets/characters/accessories/beanie_1.png"},
            {"beard_1","assets/characters/accessories/beard_1.png"},
            {"chef_1","assets/characters/accessories/chef_1.png"},
            {"dinohat_3","assets/characters/accessories/dinohat_3.png"}
    };
    map<string, string> body ={
            {"body_1","assets/characters/body/body_1.png"},
            {"body_2","assets/characters/body/body_2.png"},
            {"body_3","assets/characters/body/body_3.png"},
            {"body_4","assets/characters/body/body_4.png"}
    };
    map<string, string> eyes ={
            {"eyes_2","assets/characters/eyes/eyes_2.png"},
            {"eyes_4","assets/characters/eyes/eyes_4.png"},
            {"eyes_7","assets/characters/eyes/eyes_7.png"}
    };
    map<string, string> hairstyle ={
            {"hairstyle_1_2","assets/characters/hairstyle/hairstyle_1_2.png"},
            {"hairstyle_3_2","assets/characters/hairstyle/hairstyle_3_2.png"},
            {"hairstyle_4_2","assets/characters/hairstyle/hairstyle_4_2.png"},
            {"hairstyle_9_3","assets/characters/hairstyle/hairstyle_9_3.png"},
            {"hairstyle_11_1","assets/characters/hairstyle/hairstyle_11_1.png"},
            {"hairstyle_12_7","assets/characters/hairstyle/hairstyle_12_7.png"},
            {"hairstyle_19_6","assets/characters/hairstyle/hairstyle_19_6.png"}
    };
    map<string, string> outfits ={
            {"outfit_1_1","assets/characters/outfits/outfit_1_1.png"},
            {"outfit_2_1","assets/characters/outfits/outfit_2_1.png"},
            {"outfit_4_1","assets/characters/outfits/outfit_4_1.png"},
            {"outfit_10_1","assets/characters/outfits/outfit_10_1.png"},
            {"outfit_24_1","assets/characters/outfits/outfit_24_1.png"}
    };
};