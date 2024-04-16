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
//#include "characterManager.h"


class characterBuilder
{
    vector<imageHandler*> body;
    vector<imageHandler*> eyes;
    vector<vector<imageHandler*>> outfit;
    vector<vector<imageHandler*>> hair;
    vector<vector<imageHandler*>> accessories;

    int indexBody = 0;
    int indexEyes = 5;

    int indexOutfit = 8;
    int indexOutfitColor = 0;

    int indexHair = 2;
    int indexHairColor = 2;

    int indexAccessories = 0;
    int indexAccessoriesColor = 3;

    int frameCount_4 = 0;

    void drawCharacter(imageHandler* imgHandler, float frameTimer);
        //Internal Character Builder Function, will not and should not draw outside of character builder window
    void drawBodyEyeControl();
    void drawHairControls();
    void drawOutfitControls();
    void drawAccessoriesControl();

    void drawUI();

    void changeBody(int i);
    void changeEyes(int i);
    void changeOutfit(int i);
    void changeOutfitColor(int i);
    void changeHair(int i);
    void changeHairColor(int i);
    void changeAccessories(int i);
    void changeAccessoriesColor(int i);
public:
    ImVec2 drawPos;

    characterBuilder(imageHandler* imgHandler);
        // ONLY CONSTRUCT ONCE BECAUSE IT WILL USE A LOT OF RAM DUE TO LARGE ASSET AMOUNT BEING LOADED

    void drawCharacterBuilder(imageHandler* imgHandler, float frameTimer);

    void setAsMainCharacter(vector<int>& index);

    void drawCharacterAnimation(imageHandler* imgHandler, ImVec2 pos, pair<ImVec2,ImVec2> cords, float scale, vector<int>& characterIndex);
        // Character Index 0=Body, 1=Eyes, 2=Outfit, 3=OutfitColor, 4=Hair, 5=HairColor, 6=Accessories, 7=AccessoriesColor
    void drawCharacterAnimation(imageHandler* imgHandler, ImVec2 pos, pair<ImVec2,ImVec2> cords, float scale, vector<int>& characterIndex, float translucent);
        // used for npc to make them translucent
    void cleanUp(); // destructor to delete all image textures

    void setCharacterFromDb();
    void reset(); // used to reset to default character
};