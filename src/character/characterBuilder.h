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
    vector<imageHandler*> hair;
    vector<imageHandler*> accessories;

    int indexBody;
    int indexEyes;
    int indexOutfit;
    int indexHair;
    int indexAccessories;
public:
    characterBuilder();

    void drawDynamicCharacter();
    // function to draw everything

    void changeBody();
    void changeEyes();
    void chnageOutfit();
    void changeHair();
    void changeAccessorie();

    void setAsMainCharacter();
};