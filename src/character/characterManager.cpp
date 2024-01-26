#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <iostream>
#include <vector>
#include <map>

namespace ImGui { extern ImGuiKeyData* GetKeyData(ImGuiKey key); }

using std::vector;
using std::string;
using std::cout;
using std::endl;

#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "characterManager.h"

characterManager::characterManager() {}

void characterManager::createCharacter(std::string name, bool npc, bool fullMovement, imageHandler* imgHandler)
{
    imagePath paths = imagePath(); // gets access to the class with all file paths
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

character* characterManager::getPlayerCharacter(string name)
{
    character* find = playerCharacters.find(name)->second;
    if(find == nullptr)
    { //todo - delete print statement
        std::cout << "FAIL TO FIND BOB" << std::endl;
    }
    cout << "I: " << find->idle.size()  << " |||| " << find->name<< endl;
    return find;
}

character* characterManager::getNpcCharacter(string name)
{
    character* find = npcCharacters.find(name)->second;
    return find;
}

character *characterManager::getMainPlayer()
{
    return mainPlayer;
}

void characterManager::setMainPlayer(std::string name)
{
    mainPlayer = playerCharacters.find(name)->second;
}

void characterManager::moveMainCharacter(imageHandler* imgHandler, float frameTimer)
{
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
    struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
            const ImGuiKey key_first = ImGuiKey_NamedKey_BEGIN;
#else
    struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } };
    // Hide Native<>ImGuiKey duplicates when both exists in the array
    const ImGuiKey key_first = 0;
    //ImGui::Text("Legacy raw:");       for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { if (io.KeysDown[key]) { ImGui::SameLine(); ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key); } }
#endif

    int keyDown = 0; // used to identify which direction the character is moving
    for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++)
    {
        if (funcs::IsLegacyNativeDupe(key)) continue;
        if(ImGui::IsKeyDown(ImGuiKey_UpArrow) || ImGui::IsKeyDown(ImGuiKey_W)) { keyDown = 1; }
        else if(ImGui::IsKeyDown(ImGuiKey_DownArrow) || ImGui::IsKeyDown(ImGuiKey_S)) { keyDown = 2; }
        else if(ImGui::IsKeyDown(ImGuiKey_RightArrow) || ImGui::IsKeyDown(ImGuiKey_D)) { keyDown = 3; }
        else if(ImGui::IsKeyDown(ImGuiKey_LeftArrow) || ImGui::IsKeyDown(ImGuiKey_A)) { keyDown = 4; }
        else { keyDown = 0; }
    }

    switch(keyDown)
    {
        case 1:
            imgHandler->DrawImage(*mainPlayer->walkUp.at(frameCount_6));
            break;
        case 2:
            imgHandler->DrawImage(*mainPlayer->walkDown.at(frameCount_6));
            break;
        case 3:
            imgHandler->DrawImage(*mainPlayer->walkRight.at(frameCount_6));
            break;
        case 4:
            imgHandler->DrawImage(*mainPlayer->walkLeft.at(frameCount_6));
            break;
        default:
            imgHandler->DrawImage(*mainPlayer->idle.at(frameCount_4));
            break;
    }

    if (frameTimer <= 0.f)
    {
        frameCount_4 ++;
        frameCount_6 ++;
        if (frameCount_4 % 4 == 0) frameCount_4=0;
        if (frameCount_6 % 6 == 0) frameCount_6=0;
    }

    //todo - remove on screen key printout
    //ImGui::Text("Keys down:"); for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { if (funcs::IsLegacyNativeDupe(key)) continue; if (ImGui::IsKeyDown(key)) { ImGui::SameLine(); ImGui::Text("\"%s\" %d (%.02f secs)", ImGui::GetKeyName(key), key, ImGui::GetKeyData(key)->DownDuration); } }
}