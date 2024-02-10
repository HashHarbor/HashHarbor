#pragma once

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#if defined(__APPLE__)
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
#endif

#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "characterManager.h"


class characterBuilder
{
    // display body
    // display eyes
    // display outfit
    // display hair
    // display accessories

    //build char / save to character manager
    vector<imageHandler*> body;
    vector<imageHandler*> eyes;
    vector<imageHandler*> outfit;
    vector<vector<imageHandler*>> hair;
    vector<imageHandler*> accessories;

    int indexBody = 0;
    int indexEyes = 0;

    int indexOutfit = 0;
    int indexOutfitColor = 0;

    int indexHair = 0;
    int indexHairColor = 0;

    int indexAccessories = 0;

    int frameCount_4 = 0;

    vector<pair<ImVec2,ImVec2>> cordsAnim = {
            {ImVec2(0.f / 192.f, 0.f/320.f),ImVec2(32.f/192.f, 64.f/320.f)},
            {ImVec2(160.f / 192.f, 256.f/320.f),ImVec2(192.f/192.f, 320.f/320.f)},
            {ImVec2(64.f / 192.f, 64.f/320.f),ImVec2(96.f/192.f, 128.f/320.f)},
            {ImVec2(160.f / 192.f, 192.f/320.f),ImVec2(192.f/192.f, 256.f/320.f)}
    };

    void drawCharacter(imageHandler* imgHandler, float frameTimer);
    void drawBodyEyeContorl();
    void drawHairControls();
    void drawOutfitControls();
    void drawAccessoriesControl();
public:
    ImVec2 drawPos;

    characterBuilder(imageHandler* imgHandler);
        // ONLY CONSTRUCT ONCE BECAUSE IT WILL USE A LOT OF VRAM DUE TO LARGE ASSET AMOUNT BEING LOADED

    void changeBody(int i);
    void changeEyes(int i);
    void changeOutfit(int i);
    void changeHair(int i);
    void changeHairColor(int i);
    void changeAccessories(int i);
    void drawCharacterBuilder(imageHandler* imgHandler, float frameTimer);

    void setAsMainCharacter();
};