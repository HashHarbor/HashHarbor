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
    vector<vector<imageHandler*>> outfit;
    vector<vector<imageHandler*>> hair;
    vector<imageHandler*> accessories;

    vector<vector<ImColor>> outfitButtonColor = {
            //outfit 1
            {ImColor::HSV(290.f / 360.f,0.31f,0.659f), ImColor::HSV(295.f / 360.f,0.25f,0.784f), ImColor::HSV(283.f / 360.f,0.338f,0.557f), // 1.1
             ImColor::HSV(217.f / 360.f,0.189f,0.939f), ImColor::HSV(260.f / 360.f,0.074f,0.957f), ImColor::HSV(226.f / 360.f,0.288f,0.914f), // 1.2
             ImColor::HSV(39.f / 360.f,0.25f,0.816f), ImColor::HSV(28.f / 360.f,0.246f,0.749f), ImColor::HSV(18.f / 360.f,0.531f,0.769f), // 1.3
             ImColor::HSV(2.f / 360.f,0.757f,0.902f), ImColor::HSV(0.f / 360.f,0.742,0.761f), ImColor::HSV(0.f / 360.f,0.77f,0.851f), // 1.4
             ImColor::HSV(154.f / 360.f,0.325f,0.471f), ImColor::HSV(119.f / 360.f,0.372f,0.537f), ImColor::HSV(177.f / 360.f,0.745f,0.384f), // 1.5
             ImColor::HSV(9.f / 360.f,0.518f,0.894f), ImColor::HSV(9.f / 360.f,0.625f,0.816f), ImColor::HSV(12.f / 360.f,0.606f,0.627f), // 1.6
             ImColor::HSV(339.f / 360.f,0.439f,0.894f), ImColor::HSV(317.f / 360.f,0.534f,0.816f), ImColor::HSV(309.f / 360.f,0.726f,0.918f), // 1.7
             ImColor::HSV(51.f / 360.f,0.586f,0.635f), ImColor::HSV(39.f / 360.f,0.584f,0.584f), ImColor::HSV(36.f / 360.f,0.8f,0.588f), // 1.8
             ImColor::HSV(175.f / 360.f,0.143f,0.957f), ImColor::HSV(197.f / 360.f,0.222f,0.882f), ImColor::HSV(189.f / 360.f,0.104f,0.718f), // 1.9
             ImColor::HSV(343.f / 360.f,0.568f,0.663f), ImColor::HSV(349.f / 360.f,0.676f,0.882f), ImColor::HSV(329.f / 360.f,0.546f,0.51f)},  // 1.10
             // outfit 2
            {ImColor::HSV(355.f / 360.f,0.575f,0.682f), ImColor::HSV(354.f / 360.f,0.679f,0.855f), ImColor::HSV(350.f / 360.f,0.648f,0.635f), // 2.1
             ImColor::HSV(12.f / 360.f,0.526f,0.596f), ImColor::HSV(23.f / 360.f,0.798f,0.796f), ImColor::HSV(354.f / 360.f,0.528f,0.565f), // 2.2
             ImColor::HSV(47.f / 360.f,0.56f,0.784f), ImColor::HSV(56.f / 360.f,0.548f,0.867f), ImColor::HSV(46.f / 360.f,0.598f,0.663f), // 2.3
             ImColor::HSV(189.f / 360.f,0.788f,0.851f), ImColor::HSV(185.f / 360.f,0.424f,0.851f), ImColor::HSV(191.f / 360.f,0.598f,0.663f)}, // 2.4
             // outfit 4
            {ImColor::HSV(105.f / 360.f,0.34f,0.60f), ImColor::HSV(106.f / 360.f,0.243f,0.741f), ImColor::HSV(143.f / 360.f,0.341f,0.518f), // 4.1
             ImColor::HSV(28.f / 360.f,0.553f,0.78f), ImColor::HSV(28.f / 360.f,0.31f,0.824f), ImColor::HSV(25.f / 360.f,0.587f,0.702f), // 4.2
             ImColor::HSV(202.f / 360.f,0.272f,0.582f), ImColor::HSV(224.f / 360.f,0.259f,0.773f), ImColor::HSV(211.f / 360.f,0.418f,0.478f)}, //4.3
             // outfit 10
            {ImColor::HSV(207.f / 360.f,0.12f,0.718f), ImColor::HSV(231.f / 360.f,0.153f,0.667f), ImColor::HSV(210.f / 360.f,0.377f,0.686f), // 10.1
             ImColor::HSV(195.f / 360.f,0.441f,0.765f), ImColor::HSV(207.f / 360.f,0.41f,0.718f), ImColor::HSV(204.f / 360.f,0.764f,0.831f), // 10.2
             ImColor::HSV(359.f / 360.f,0.637f,0.714f), ImColor::HSV(348.f / 360.f,0.699f,0.651f), ImColor::HSV(349.f / 360.f,0.787f,0.588f), // 10.3
             ImColor::HSV(356.f / 360.f,0.108f,0.616f), ImColor::HSV(257.f / 360.f,0.074f,0.737f), ImColor::HSV(349.f / 360.f,0.153f,0.537f), // 10.4
             ImColor::HSV(104.f / 360.f,0.299f,0.788f), ImColor::HSV(134.f / 360.f,0.446f,0.58f), ImColor::HSV(100.f / 360.f,0.298f,0.671f)}, //10/5
             //outfit 24
            {ImColor::HSV(201.f / 360.f,0.1f,0.89f), ImColor::HSV(201.f / 360.f,0.375f,0.941f), ImColor::HSV(210.f / 360.f,0.958f,0.831f), // 24.1
             ImColor::HSV(174.f / 360.f,0.473f,0.812f), ImColor::HSV(201.f / 360.f,0.254f,0.957f), ImColor::HSV(186.f / 360.f,0.51f,0.784f), // 24.2
             ImColor::HSV(180.f / 360.f,0.100f,0.573f), ImColor::HSV(194.f / 360.f,0.389f,0.867f), ImColor::HSV(201.f / 360.f,0.49f,0.408f), // 24.3
             ImColor::HSV(278.f / 360.f,0.338f,0.776f), ImColor::HSV(340.f / 360.f,0.319f,0.984f), ImColor::HSV(277.f / 360.f,0.525f,0.784f)} // 24.3
    };
    // ImColor::HSV(0.f / 360.f,0.f,0.f), ImColor::HSV(0.f / 360.f,0.f,0.f), ImColor::HSV(0.f / 360.f,0.f,0.f)

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
    void drawBodyEyeControl();
    void drawHairControls();
    void drawOutfitControls();
    void drawAccessoriesControl();

    void changeBody(int i);
    void changeEyes(int i);
    void changeOutfit(int i);
    void changeOutfitColor(int i);
    void changeHair(int i);
    void changeHairColor(int i);
    void changeAccessories(int i);
public:
    ImVec2 drawPos;

    characterBuilder(imageHandler* imgHandler);
        // ONLY CONSTRUCT ONCE BECAUSE IT WILL USE A LOT OF VRAM DUE TO LARGE ASSET AMOUNT BEING LOADED

    void drawCharacterBuilder(imageHandler* imgHandler, float frameTimer);

    void setAsMainCharacter();
};