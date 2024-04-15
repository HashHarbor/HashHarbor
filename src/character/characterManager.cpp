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
#include "characterBuilder.h"
#include "configReader/configReader.h"

namespace ImGui { extern ImGuiKeyData* GetKeyData(ImGuiKey key); }


#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "characterManager.h"
#include "characterConfig.h"

characterManager::characterManager() {}

void characterManager::createCharacter(string name, bool npc, bool fullMovement, imageHandler* imgHandler)
{
    character* newChar = new character(name, fullMovement, 32.f, 64.f); // create the character


    if(!npc)
        playerCharacters.emplace(name, newChar);
    else
        npcCharacters.emplace(name, newChar);
}

character *characterManager::getMainPlayer()
{
    return mainPlayer;
}

void characterManager::setMainPlayer(std::string name)
{
    mainPlayer = playerCharacters.find(name)->second;
}

void characterManager::moveMainCharacter(imageHandler* imgHandler, characterBuilder* charBuild,float frameTimer, bool canMove, ImDrawList* draw_list)
{


    int keyDown = 0; // used to identify which direction the character is moving
    static int previousKey = 2;
    if(canMove)
    {
        if(ImGui::IsKeyDown(ImGuiKey_W)) { keyDown = 1; }
        else if(ImGui::IsKeyDown(ImGuiKey_S)) { keyDown = 2; }
        else if(ImGui::IsKeyDown(ImGuiKey_D)) { keyDown = 3; }
        else if(ImGui::IsKeyDown(ImGuiKey_A)) { keyDown = 4; }
    }

    if(keyDown != 0)
    {
        previousKey = keyDown;
    }

    float factor = 1.f;
    characterConfig cords;
    //Draw Order: Body -> Eyes -> Outfit -> Hair -> Accessories
    switch(keyDown)
    {
        case 1: // UP
            draw_list->AddRectFilled(ImVec2(drawPos.x + 0.f, drawPos.y + 55.f), ImVec2(drawPos.x + 32.f, drawPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
            charBuild->drawCharacterAnimation(imgHandler,drawPos,cords.cordsWalkUp.at(frameCount_6),factor,mainPlayer->dynamicIndex);
            break;
        case 2: // DOWN
            draw_list->AddRectFilled(ImVec2(drawPos.x + 0.f, drawPos.y + 53.f), ImVec2(drawPos.x + 32.f, drawPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
            charBuild->drawCharacterAnimation(imgHandler,drawPos,cords.cordsWalkDown.at(frameCount_6),factor,mainPlayer->dynamicIndex);
            break;
        case 3: // RIGHT
            draw_list->AddRectFilled(ImVec2(drawPos.x + 2.f, drawPos.y + 55.f), ImVec2(drawPos.x + 32.f, drawPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
            charBuild->drawCharacterAnimation(imgHandler,drawPos,cords.cordsWalkRight.at(frameCount_6),factor,mainPlayer->dynamicIndex);
            break;
        case 4: // LEFT
            draw_list->AddRectFilled(ImVec2(drawPos.x + 0.f, drawPos.y + 55.f), ImVec2(drawPos.x + 30.f, drawPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
            charBuild->drawCharacterAnimation(imgHandler,drawPos,cords.cordsWalkLeft.at(frameCount_6),factor,mainPlayer->dynamicIndex);
            break;
        default:
            switch(previousKey)
            {
                case 1:
                    draw_list->AddRectFilled(ImVec2(drawPos.x + 0.f, drawPos.y + 55.f), ImVec2(drawPos.x + 32.f, drawPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                    charBuild->drawCharacterAnimation(imgHandler,drawPos,cords.cordsIdleUp.at(frameCount_6),factor,mainPlayer->dynamicIndex);
                    break;
                case 2:
                    draw_list->AddRectFilled(ImVec2(drawPos.x + 0.f, drawPos.y + 55.f), ImVec2(drawPos.x + 32.f, drawPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                    charBuild->drawCharacterAnimation(imgHandler,drawPos,cords.cordsIdleDown.at(frameCount_6),factor,mainPlayer->dynamicIndex);
                    break;
                case 3:
                    draw_list->AddRectFilled(ImVec2(drawPos.x + 5.f, drawPos.y + 55.f), ImVec2(drawPos.x + 27.f, drawPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                    charBuild->drawCharacterAnimation(imgHandler,drawPos,cords.cordsIdleRight.at(frameCount_6),factor,mainPlayer->dynamicIndex);
                    break;
                case 4:
                    draw_list->AddRectFilled(ImVec2(drawPos.x + 5.f, drawPos.y + 55.f), ImVec2(drawPos.x + 27.f, drawPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                    charBuild->drawCharacterAnimation(imgHandler,drawPos,cords.cordsIdleLeft.at(frameCount_6),factor,mainPlayer->dynamicIndex);
                    break;
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

void characterManager::selectMainCharacter(characterBuilder* charBuild)
{
    charBuild->setAsMainCharacter(mainPlayer->dynamicIndex);

}

map<pair<int,int>, npc>* characterManager::getNpc()
{
    return &mapNpc;
}

void characterManager::setNpc(string configPath)
{
    mapNpc.clear();

    configReader config = configReader();
    config.getNpc(configPath, this);
}
