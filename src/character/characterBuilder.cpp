#include "characterBuilder.h"
#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "characterManager.h"

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

characterBuilder::characterBuilder() {}

void characterBuilder::changeBody() {}
void characterBuilder::changeEyes() {}
void characterBuilder::chnageOutfit() {}
void characterBuilder::changeHair() {}
void characterBuilder::changeAccessorie() {}

void characterBuilder::drawDynamicCharacter() {}
