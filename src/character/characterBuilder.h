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
             // outfit 5
            {ImColor::HSV(235.f / 360.f,0.188f,0.522f), ImColor::HSV(219.f / 360.f,0.211f,0.835f), ImColor::HSV(234.f / 360.f,0.246f,0.447f), // 5.1
             ImColor::HSV(205.f / 360.f,0.421f,0.522f), ImColor::HSV(203.f / 360.f,0.579f,0.447f), ImColor::HSV(240.f / 360.f,0.271f,0.376f), // 5.2
             ImColor::HSV(148.f / 360.f,0.267f,0.412f), ImColor::HSV(146.f / 360.f,0.266f,0.369f), ImColor::HSV(135.f / 360.f,0.450f,0.392f), // 5.3
             ImColor::HSV(30.f / 360.f,0.591f,0.863f), ImColor::HSV(34.f / 360.f,0.152f,0.875f), ImColor::HSV(22.f / 360.f,0.547f,0.788f), // 5.4
             ImColor::HSV(345.f / 360.f,0.223f,0.937f), ImColor::HSV(330.f / 360.f,0.156f,0.855f), ImColor::HSV(338.f / 360.f,0.138f,0.624f)}, // 5.5
             // outfit 6
            {ImColor::HSV(235.f / 360.f,0.188f,0.522f), ImColor::HSV(234.f / 360.f,0.246f,0.447f), ImColor::HSV(240.f / 360.f,0.255f,0.369f), // 6.1
             ImColor::HSV(200.f / 360.f,0.027f,0.435f), ImColor::HSV(222.f / 360.f,0.104f,0.376f), ImColor::HSV(140.f / 360.f,0.074f,0.318f), // 6.2
             ImColor::HSV(291.f / 360.f,0.189f,0.435f), ImColor::HSV(267.f / 360.f,0.187f,0.376f), ImColor::HSV(259.f / 360.f,0.201f,0.361f), // 6.3
             ImColor::HSV(209.f / 360.f,0.480f,0.596f), ImColor::HSV(216.f / 360.f,0.549f,0.557f), ImColor::HSV(240.f / 360.f,0.275f,0.314f)}, // 6.4
             // outfit 8
            {ImColor::HSV(270.f / 360.f,0.058f,0.949f), ImColor::HSV(0.f / 360.f,0.f,0.973f), ImColor::HSV(270.f / 360.f,0.071f,0.878f),
             ImColor::HSV(280.f / 360.f,0.07f,0.675f), ImColor::HSV(317.f / 360.f,0.038f,0.729f), ImColor::HSV(252.f / 360.f,0.65f,0.608f),
             ImColor::HSV(276.f / 360.f,0.043f,0.451f), ImColor::HSV(313.f / 360.f,0.069f,0.510f), ImColor::HSV(252.f / 360.f,0.049f,0.404f)},
             // outfit 9
            {ImColor::HSV(2.f / 360.f,0.757f,0.902f), ImColor::HSV(7.f / 360.f,0.722f,0.988f), ImColor::HSV(0.f / 360.f,0.77f,0.851f),
             ImColor::HSV(40.f / 360.f,0.757f,0.902f), ImColor::HSV(29.f / 360.f,0.77f,0.851f), ImColor::HSV(33.f / 360.f,0.715f,0.620f),
             ImColor::HSV(176.f / 360.f,0.480f,0.678f), ImColor::HSV(184.f / 360.f,0.434f,0.624f), ImColor::HSV(210.f / 360.f,0.365f,0.494f)},
             // outfit 10
            {ImColor::HSV(207.f / 360.f,0.12f,0.718f), ImColor::HSV(231.f / 360.f,0.153f,0.667f), ImColor::HSV(210.f / 360.f,0.377f,0.686f), // 10.1
             ImColor::HSV(195.f / 360.f,0.441f,0.765f), ImColor::HSV(207.f / 360.f,0.41f,0.718f), ImColor::HSV(204.f / 360.f,0.764f,0.831f), // 10.2
             ImColor::HSV(359.f / 360.f,0.637f,0.714f), ImColor::HSV(348.f / 360.f,0.699f,0.651f), ImColor::HSV(349.f / 360.f,0.787f,0.588f), // 10.3
             ImColor::HSV(356.f / 360.f,0.108f,0.616f), ImColor::HSV(257.f / 360.f,0.074f,0.737f), ImColor::HSV(349.f / 360.f,0.153f,0.537f), // 10.4
             ImColor::HSV(104.f / 360.f,0.299f,0.788f), ImColor::HSV(134.f / 360.f,0.446f,0.58f), ImColor::HSV(100.f / 360.f,0.298f,0.671f)}, //10.5
             // outfit 11
            {ImColor::HSV(287.f / 360.f,0.171f,0.851f), ImColor::HSV(287.f / 360.f,0.116f,0.949f), ImColor::HSV(281.f / 360.f,0.221f,0.80f), // 11.1
             ImColor::HSV(32.f / 360.f,0.297f,0.82f), ImColor::HSV(17.f / 360.f,0.153f,0.925f), ImColor::HSV(32.f / 360.f,0.347f,0.745f), // 11.2
             ImColor::HSV(275.f / 360.f,0.088f,0.851f), ImColor::HSV(286.f / 360.f,0.190f,0.722f), ImColor::HSV(262.f / 360.f,0.108f,0.80), // 11.3
             ImColor::HSV(56.f / 360.f,0.612f,0.627f), ImColor::HSV(58.f / 360.f,0.173f,0.769f), ImColor::HSV(44.f / 360.f,0.747f,0.635f)}, // 11.4
             // outfit 16
            {ImColor::HSV(40.f / 360.f,0.918f,0.961f), ImColor::HSV(34.f / 360.f,0.873f,0.929f), ImColor::HSV(23.f / 360.f,0.616f,0.100f), // 16.1
             ImColor::HSV(21.f / 360.f,0.678f,0.961f), ImColor::HSV(15.f / 360.f,0.667f,0.929f), ImColor::HSV(25.f / 360.f,0.079f,0.839f), // 16.2
             ImColor::HSV(7.f / 360.f,0.684f,0.882f), ImColor::HSV(355.f / 360.f,0.707f,0.871f), ImColor::HSV(357.f / 360.f,0.737f,0.953f)}, // 16.3
             // outfit 18
            {ImColor::HSV(26.f / 360.f,0.605f,0.824f), ImColor::HSV(17.f / 360.f,0.544f,0.765f), ImColor::HSV(357.f / 360.f,0.373f,0.557f), // 18.1
             ImColor::HSV(28.f / 360.f,0.276f,0.725f), ImColor::HSV(22.f / 360.f,0.345f,0.694f), ImColor::HSV(352.f / 360.f,0.426f,0.533f), // 18.2
             ImColor::HSV(358.f / 360.f,0.581f,0.824f), ImColor::HSV(355.f / 360.f,0.590f,0.765f), ImColor::HSV(357.f / 360.f,0.373f,0.557f), // 18.3
             ImColor::HSV(201.f / 360.f,0.547f,0.675f), ImColor::HSV(203.f / 360.f,0.553f,0.588f), ImColor::HSV(192.f / 360.f,0.163f,0.361f)}, // 18.4
             // outfit 20
            {ImColor::HSV(233.f / 360.f,0.470f,0.518f), ImColor::HSV(233.f / 360.f,0.215f,0.875f), ImColor::HSV(191.f / 360.f,0.586f,0.682f), // 20.1
             ImColor::HSV(227.f / 360.f,0.116f,0.812f), ImColor::HSV(227.f / 360.f,0.531f,0.769f), ImColor::HSV(234.f / 360.f,0.159f,0.765f), // 20.2
             ImColor::HSV(157.f / 360.f,0.633f,0.576f), ImColor::HSV(155.f / 360.f,0.637f,0.671f), ImColor::HSV(104.f / 360.f,0.245f,0.369f)}, // 20.3
             // outfit 21
            {ImColor::HSV(256.f / 360.f,0.342f,0.620f), ImColor::HSV(273.f / 360.f,0.341f,0.655f), ImColor::HSV(247.f / 360.f,0.396f,0.604f), // 21.1
             ImColor::HSV(256.f / 360.f,0.223f,0.792f), ImColor::HSV(273.f / 360.f,0.085f,0.831f), ImColor::HSV(247.f / 360.f,0.235f,0.718f), // 21.2
             ImColor::HSV(190.f / 360.f,0.582f,0.620f), ImColor::HSV(229.f / 360.f,0.290f,0.784f), ImColor::HSV(199.f / 360.f,0.623f,0.604f), // 21.3
             ImColor::HSV(19.f / 360.f,0.689f,0.882f), ImColor::HSV(13.f / 360.f,0.389f,0.725f), ImColor::HSV(8.f / 360.f,0.639f,0.816f)}, // 21.4
             // outfit 24
            {ImColor::HSV(201.f / 360.f,0.1f,0.89f), ImColor::HSV(201.f / 360.f,0.375f,0.941f), ImColor::HSV(210.f / 360.f,0.958f,0.831f), // 24.1
             ImColor::HSV(174.f / 360.f,0.473f,0.812f), ImColor::HSV(201.f / 360.f,0.254f,0.957f), ImColor::HSV(186.f / 360.f,0.51f,0.784f), // 24.2
             ImColor::HSV(180.f / 360.f,0.100f,0.573f), ImColor::HSV(194.f / 360.f,0.389f,0.867f), ImColor::HSV(201.f / 360.f,0.49f,0.408f), // 24.3
             ImColor::HSV(278.f / 360.f,0.338f,0.776f), ImColor::HSV(340.f / 360.f,0.319f,0.984f), ImColor::HSV(277.f / 360.f,0.525f,0.784f)} // 24.3
    };
    // ImColor::HSV(0.f / 360.f,0.f,0.f), ImColor::HSV(0.f / 360.f,0.f,0.f), ImColor::HSV(0.f / 360.f,0.f,0.f)

    vector<vector<ImColor>> accessoriesButtonColor = {
            // backpack
            {ImColor::HSV(30.f / 360.f,0.289f,0.624f), ImColor::HSV(39.f / 360.f,0.25f,0.816f), ImColor::HSV(18.f / 360.f,0.376f,0.49f), // 1
             ImColor::HSV(55.f / 360.f,0.37f,0.784f), ImColor::HSV(37.f / 360.f,0.533f,0.706f), ImColor::HSV(18.f / 360.f,0.376f,0.49f), // 2
             ImColor::HSV(107.f / 360.f,0.518f,0.651f), ImColor::HSV(79.f / 360.f,0.639f,0.761f), ImColor::HSV(156.f / 360.f,0.291f,0.431f), // 3
             ImColor::HSV(142.f / 360.f,0.53f,0.776f), ImColor::HSV(149.f / 360.f,0.659f,0.69f), ImColor::HSV(166.f / 360.f,0.694f,0.627f), // 4
             ImColor::HSV(2.f / 360.f,0.757f,0.902f), ImColor::HSV(7.f / 360.f,0.722f,0.988f), ImColor::HSV(359.f / 360.f,0.744f,0.659f), // 5
             ImColor::HSV(206.f / 360.f,0.655f,0.91f), ImColor::HSV(216.f / 360.f,0.701f,0.867f), ImColor::HSV(230.f / 360.f,0.71f,0.824f), // 6
             ImColor::HSV(344.f / 360.f,0.307f,0.933f), ImColor::HSV(339.f / 360.f,0.392f,0.91f), ImColor::HSV(336.f / 360.f,0.444f,0.839f), // 7
             ImColor::HSV(217.f / 360.f,0.115f,0.784f), ImColor::HSV(226.f / 360.f,0.142f,0.718f), ImColor::HSV(240.f / 360.f,0.187f,0.671f), // 8
             ImColor::HSV(266.f / 360.f,0.569f,0.773f), ImColor::HSV(46.f / 360.f,0.667f,0.706f), ImColor::HSV(38.f / 360.f,0.722f,0.62f), // 9
             ImColor::HSV(206.f / 360.f,0.655f,0.91f), ImColor::HSV(50.f / 360.f,0.655f,0.91f), ImColor::HSV(44.f / 360.f,0.751f,0.835f)}, // 10
             // Beanie
            {ImColor::HSV(229.f / 360.f,0.27f,0.553f), ImColor::HSV(229.f / 360.f,0.098f,0.682f), ImColor::HSV(215.f / 360.f,0.274f,0.416f), // 1
             ImColor::HSV(22.f / 360.f,0.115f,0.647f), ImColor::HSV(16.f / 360.f,0.176f,0.58f), ImColor::HSV(163.f / 360.f,0.532f,0.427f), // 2
             ImColor::HSV(326.f / 360.f,0.267f,0.647f), ImColor::HSV(331.f / 360.f,0.345f,0.580f), ImColor::HSV(324.f / 360.f,0.503f,0.561f), // 3
             ImColor::HSV(186.f / 360.f,0.446f,0.616f), ImColor::HSV(197.f / 360.f,0.493f,0.596f), ImColor::HSV(207.f / 360.f,0.522f,0.541f), // 4
             ImColor::HSV(8.f / 360.f,0.525f,0.627f), ImColor::HSV(0.f / 360.f,0.423f,0.557f), ImColor::HSV(349.f / 360.f,0.432f,0.49f)}, // 5
             // Beard
            {ImColor::HSV(8.f / 360.f,0.525f,0.627f), ImColor::HSV(0.f / 360.f,0.423f,0.557f), ImColor::HSV(349.f / 360.f,0.432f,0.49f)},
            // Chef Hat
            {ImColor::HSV(8.f / 360.f,0.525f,0.627f), ImColor::HSV(0.f / 360.f,0.423f,0.557f), ImColor::HSV(349.f / 360.f,0.432f,0.49f)},
            // Dino Hat
            {ImColor::HSV(8.f / 360.f,0.525f,0.627f), ImColor::HSV(0.f / 360.f,0.423f,0.557f), ImColor::HSV(349.f / 360.f,0.432f,0.49f)}
    };

    int indexBody = 0;
    int indexEyes = 0;

    int indexOutfit = 0;
    int indexOutfitColor = 0;

    int indexHair = 0;
    int indexHairColor = 0;

    int indexAccessories = 0;
    int indexAccessoriesColor = 0;

    int frameCount_4 = 0;

    vector<pair<ImVec2,ImVec2>> cordsAnim = {
            {ImVec2(0.f / 192.f, 0.f/320.f),ImVec2(32.f/192.f, 64.f/320.f)},
            {ImVec2(160.f / 192.f, 256.f/320.f),ImVec2(192.f/192.f, 320.f/320.f)},
            {ImVec2(64.f / 192.f, 64.f/320.f),ImVec2(96.f/192.f, 128.f/320.f)},
            {ImVec2(160.f / 192.f, 192.f/320.f),ImVec2(192.f/192.f, 256.f/320.f)}
    };

    void drawCharacter(imageHandler* imgHandler, float frameTimer);
        //Internal Character Builder Function, will not and should not draw outside of character builder window
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
    void changeAccessoriesColor(int i);
public:
    ImVec2 drawPos;

    characterBuilder(imageHandler* imgHandler);
        // ONLY CONSTRUCT ONCE BECAUSE IT WILL USE A LOT OF RAM DUE TO LARGE ASSET AMOUNT BEING LOADED

    void drawCharacterBuilder(imageHandler* imgHandler, float frameTimer);

    int* setAsMainCharacter();

    void drawCharacterAnimation(imageHandler* imgHandler, ImVec2 pos, pair<ImVec2,ImVec2> cords, float scale, int characterIndex[8]);
        // Character Index 0=Body, 1=Eyes, 2=Outfit, 3=OutfitColor, 4=Hair, 5=HairColor, 6=Accessories, 7=AccessoriesColor

    void cleanUp();
        // destructor to delete all image textures
};