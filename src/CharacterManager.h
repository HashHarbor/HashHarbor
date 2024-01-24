#pragma once

#define GL_SILENCE_DEPRECATION

#include "imgui.h"
//#include "../backends/imgui_impl_glfw.h"
#include "SDL2/SDL.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include <iostream>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::map;
#include "ImageHandler.h"
#include "ImagePath.h"

struct character
{
public:
    string name; // character name used for identification
    bool fullMovement; // used to differentiate player or npc that needs all animations and npc that just idles
    /* --- Animation Frames --- */
    vector<image*> idle;
    vector<image*> walkUp;
    vector<image*> walkDown;
    vector<image*> walkRight;
    vector<image*> walkLeft;

    character(string name, bool fullMovement)
    {
        this->name = name;
        this->fullMovement = fullMovement;
    }
};

class CharacterManager
{
    map<string, character*> playerCharacters; // stores all the player characters
    map<string, character*> npcCharacters; // stores all npc characters

    character* mainPlayer; // stores a pointer to the main player character to reduce map finds

    int frameCount_4 = 0; // animation control for 4 frame
    int frameCount_6 = 0; // animation control for 6 frame
public:
    CharacterManager();

    void createCharacter(string name, bool npc, bool fullMovement, ImageHandler* imgHandler);
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
    void moveMainCharacter(ImageHandler* imgHandler, float frameTimer);
    // used to move the main character based on keyboard input

};