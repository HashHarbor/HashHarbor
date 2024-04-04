#include "characterBuilder.h"
#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "userProfile/userProfile.h"

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

#if defined(__APPLE__)
        string path = imgPath.currentPath.string() +"/"+ iter;
#else
        string path = imgPath.absolutePath + iter;
#endif
        bool ret = imgHandler->loadTexture(path.c_str(), img);
        IM_ASSERT(ret);
        body.push_back(img);
    }
    //cout << "---Body: " << body.size() << endl;

    for(auto & iter : imgPath.eyes)
    {
        imageHandler* img = new imageHandler();
#if defined(__APPLE__)
        string path = imgPath.currentPath.string() +"/"+ iter;
#else
        string path = imgPath.absolutePath + iter;
#endif
        bool ret = imgHandler->loadTexture(path.c_str(), img);
        IM_ASSERT(ret);
        eyes.push_back(img);
    }
    //cout << "---Eyes: " << eyes.size() << endl;

    for(auto & iter : imgPath.outfits)
    {
        vector<imageHandler*> temp;
        for(auto & jter : iter.second)
        {
            imageHandler* img = new imageHandler();
#if defined(__APPLE__)
            string path = imgPath.currentPath.string() +"/"+ jter;
#else
            string path = imgPath.absolutePath + jter;
#endif
            bool ret = imgHandler->loadTexture(path.c_str(), img);
            IM_ASSERT(ret);
            temp.push_back(img);
        }
        outfit.push_back(temp);
    }
/*
    cout << "---Outfit: " << outfit.size() << endl;
    for(int i = 0; i < outfit.size(); i++)
    {
        cout << "------" << i << ": " << outfit.at(i).size() << endl;
    }
*/
    for(auto & iter : imgPath.hairstyle)
    {
        vector<imageHandler*> temp;
        for(auto & jter : iter.second)
        {
            imageHandler* img = new imageHandler();
#if defined(__APPLE__)
            string path = imgPath.currentPath.string() +"/"+ jter;
#else
            string path = imgPath.absolutePath + jter;
#endif
            bool ret = imgHandler->loadTexture(path.c_str(), img);
            IM_ASSERT(ret);
            temp.push_back(img);
        }
        hair.push_back(temp);
    }
/*
    cout << "---Hair: " << hair.size() << endl;
    for(int i = 0; i < hair.size(); i++)
    {
        cout << "------" << i << ": " << hair.at(i).size() << endl;
    }
*/

    for(auto & iter : imgPath.accessories)
    {
        vector<imageHandler*> temp;
        for(auto & jter : iter.second)
        {
            imageHandler* img = new imageHandler();
#if defined(__APPLE__)
            string path = imgPath.currentPath.string() +"/"+ jter;
#else
            string path = imgPath.absolutePath + jter;
#endif
            bool ret = imgHandler->loadTexture(path.c_str(), img);
            IM_ASSERT(ret);
            temp.push_back(img);
        }
        accessories.push_back(temp);
    }

    //indexAccessories = accessories.size();
/*
    cout << "---Accessories: " << accessories.size() << endl;
    for(int i = 0; i < accessories.size(); i++)
    {
        cout << "------" << i << ": " << accessories.at(i).size() << endl;
    }
*/
}

