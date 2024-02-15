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

    imageHandler* spriteBody;
    imageHandler* spriteEyes;
    imageHandler* spriteOutfit;
    imageHandler* spriteHair;
    imageHandler* spriteAccessories;

    int dynamicIndex[8] = {0,0,0,0,0,0,0,0};

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

class characterManager
{
    map<string, character*> playerCharacters; // stores all the player characters
    map<string, character*> npcCharacters; // stores all npc characters

    character* mainPlayer; // stores a pointer to the main player character to reduce map finds

    // ----- Hardcoded Coordinates for Character Drawing ONLY ------ //
    vector<pair<ImVec2,ImVec2>> cordsIdle = {
            {ImVec2(0.f / 192.f, 0.f/320.f),ImVec2(32.f/192.f, 64.f/320.f)},
            {ImVec2(32.f / 192.f, 0.f/320.f),ImVec2(64.f/192.f, 64.f/320.f)},
            {ImVec2(64.f / 192.f, 0.f/320.f),ImVec2(96.f/192.f, 64.f/320.f)},
            {ImVec2(96.f / 192.f, 0.f/320.f),ImVec2(128.f/192.f, 64.f/320.f)},
            {ImVec2(128.f / 192.f, 0.f/320.f),ImVec2(160.f/192.f, 64.f/320.f)},
             {ImVec2(160.f / 192.f, 0.f/320.f),ImVec2(192.f/192.f, 64.f/320.f)}
    };
    vector<pair<ImVec2,ImVec2>> cordsWalkUp = {
            {ImVec2(0.f / 192.f, 64.f/320.f),ImVec2(32.f/192.f, 128.f/320.f)},
            {ImVec2(32.f / 192.f, 64.f/320.f),ImVec2(64.f/192.f, 128.f/320.f)},
            {ImVec2(64.f / 192.f, 64.f/320.f),ImVec2(96.f/192.f, 128.f/320.f)},
            {ImVec2(96.f / 192.f, 64.f/320.f),ImVec2(128.f/192.f, 128.f/320.f)},
            {ImVec2(128.f / 192.f, 64.f/320.f),ImVec2(160.f/192.f, 128.f/320.f)},
            {ImVec2(160.f / 192.f, 64.f/320.f),ImVec2(192.f/192.f, 128.f/320.f)}
    };
    vector<pair<ImVec2,ImVec2>> cordsWalkDown = {
            {ImVec2(0.f / 192.f, 128.f/320.f),ImVec2(32.f/192.f, 192.f/320.f)},
            {ImVec2(32.f / 192.f, 128.f/320.f),ImVec2(64.f/192.f, 192.f/320.f)},
            {ImVec2(64.f / 192.f, 128.f/320.f),ImVec2(96.f/192.f, 192.f/320.f)},
            {ImVec2(96.f / 192.f, 128.f/320.f),ImVec2(128.f/192.f, 192.f/320.f)},
            {ImVec2(128.f / 192.f, 128.f/320.f),ImVec2(160.f/192.f, 192.f/320.f)},
            {ImVec2(160.f / 192.f, 128.f/320.f),ImVec2(192.f/192.f, 192.f/320.f)}
    };
    vector<pair<ImVec2,ImVec2>> cordsWalkRight = {
            {ImVec2(0.f / 192.f, 192.f/320.f),ImVec2(32.f/192.f, 256.f/320.f)},
            {ImVec2(32.f / 192.f, 192.f/320.f),ImVec2(64.f/192.f, 256.f/320.f)},
            {ImVec2(64.f / 192.f, 192.f/320.f),ImVec2(96.f/192.f, 256.f/320.f)},
            {ImVec2(96.f / 192.f, 192.f/320.f),ImVec2(128.f/192.f, 256.f/320.f)},
            {ImVec2(128.f / 192.f, 192.f/320.f),ImVec2(160.f/192.f, 256.f/320.f)},
            {ImVec2(160.f / 192.f, 192.f/320.f),ImVec2(192.f/192.f, 256.f/320.f)}
    };
    vector<pair<ImVec2,ImVec2>> cordsWalkLeft = {
            {ImVec2(0.f / 192.f, 256.f/320.f),ImVec2(32.f/192.f, 320.f/320.f)},
            {ImVec2(32.f / 192.f, 256.f/320.f),ImVec2(64.f/192.f, 320.f/320.f)},
            {ImVec2(64.f / 192.f, 256.f/320.f),ImVec2(96.f/192.f, 320.f/320.f)},
            {ImVec2(96.f / 192.f, 256.f/320.f),ImVec2(128.f/192.f, 320.f/320.f)},
            {ImVec2(128.f / 192.f, 256.f/320.f),ImVec2(160.f/192.f, 320.f/320.f)},
            {ImVec2(160.f / 192.f, 256.f/320.f),ImVec2(192.f/192.f, 320.f/320.f)}
    };

    int frameCount_4 = 0; // animation control for 4 frame
    int frameCount_6 = 0; // animation control for 6 frame
public:
    ImVec2 drawPos;
    characterManager();

    void createCharacter(string name, bool npc, bool fullMovement, imageHandler* imgHandler);
    // name required to access the character
    // npc designates character type
    // full movement designates if all movement animations are requires or not

    character* getPlayerCharacter(string name);
    character* getNpcCharacter(string name);

    character* getMainPlayer();
    // quickly get the main character to reduce map find calls
    void setMainPlayer(string name);
    // set the main character to animate
    // can be used to change the players character after game started
    void moveMainCharacter(imageHandler* imgHandler, characterBuilder* charBuild,float frameTimer);
    // used to move the main character based on keyboard input
    void selectMainCharacter(characterBuilder* charBuild);
    // used to save character item indexes
};