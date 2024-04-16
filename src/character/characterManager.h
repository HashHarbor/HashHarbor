#pragma once

#define GL_SILENCE_DEPRECATION

#include "../../imgui/imgui.h"
//#include "../backends/imgui_impl_glfw.h"
#include "SDL2/SDL.h"
#include "../../backends/imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#if defined(__APPLE__)
#include <iostream>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;
#else
#include <bits/stdc++.h>
#endif

#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "characterBuilder.h"


struct character
{
public:
    string name; // character name used for identification
    bool fullMovement; // used to differentiate player or npc that needs all animations and npc that just idles

    vector<int> dynamicIndex = {0,0,0,0,0,0,0,0};

    imageHandler* spriteSheet;
    float spriteWidth;
    float spriteHeight;

    character(string name, bool fullMovement, float width, float height)
    {
        this->name = name;
        this->fullMovement = fullMovement;
        this->spriteWidth = width;
        this->spriteHeight = height;

    }
};

struct npc
{
public:
    int cordX;
    int cordY;
    bool hasQuestion;
    int question;
    vector<int> character = {0,0,0,0,0,0,0,0};
    // add something for dialog
    npc(int x, int y, bool hasQ, int q, int arr[8])
    {
        this->cordX = x;
        this->cordY = y;
        this->hasQuestion = hasQ;
        this->question = q;
        this->character[0] = arr[0];
        this->character[1] = arr[1];
        this->character[2] = arr[2];
        this->character[3] = arr[3];
        this->character[4] = arr[4];
        this->character[5] = arr[5];
        this->character[6] = arr[6];
        this->character[7] = arr[7];
    }
};

class characterManager
{
    map<string, character*> playerCharacters; // stores all the player characters
    map<string, character*> npcCharacters; // stores all npc characters

    character* mainPlayer; // stores a pointer to the main player character to reduce map finds

    int frameCount_4 = 0; // animation control for 4 frame
    int frameCount_6 = 0; // animation control for 6 frame

    map<pair<int,int>, npc> mapNpc;

#if defined(__APPLE__)
    //string talkPath = std::filesystem::current_path().string() + "/assets/other/talk.png";
    string talkPath = "../assets/other/talk.png";

#else
    string talkPath = "../assets/other/talk.png";
#endif
    imageHandler talk = imageHandler(talkPath.c_str());

public:
    ImVec2 drawPos;
    characterManager();

    void createCharacter(string name, bool npc, bool fullMovement, imageHandler* imgHandler);
    // name required to access the character
    // npc designates character type
    // full movement designates if all movement animations are requires or not

    map<pair<int,int>, npc>* getNpc();
    void setNpc(string configPath);

    character* getMainPlayer();
    // quickly get the main character to reduce map find calls
    void setMainPlayer(string name);
    // set the main character to animate
    // can be used to change the players character after game started
    void moveMainCharacter(imageHandler* imgHandler, characterBuilder* charBuild,float frameTimer, bool canMove, ImDrawList* draw_list);
    // used to move the main character based on keyboard input
    void selectMainCharacter(characterBuilder* charBuild);
    // used to save character item indexes

    void drawTalkBubble(int frame, float scale);
};