void characterBuilder::changeBody(int i)
{
    if(i < (int)body.size())
    {
        indexBody = i;
    }
}
void characterBuilder::changeEyes(int i)
{
    if(i < (int)eyes.size())
    {
        indexEyes = i;
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
        if(indexOutfit == (int)outfit.size())
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
        if(indexHair > (int)hair.size())
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
    else if(i == 1)
    {
        indexAccessories ++;
        if(indexAccessories > (int)accessories.size())
        {
            indexAccessories = 0;
        }
    }
}

void characterBuilder::changeHairColor(int i)
{
    if(i < (int)hair.at(indexHair).size())
    {
        indexHairColor = i;
    }
}
void characterBuilder::changeOutfitColor(int i)
{
    if(i < (int)outfit.at(indexOutfit).size())
    {
        indexOutfitColor = i;
    }
}
void characterBuilder::changeAccessoriesColor(int i)
{
    if(i < (int)accessories.at(indexAccessories).size())
    {
        indexAccessoriesColor = i;
    }
}

void characterBuilder::drawCharacter(imageHandler *imgHandler, float frameTimer)
{
    float factor = 4.f;

    ImGui::SetCursorPos(ImVec2(drawPos.x + 20.f,drawPos.y - 20.f));
    static bool animate = true;
    ImGui::Checkbox("Animate", &animate);

    if(!animate)
    {
        frameCount_4 = 0 ;
    }

    ImGui::SetCursorPos(drawPos);
    imgHandler->DrawAnimationFrame(*body.at(indexBody), cordsAnim.at(frameCount_4), factor); // body

    ImGui::SetCursorPos(drawPos);
    imgHandler->DrawAnimationFrame(*eyes.at(indexEyes), cordsAnim.at(frameCount_4), factor); // eyes

    ImGui::SetCursorPos(drawPos);
    imgHandler->DrawAnimationFrame(*outfit.at(indexOutfit).at(indexOutfitColor), cordsAnim.at(frameCount_4), factor); // outfit

    if(indexHair != (int)hair.size())
    {
        ImGui::SetCursorPos(drawPos);
        imgHandler->DrawAnimationFrame(*hair.at(indexHair).at(indexHairColor), cordsAnim.at(frameCount_4), factor); // hair
    }

    if(indexAccessories != (int)accessories.size())
    {
        ImGui::SetCursorPos(drawPos);
        imgHandler->DrawAnimationFrame(*accessories.at(indexAccessories).at(indexAccessoriesColor), cordsAnim.at(frameCount_4), factor); // accessories
    }

    if (frameTimer <= 0.f)
    {
        frameCount_4 ++;
        if (frameCount_4 % 4 == 0) frameCount_4=0;
    }
}
void characterBuilder::drawHairControls()
{
    ImGui::SetCursorPos(ImVec2(50.f,342.f));
    ImGui::Text("Hair       ");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##Hleft", ImGuiDir_Left)) { changeHair(0); }
    ImGui::SameLine(0.0f, 15.f);
    if (ImGui::ArrowButton("##Hright", ImGuiDir_Right)) { changeHair(1); }
    ImGui::PopButtonRepeat();

    if(indexHair != (int)hair.size())
    {
        // hairstyle_x_1
        ImGui::SetCursorPos(ImVec2(30.f,372.f));
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(31.f / 360.f, 0.648f, 0.702f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(32.f / 360.f, 0.564f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(25.f / 360.f, 0.684f, 0.671f));
        if(ImGui::Button("  "))
        {
            changeHairColor(0);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        // hairstyle_x_2
        ImGui::SetCursorPos(ImVec2(60.f,372.f));
        ImGui::PushID(2);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(30.f / 360.f, 0.463f, 0.584f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(31.f / 360.f, 0.495f, 0.729f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(20.f / 360.f, 0.406f, 0.541f));
        if(ImGui::Button("  "))
        {
            changeHairColor(1);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        // hairstyle_x_3
        ImGui::SetCursorPos(ImVec2(90.f,372.f));
        ImGui::PushID(3);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(12.f / 360.f, 0.43f, 0.502f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(19.f / 360.f, 0.403f, 0.525f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(12.f / 360.f, 0.439f, 0.447f));
        if(ImGui::Button("  "))
        {
            changeHairColor(2);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        // hairstyle_x_4
        ImGui::SetCursorPos(ImVec2(120.f,372.f));
        ImGui::PushID(4);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(8.f / 360.f, 0.291f, 0.404f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(17.f / 360.f, 0.342f, 0.435f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(8.f / 360.f, 0.264f, 0.357f));
        if(ImGui::Button("  "))
        {
            changeHairColor(3);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        // hairstyle_x_5
        ImGui::SetCursorPos(ImVec2(150.f,372.f));
        ImGui::PushID(5);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(60.f / 360.f, 0.007f, 0.588f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(60.f / 360.f, 0.012f, 0.635f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(30.f / 360.f, 0.015f, 0.537f));
        if(ImGui::Button("  "))
        {
            changeHairColor(4);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        // hairstyle_x_6
        ImGui::SetCursorPos(ImVec2(180.f,372.f));
        ImGui::PushID(6);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(30.f / 360.f, 0.083f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(40.f / 360.f, 0.175f, 0.537f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(14.f / 360.f, 0.116f, 0.439f));
        if(ImGui::Button("  "))
        {
            changeHairColor(5);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        // hairstyle_x_7
        ImGui::SetCursorPos(ImVec2(210.f,372.f));
        ImGui::PushID(7);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("  "))
        {
            changeHairColor(6);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
}
void characterBuilder::drawBodyEyeControl()
{
    ImGui::SetCursorPos(ImVec2(50.f,138.f));
    ImGui::Text("Body       ");

    for(int i = 0; i < (int)body.size(); i++)
    {
        float x = 30.f + (30.f * (float)i);
        ImGui::SetCursorPos(ImVec2(x,168.f));
        ImGui::PushID((i + 1) * 100);
        int colorIndex = i * 3;
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)bodyColors.at(colorIndex));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)bodyColors.at(colorIndex + 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)bodyColors.at(colorIndex + 2));
        if(ImGui::Button("  "))
        {
            changeBody(i);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }



    ImGui::SetCursorPos(ImVec2(50.f,206.f));
    ImGui::Text("Eyes       ");

    for(int i = 0; i < (int)eyes.size(); i++)
    {
        float x = 30.f + (30.f * (float)i);
        ImGui::SetCursorPos(ImVec2(x,236.f));
        ImGui::PushID((i + 1) * 110);
        int colorIndex = i * 3;
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)eyeColors.at(colorIndex));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)eyeColors.at(colorIndex + 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)eyeColors.at(colorIndex + 2));
        if(ImGui::Button("  "))
        {
            changeEyes(i);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
}
void characterBuilder::drawOutfitControls()
{
    ImGui::SetCursorPos(ImVec2(50.f,274.f));
    ImGui::Text("Outfit     ");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##Oleft", ImGuiDir_Left))
    {
        changeOutfit(0);
        indexOutfitColor = 0;
    }
    ImGui::SameLine(0.0f, 15.f);
    if (ImGui::ArrowButton("##Oright", ImGuiDir_Right))
    {
        changeOutfit(1);
        indexOutfitColor = 0;
    }
    ImGui::PopButtonRepeat();

    for(int i = 0; i < (int)outfit.at(indexOutfit).size(); i++)
    {
        float x = 30.f + (30.f * (float)i);
        ImGui::SetCursorPos(ImVec2(x,304.f));
        ImGui::PushID((i + 1) * 11);
        int colorIndex = i * 3;
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)outfitButtonColor.at(indexOutfit).at(colorIndex));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)outfitButtonColor.at(indexOutfit).at(colorIndex + 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)outfitButtonColor.at(indexOutfit).at(colorIndex + 2));
        if(ImGui::Button("  "))
        {
            changeOutfitColor(i);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
}
void characterBuilder::drawAccessoriesControl()
{
    ImGui::SetCursorPos(ImVec2(50.f,70.f));
    ImGui::Text("Accessories");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##accessorieLEFT", ImGuiDir_Left))
    {
        changeAccessories(0);
        indexAccessoriesColor = 0;
    }
    ImGui::SameLine(0.0f, 15.f);
    if (ImGui::ArrowButton("##accessorieRIGHT", ImGuiDir_Right))
    {
        changeAccessories(1);
        indexAccessoriesColor = 0;
    }
    ImGui::PopButtonRepeat();

    if(indexAccessories != (int)accessories.size())
    {
        for(int i = 0; i < (int)accessories.at(indexAccessories).size(); i++)
        {
            float x = 30.f + (30.f * (float)i);
            ImGui::SetCursorPos(ImVec2(x,100.f));
            ImGui::PushID((i + 1) * 13);
            int colorIndex = i * 3;
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)accessoriesButtonColor.at(indexAccessories).at(colorIndex));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)accessoriesButtonColor.at(indexAccessories).at(colorIndex + 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)accessoriesButtonColor.at(indexAccessories).at(colorIndex + 2));
            if(ImGui::Button("  "))
            {
                changeAccessoriesColor(i);
            }
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
    }
}

void characterBuilder::drawCharacterBuilder(imageHandler* imgHandler, float frameTimer)
{
    //cout << indexBody << " || " << indexEyes << " || " << indexOutfit << " | " << indexOutfitColor << " || " << indexHair << " | " << indexHairColor << " || " << indexAccessories << " | " << indexAccessoriesColor << " ||||" << endl;
    drawCharacter(imgHandler,frameTimer);
    drawAccessoriesControl();
    drawBodyEyeControl();
    drawOutfitControls();
    drawHairControls();
}

void characterBuilder::setAsMainCharacter(vector<int>& index)
{
    userProfile& usrProfile = userProfile::getInstance();

    index[0] = indexBody;
    index[1] = indexEyes;
    index[2] = indexOutfit;
    index[3] = indexOutfitColor;
    index[4] = indexHair;
    index[5] = indexHairColor;
    index[6] = indexAccessories;
    index[7] = indexAccessoriesColor;

    usrProfile.setCharacter(0, indexBody);
    usrProfile.setCharacter(1, indexEyes);
    usrProfile.setCharacter(2, indexOutfit);
    usrProfile.setCharacter(3, indexOutfitColor);
    usrProfile.setCharacter(4, indexHair);
    usrProfile.setCharacter(5, indexHairColor);
    usrProfile.setCharacter(6, indexAccessories);
    usrProfile.setCharacter(7, indexAccessoriesColor);
}
void characterBuilder::cleanUp()
{
    for(auto iter : body)
    {
        iter->cleanUp();
    }
    body.clear();
    for(auto iter : eyes)
    {
        iter->cleanUp();
    }
    eyes.clear();
    for(auto iter : outfit)
    {
        for(auto jter : iter)
        {
            jter->cleanUp();
        }
        iter.clear();
    }
    outfit.clear();
    for(auto iter : hair)
    {
        for(auto jter : iter)
        {
            jter->cleanUp();
        }
        iter.clear();
    }
    hair.clear();
    for(auto iter : accessories)
    {
        for(auto jter : iter)
        {
            jter->cleanUp();
        }
        iter.clear();
    }
    accessories.clear();

   // cout << body.size() << ", " << eyes.size() << ", " << outfit.size() << ", " << hair.size() << ", " << accessories.size() << endl;
}

void characterBuilder::drawCharacterAnimation(imageHandler *imgHandler, ImVec2 pos, pair<ImVec2, ImVec2> cords, float scale,vector<int>& characterIndex)
{
    ImGui::SetCursorPos(pos);
    imgHandler->DrawAnimationFrame(*body[characterIndex[0]], cords, scale);

    ImGui::SetCursorPos(pos);
    imgHandler->DrawAnimationFrame(*eyes[characterIndex[1]], cords, scale);

    ImGui::SetCursorPos(pos);
    imgHandler->DrawAnimationFrame(*outfit[characterIndex[2]][characterIndex[3]], cords, scale);

    if(characterIndex[4] != (int)hair.size()) // allow for hair index to equal size to allow for bald character
    {
        ImGui::SetCursorPos(pos);
        imgHandler->DrawAnimationFrame(*hair[characterIndex[4]][characterIndex[5]], cords, scale);
    }
    if(characterIndex[6] != (int)accessories.size()) // allow for accessories to equal size to allow for no accessories
    {
        ImGui::SetCursorPos(pos);
        imgHandler->DrawAnimationFrame(*accessories[characterIndex[6]][characterIndex[7]], cords, scale);
    }
}

void characterBuilder::setCharacterFromDb()
{
    userProfile& usrProfile = userProfile::getInstance();

    indexBody = usrProfile.getCharacter()[0];
    indexEyes = usrProfile.getCharacter()[1];
    indexOutfit = usrProfile.getCharacter()[2];
    indexOutfitColor = usrProfile.getCharacter()[3];
    indexHair = usrProfile.getCharacter()[4];
    indexHairColor = usrProfile.getCharacter()[5];
    indexAccessories = usrProfile.getCharacter()[6];
    indexAccessoriesColor = usrProfile.getCharacter()[7];
}

void characterBuilder::reset()
{
    indexBody = 0;
    indexEyes = 5;

    indexOutfit = 8;
    indexOutfitColor = 0;

    indexHair = 2;
    indexHairColor = 2;

    indexAccessories = 0;
    indexAccessoriesColor = 3;

    frameCount_4 = 0;
}