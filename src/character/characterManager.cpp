#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#if defined(__APPLE__)
#include <iostream>
#include <vector>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
#endif

#include "../imageHandler/imageHandler.h"

namespace ImGui { extern ImGuiKeyData* GetKeyData(ImGuiKey key); }

#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "characterManager.h"

characterManager::characterManager() {}

void characterManager::createCharacter(string name, bool npc, bool fullMovement, imageHandler* imgHandler)
{
    imagePath imgPath = imagePath(); // gets access to the class with all file paths
    auto charPath = imgPath.charIndex.find(name); // get the character to create
    if(charPath == imgPath.charIndex.end())
    {
        cout << "NOT FOUND" << endl;
        return;
    }
    string path = imgPath.absolutePath + charPath->second;

    character* newChar = new character(name, fullMovement, 32.f, 64.f); // create the character
    newChar->spriteSheet = new imageHandler();
    imgHandler->loadTexture(path.c_str(), newChar->spriteSheet);

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

    float factor = 1.f;
    //Draw Order: Body -> Eyes -> Outfit -> Hair -> Accessories
    switch(keyDown)
    {
        case 1:
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteBody, cordsWalkUp.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteEyes, cordsWalkUp.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteOutfit, cordsWalkUp.at(frameCount_6), factor);
            if(mainPlayer->spriteHair != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteHair, cordsWalkUp.at(frameCount_6), factor);
            }
            if(mainPlayer->spriteAccessories != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteAccessories, cordsWalkUp.at(frameCount_6), factor);
            }
            break;
        case 2:
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteBody, cordsWalkDown.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteEyes, cordsWalkDown.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteOutfit, cordsWalkDown.at(frameCount_6), factor);
            if(mainPlayer->spriteHair != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteHair, cordsWalkDown.at(frameCount_6), factor);
            }
            if(mainPlayer->spriteAccessories != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteAccessories, cordsWalkDown.at(frameCount_6), factor);
            }
            break;
        case 3:
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteBody, cordsWalkRight.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteEyes, cordsWalkRight.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteOutfit, cordsWalkRight.at(frameCount_6), factor);
            if(mainPlayer->spriteHair != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteHair, cordsWalkRight.at(frameCount_6), factor);
            }
            if(mainPlayer->spriteAccessories != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteAccessories, cordsWalkRight.at(frameCount_6), factor);
            }
            break;
        case 4:
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteBody, cordsWalkLeft.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteEyes, cordsWalkLeft.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteOutfit, cordsWalkLeft.at(frameCount_6), factor);
            if(mainPlayer->spriteHair != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteHair, cordsWalkLeft.at(frameCount_6), factor);
            }
            if(mainPlayer->spriteAccessories != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteAccessories, cordsWalkLeft.at(frameCount_6), factor);
            }
            break;
        default:
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteBody, cordsIdle.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteEyes, cordsIdle.at(frameCount_6), factor);
            ImGui::SetCursorPos(drawPos);
            imgHandler->DrawAnimationFrame(*mainPlayer->spriteOutfit, cordsIdle.at(frameCount_6), factor);
            if(mainPlayer->spriteHair != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteHair, cordsIdle.at(frameCount_6), factor);
            }
            if(mainPlayer->spriteAccessories != nullptr)
            {
                ImGui::SetCursorPos(drawPos);
                imgHandler->DrawAnimationFrame(*mainPlayer->spriteAccessories, cordsIdle.at(frameCount_6), factor);
            }
            break;
    }

    if (frameTimer <= 0.f)
    {
        frameCount_4 ++;
        frameCount_6 ++;
        if (frameCount_4 % 4 == 0) frameCount_4=0;
        if (frameCount_6 % 6 == 0) frameCount_6=0;
    }
}