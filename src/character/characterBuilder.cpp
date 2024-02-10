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

characterBuilder::characterBuilder(imageHandler* imgHandler)
{
    imagePath imgPath = imagePath();

    for(auto & iter : imgPath.body)
    {
        imageHandler* img = new imageHandler();

        string path = imgPath.absolutePath + iter;
        imgHandler->loadTexture(path.c_str(), img);
        body.push_back(img);
    }
    cout << "---Body: " << body.size() << endl;

    for(auto & iter : imgPath.eyes)
    {
        imageHandler* img = new imageHandler();
        string path = imgPath.absolutePath + iter;
        imgHandler->loadTexture(path.c_str(), img);
        eyes.push_back(img);
    }
    cout << "---Eyes: " << eyes.size() << endl;

    for(auto & iter : imgPath.outfits)
    {
        for(auto & jter : iter.second)
        {
            imageHandler* img = new imageHandler();
            string path = imgPath.absolutePath + jter;
            imgHandler->loadTexture(path.c_str(), img);
            outfit.push_back(img);
        }
    }

    cout << "---Outfit: " << outfit.size() << endl;

    for(auto & iter : imgPath.hairstyle)
    {
        vector<imageHandler*> temp;
        for(auto & jter : iter.second)
        {
            imageHandler* img = new imageHandler();
            string path = imgPath.absolutePath + jter;
            imgHandler->loadTexture(path.c_str(), img);
            temp.push_back(img);
        }
        hair.push_back(temp);
    }

    cout << "---Hair: " << hair.size() << endl;
    for(int i = 0; i < hair.size(); i++)
    {
        cout << "------" << i << ": " << hair.at(i).size() << endl;
    }


    for(auto & iter : imgPath.accessories)
    {
        for(auto & jter : iter.second)
        {
            imageHandler* img = new imageHandler();
            string path = imgPath.absolutePath + jter;
            imgHandler->loadTexture(path.c_str(), img);
            accessories.push_back(img);
        }
    }

    indexAccessories = accessories.size();
    cout << "---Accessories: " << accessories.size() << endl;
}

void characterBuilder::changeBody(int i)
{
    if(i == 0)
    {
        indexBody --;
        if(indexBody < 0)
        {
            indexBody = body.size() - 1;
        }
    }
    else if(i == 1)
    {
        indexBody++;
        if(indexBody == body.size())
        {
            indexBody = 0;
        }
    }
}
void characterBuilder::changeEyes(int i)
{
    if( i == 0)
    {
        indexEyes --;
        if(indexEyes < 0)
        {
            indexEyes = eyes.size() - 1;
        }
    }
    else if (i == 1)
    {
        indexEyes ++;
        if(indexEyes == eyes.size())
        {
            indexEyes = 0;
        }
    }
}
void characterBuilder::changeOutfit(int i)
{
    if(i == 0)
    {
        indexOutfit --;
        if(indexOutfit < 0)
        {
            indexOutfit = outfit.size() - 1;
        }
    }
    else if(i == 1)
    {
        indexOutfit ++;
        if(indexOutfit == outfit.size())
        {
            indexOutfit = 0;
        }
    }
}
void characterBuilder::changeHair(int i)
{
    if(i == 0)
    {
        indexHair --;
        if(indexHair < 0)
        {
            indexHair = hair.size();
        }
    }
    else if(i == 1)
    {
        indexHair ++;
        if(indexHair > hair.size())
        {
            indexHair = 0;
        }
    }
}
void characterBuilder::changeAccessories(int i)
{
    if(i == 0)
    {
        indexAccessories --;
        if(indexAccessories < 0)
        {
            indexAccessories = accessories.size();
        }
    }
    else if(i ==1)
    {
        indexAccessories ++;
        if(indexAccessories > accessories.size())
        {
            indexAccessories = 0;
        }
    }
}

void characterBuilder::changeHairColor(int i)
{
    if(i < hair.at(indexHair).size())
    {
        indexHairColor = i;
    }
}

void characterBuilder::drawCharacterBuilder(imageHandler* imgHandler, float frameTimer)
{
    cout <<  "Body: " <<indexBody <<  " Eyes: " << indexEyes <<  " Outfit: " << indexOutfit <<  " Hair: " << indexHair <<  " Accessories: " << indexAccessories << endl;
    float factor = 4.f;

    ImGui::SetCursorPos(drawPos);
    imgHandler->DrawAniamtionFrame(*body.at(indexBody), cordsAnim.at(frameCount_4), factor); // body

    ImGui::SetCursorPos(drawPos);
    imgHandler->DrawAniamtionFrame(*eyes.at(indexEyes), cordsAnim.at(frameCount_4), factor); // eyes

    ImGui::SetCursorPos(drawPos);
    imgHandler->DrawAniamtionFrame(*outfit.at(indexOutfit), cordsAnim.at(frameCount_4), factor); // outfit

    if(indexHair != hair.size())
    {
        ImGui::SetCursorPos(drawPos);
        imgHandler->DrawAniamtionFrame(*hair.at(indexHair).at(indexHairColor), cordsAnim.at(frameCount_4), factor); // hair
    }

    if(indexAccessories != accessories.size())
    {
        ImGui::SetCursorPos(drawPos);
        imgHandler->DrawAniamtionFrame(*accessories.at(indexAccessories), cordsAnim.at(frameCount_4), factor); // accessories
    }

    if (frameTimer <= 0.f)
    {
        frameCount_4 ++;
        if (frameCount_4 % 4 == 0) frameCount_4=0;
    }
}
