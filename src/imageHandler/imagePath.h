#pragma once

#include <map>
#include <vector>
#include <string>
#include <unordered_map>

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

    map<int, vector<string>> accessories ={
            // bow
            {1, {"assets/characters/accessories/bow_1.png",
                        "assets/characters/accessories/bow_2.png",
                        "assets/characters/accessories/bow_3.png",
                        "assets/characters/accessories/bow_4.png",
                        "assets/characters/accessories/bow_5.png",
                        "assets/characters/accessories/bow_6.png"}},
            //backpack
            {2,{"assets/characters/accessories/backpack_1.png",
                                "assets/characters/accessories/backpack_2.png",
                                "assets/characters/accessories/backpack_3.png",
                                "assets/characters/accessories/backpack_4.png",
                                "assets/characters/accessories/backpack_5.png",
                                "assets/characters/accessories/backpack_6.png",
                                "assets/characters/accessories/backpack_7.png",
                                "assets/characters/accessories/backpack_8.png",
                                "assets/characters/accessories/backpack_9.png",
                                "assets/characters/accessories/backpack_10.png"}},
            // beanie
            {3, {"assets/characters/accessories/beanie_1.png",
                               "assets/characters/accessories/beanie_2.png",
                               "assets/characters/accessories/beanie_3.png",
                               "assets/characters/accessories/beanie_4.png",
                               "assets/characters/accessories/beanie_5.png"}},
            // beard
            {4, {"assets/characters/accessories/beard_1.png"}},
            // chef
            {5, {"assets/characters/accessories/chef_1.png"}},
            // dino hat
            {6, {"assets/characters/accessories/dinohat_3.png"}}
    };
    vector<string> body ={
            {"assets/characters/body/body_1.png"},
            {"assets/characters/body/body_2.png"},
            {"assets/characters/body/body_3.png"},
            {"assets/characters/body/body_4.png"},
            {"assets/characters/body/body_5.png"},
            {"assets/characters/body/body_6.png"},
            {"assets/characters/body/body_7.png"},
            {"assets/characters/body/body_8.png"},
            {"assets/characters/body/body_9.png"}
    };
    vector<string> eyes ={
            {"assets/characters/eyes/eyes_1.png"},
            {"assets/characters/eyes/eyes_2.png"},
            {"assets/characters/eyes/eyes_3.png"},
            {"assets/characters/eyes/eyes_4.png"},
            {"assets/characters/eyes/eyes_5.png"},
            {"assets/characters/eyes/eyes_6.png"},
            {"assets/characters/eyes/eyes_7.png"}
    };
    map<int, vector<string>> hairstyle ={
            {1, {"assets/characters/hairstyle/hairstyle_1_1.png",
                        "assets/characters/hairstyle/hairstyle_1_2.png",
                        "assets/characters/hairstyle/hairstyle_1_3.png",
                        "assets/characters/hairstyle/hairstyle_1_4.png",
                        "assets/characters/hairstyle/hairstyle_1_5.png",
                        "assets/characters/hairstyle/hairstyle_1_6.png",
                        "assets/characters/hairstyle/hairstyle_1_7.png"}},

            {3, {"assets/characters/hairstyle/hairstyle_3_1.png",
                        "assets/characters/hairstyle/hairstyle_3_2.png",
                        "assets/characters/hairstyle/hairstyle_3_3.png",
                        "assets/characters/hairstyle/hairstyle_3_4.png",
                        "assets/characters/hairstyle/hairstyle_3_5.png",
                        "assets/characters/hairstyle/hairstyle_3_6.png",
                        "assets/characters/hairstyle/hairstyle_3_7.png"}},

            {4, {"assets/characters/hairstyle/hairstyle_4_1.png",
                        "assets/characters/hairstyle/hairstyle_4_2.png",
                        "assets/characters/hairstyle/hairstyle_4_3.png",
                        "assets/characters/hairstyle/hairstyle_4_4.png",
                        "assets/characters/hairstyle/hairstyle_4_5.png",
                        "assets/characters/hairstyle/hairstyle_4_6.png",
                        "assets/characters/hairstyle/hairstyle_4_7.png"}},

            {7, {"assets/characters/hairstyle/hairstyle_7_1.png",
                        "assets/characters/hairstyle/hairstyle_7_2.png",
                        "assets/characters/hairstyle/hairstyle_7_3.png",
                        "assets/characters/hairstyle/hairstyle_7_4.png",
                        "assets/characters/hairstyle/hairstyle_7_5.png",
                        "assets/characters/hairstyle/hairstyle_7_6.png",
                        "assets/characters/hairstyle/hairstyle_7_7.png"}},

            {8, {"assets/characters/hairstyle/hairstyle_8_1.png",
                        "assets/characters/hairstyle/hairstyle_8_2.png",
                        "assets/characters/hairstyle/hairstyle_8_3.png",
                        "assets/characters/hairstyle/hairstyle_8_4.png",
                        "assets/characters/hairstyle/hairstyle_8_5.png",
                        "assets/characters/hairstyle/hairstyle_8_6.png",
                        "assets/characters/hairstyle/hairstyle_8_7.png"}},

            {9, {"assets/characters/hairstyle/hairstyle_9_1.png",
                        "assets/characters/hairstyle/hairstyle_9_2.png",
                        "assets/characters/hairstyle/hairstyle_9_3.png",
                        "assets/characters/hairstyle/hairstyle_9_4.png",
                        "assets/characters/hairstyle/hairstyle_9_5.png",
                        "assets/characters/hairstyle/hairstyle_9_6.png",
                        "assets/characters/hairstyle/hairstyle_9_7.png"}},

            {10, {"assets/characters/hairstyle/hairstyle_10_1.png",
                        "assets/characters/hairstyle/hairstyle_10_2.png",
                        "assets/characters/hairstyle/hairstyle_10_3.png",
                        "assets/characters/hairstyle/hairstyle_10_4.png",
                        "assets/characters/hairstyle/hairstyle_10_5.png",
                        "assets/characters/hairstyle/hairstyle_10_6.png",
                        "assets/characters/hairstyle/hairstyle_10_7.png"}},

            {11, {"assets/characters/hairstyle/hairstyle_11_1.png",
                         "assets/characters/hairstyle/hairstyle_11_2.png",
                         "assets/characters/hairstyle/hairstyle_11_3.png",
                         "assets/characters/hairstyle/hairstyle_11_4.png",
                         "assets/characters/hairstyle/hairstyle_11_5.png",
                         "assets/characters/hairstyle/hairstyle_11_6.png",
                         "assets/characters/hairstyle/hairstyle_11_7.png"}},

            {12, {"assets/characters/hairstyle/hairstyle_12_1.png",
                         "assets/characters/hairstyle/hairstyle_12_2.png",
                         "assets/characters/hairstyle/hairstyle_12_3.png",
                         "assets/characters/hairstyle/hairstyle_12_4.png",
                         "assets/characters/hairstyle/hairstyle_12_5.png",
                         "assets/characters/hairstyle/hairstyle_12_6.png",
                         "assets/characters/hairstyle/hairstyle_12_7.png"}},

            {15, {"assets/characters/hairstyle/hairstyle_15_1.png",
                         "assets/characters/hairstyle/hairstyle_15_2.png",
                         "assets/characters/hairstyle/hairstyle_15_3.png",
                         "assets/characters/hairstyle/hairstyle_15_4.png",
                         "assets/characters/hairstyle/hairstyle_15_5.png",
                         "assets/characters/hairstyle/hairstyle_15_6.png",
                         "assets/characters/hairstyle/hairstyle_15_7.png"}},

            {18, {"assets/characters/hairstyle/hairstyle_18_1.png",
                         "assets/characters/hairstyle/hairstyle_18_2.png",
                         "assets/characters/hairstyle/hairstyle_18_3.png",
                         "assets/characters/hairstyle/hairstyle_18_4.png",
                         "assets/characters/hairstyle/hairstyle_18_5.png",
                         "assets/characters/hairstyle/hairstyle_18_6.png",
                         "assets/characters/hairstyle/hairstyle_18_7.png"}},

            {19, {"assets/characters/hairstyle/hairstyle_19_1.png",
                         "assets/characters/hairstyle/hairstyle_19_2.png",
                         "assets/characters/hairstyle/hairstyle_19_3.png",
                         "assets/characters/hairstyle/hairstyle_19_4.png",
                         "assets/characters/hairstyle/hairstyle_19_5.png",
                         "assets/characters/hairstyle/hairstyle_19_6.png",
                         "assets/characters/hairstyle/hairstyle_19_7.png"}},

            {23, {"assets/characters/hairstyle/hairstyle_23_1.png",
                         "assets/characters/hairstyle/hairstyle_23_2.png",
                         "assets/characters/hairstyle/hairstyle_23_3.png",
                         "assets/characters/hairstyle/hairstyle_23_4.png",
                         "assets/characters/hairstyle/hairstyle_23_5.png",
                         "assets/characters/hairstyle/hairstyle_23_6.png",
                         "assets/characters/hairstyle/hairstyle_23_7.png"}}
    };
    map<int, vector<string>> outfits ={
            {1, {"assets/characters/outfits/outfit_1_1.png",
                        "assets/characters/outfits/outfit_1_2.png",
                        "assets/characters/outfits/outfit_1_3.png",
                        "assets/characters/outfits/outfit_1_4.png",
                        "assets/characters/outfits/outfit_1_5.png",
                        "assets/characters/outfits/outfit_1_6.png",
                        "assets/characters/outfits/outfit_1_7.png",
                        "assets/characters/outfits/outfit_1_8.png",
                        "assets/characters/outfits/outfit_1_9.png",
                        "assets/characters/outfits/outfit_1_10.png"}},

            {2, {"assets/characters/outfits/outfit_2_1.png",
                        "assets/characters/outfits/outfit_2_2.png",
                        "assets/characters/outfits/outfit_2_3.png",
                        "assets/characters/outfits/outfit_2_4.png"}},

            {4, {"assets/characters/outfits/outfit_4_1.png",
                        "assets/characters/outfits/outfit_4_2.png",
                        "assets/characters/outfits/outfit_4_3.png"}},

            {5, {"assets/characters/outfits/outfit_5_1.png",
                        "assets/characters/outfits/outfit_5_2.png",
                        "assets/characters/outfits/outfit_5_3.png",
                        "assets/characters/outfits/outfit_5_4.png",
                        "assets/characters/outfits/outfit_5_5.png"}},

            {6, {"assets/characters/outfits/outfit_6_1.png",
                        "assets/characters/outfits/outfit_6_2.png",
                        "assets/characters/outfits/outfit_6_3.png",
                        "assets/characters/outfits/outfit_6_4.png"}},

            {8, {"assets/characters/outfits/outfit_8_1.png",
                        "assets/characters/outfits/outfit_8_2.png",
                        "assets/characters/outfits/outfit_8_3.png"}},

            {9, {"assets/characters/outfits/outfit_9_1.png",
                        "assets/characters/outfits/outfit_9_2.png",
                        "assets/characters/outfits/outfit_9_3.png"}},

            {10, {"assets/characters/outfits/outfit_10_1.png",
                         "assets/characters/outfits/outfit_10_2.png",
                         "assets/characters/outfits/outfit_10_3.png",
                         "assets/characters/outfits/outfit_10_4.png",
                         "assets/characters/outfits/outfit_10_5.png"}},

            {11, {"assets/characters/outfits/outfit_11_1.png",
                        "assets/characters/outfits/outfit_11_2.png",
                        "assets/characters/outfits/outfit_11_3.png",
                        "assets/characters/outfits/outfit_11_4.png"}},

            {16, {"assets/characters/outfits/outfit_16_1.png",
                        "assets/characters/outfits/outfit_16_2.png",
                        "assets/characters/outfits/outfit_16_3.png"}},

            {18, {"assets/characters/outfits/outfit_18_1.png",
                        "assets/characters/outfits/outfit_18_2.png",
                        "assets/characters/outfits/outfit_18_3.png",
                        "assets/characters/outfits/outfit_18_4.png"}},

            {20, {"assets/characters/outfits/outfit_20_1.png",
                        "assets/characters/outfits/outfit_20_2.png",
                        "assets/characters/outfits/outfit_20_3.png"}},

            {21, {"assets/characters/outfits/outfit_21_1.png",
                        "assets/characters/outfits/outfit_21_2.png",
                        "assets/characters/outfits/outfit_21_3.png",
                        "assets/characters/outfits/outfit_21_4.png"}},

            {24, {"assets/characters/outfits/outfit_24_1.png",
                         "assets/characters/outfits/outfit_24_2.png",
                         "assets/characters/outfits/outfit_24_3.png",
                         "assets/characters/outfits/outfit_24_4.png"}}
    };
};