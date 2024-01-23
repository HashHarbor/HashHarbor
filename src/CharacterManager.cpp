#include "CharacterManager.h"
#include "../imgui/imgui.h"
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui_internal.h"

#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers


#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::string;
using std::cout;
using std::endl;

#include "ImageHandler.h"
#include "ImagePath.h"

CharacterManager::CharacterManager() {}

void CharacterManager::createCharacter(std::string name, bool npc, bool fullMovement, ImageHandler* imgHandler)
{
    ImagePath paths = ImagePath(); // gets access to the class with all file paths
    auto charPath = paths.index.find(name); // get the character to create
    if(charPath == paths.index.end())
    {
        return;
    }

    character* newChar = new character(name, fullMovement); // create the character

    imgHandler->CreateAnimation(charPath->second.find("Idle")->second, newChar->idle);

    if(fullMovement) // if character has full movement create the animations
    {
        imgHandler->CreateAnimation(charPath->second.find("Up")->second, newChar->walkUp);
        imgHandler->CreateAnimation(charPath->second.find("Down")->second, newChar->walkDown);
        imgHandler->CreateAnimation(charPath->second.find("Right")->second, newChar->walkRight);
        imgHandler->CreateAnimation(charPath->second.find("Left")->second, newChar->walkLeft);
    }

    if(!npc)
        playerCharacters.emplace(name, newChar);
    else
        npcCharacters.emplace(name, newChar);
}

character* CharacterManager::getPlayerCharacter(string name)
{
    character* find = playerCharacters.find(name)->second;
    if(find == nullptr)
    { //todo - delete print statement
        std::cout << "FAIL TO FIND BOB" << std::endl;
    }
    cout << "I: " << find->idle.size()  << " |||| " << find->name<< endl;
    return find;
}

character* CharacterManager::getNpcCharacter(string name)
{
    character* find = npcCharacters.find(name)->second;
    return find;
}

character *CharacterManager::getMainPlayer()
{
    return mainPlayer;
}

void CharacterManager::setMainPlayer(std::string name)
{
    mainPlayer = playerCharacters.find(name)->second;
}
