#include "pause.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "database/database.h"

#if defined(__APPLE__)
#include <iostream>
#include <vector>
#include <filesystem>
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
using std::pair;
#include <utility>
#endif

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "textEditor/TextEditor.h"
#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "../character/characterManager.h"
#include "../character/characterBuilder.h"
#include "../movement/movementHandler.h"
#include "../assets/font/IconsFontAwesome6.h"
#include "../login/login.h"
#include "userProfile/userProfile.h"
#include "database/database.h"
#include "authentication/authentication.h"

using std::string;
using std::vector;
using std::regex;

pause::pause(int width, int height)
{
    width_px = width;
    height_px = height;

    windowWidth = 320.f;
    windowHeight = 620.f; // allow for 50px padding on a 1280x720 window
    paddingHeight = ((float)height_px - windowHeight) / 2.f;
    paddingWidth = ((float)width_px - (320.f + 860.f)) / 2.f;
}

void pause::drawPauseMenu(imageHandler *image, characterManager *character, characterBuilder *charBuild, bool *resetPauseScreen,bool *updateCharacter, bool *reset, bool *done)
{
    userProfile& usrProfile = userProfile::getInstance();

    mainControls();

    if(settingsWindow)
    {
        drawSettingsWindow();
    }
    else if(userProfileWindow)
    {
        const float profileWidth = windowWidth + paddingWidth + 10.f;
        const float profileHeight = paddingHeight;
        ImGui::SetNextWindowSize({850.f, 620.f});
        ImGui::SetNextWindowPos({profileWidth, profileHeight});

        ImGui::Begin("User Profile", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        {
            draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(ImVec2(profileWidth + 20.f, profileHeight + 70.f), ImVec2(profileWidth + 300.f, profileHeight + 230.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);
            draw_list->AddRectFilled(ImVec2(profileWidth + 35.f, profileHeight + 85.f), ImVec2(profileWidth + 109.f, profileHeight + 218.f), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 1.0f)), 20.0f);

            charBuild->drawCharacterAnimation(image, ImVec2(40.f, 85.f), {ImVec2(0.1f / 192.f, 0.1f/320.f),ImVec2(31.99f/192.f, 64.f/320.f)}, 2.f, character->getMainPlayer()->dynamicIndex);
            // show username
            //ICON_FA_ID_CARD
            ImGui::SetCursorPos(ImVec2(270.f, 85.f));
            ImGui::Text(ICON_FA_USER);

            ImGui::SetCursorPos(ImVec2(124.f, 110.f));
            ImGui::Text("Username: ");
            ImGui::SameLine();
            ImGui::Text("%s", usrProfile.getUsername().c_str());

            ImGui::SetCursorPos(ImVec2(124.f, 130.f));
            ImGui::Text("Date Joined: ");
            ImGui::SameLine();
            ImGui::Text("%s", usrProfile.getJoinDate().c_str()); // todo - get from database

            ImGui::SetCursorPos(ImVec2(124.f, 150.f));
            ImGui::Text("Level: "); // todo - change to whatever gets implemented
            ImGui::SameLine();
            ImGui::Text("0");

            ImGui::SetCursorPos(ImVec2(124.f, 170.f));
            ImGui::Text("Problems Solved: "); // todo - get from database
            ImGui::SameLine();
            ImGui::Text("0");

            // change username

            ImGui::SetCursorPos(ImVec2(360.f, 40.f));
            ImGui::PushID(11);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
            if(ImGui::Button("Change Username", ImVec2(130.f, 20.f)))
            {
                usr_Username = !usr_Username;
                usr_Password = false;

                createUsername[0] = '\0';
            }
            ImGui::PopStyleColor(3);
            ImGui::PopID();
            // change password
            ImGui::SameLine();
            ImGui::PushID(22);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
            if(ImGui::Button("Change Password", ImVec2(130.f, 20.f)))
            {
                usr_Username = false;
                usr_Password = !usr_Password;

                currentPasswd[0] = '\0';
                createPasswd[0] = '\0';
                confirmPasswd[0] = '\0';
            }
            ImGui::PopStyleColor(3);
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID(33);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
            if(ImGui::Button("Change Character", ImVec2(130.f, 20.f)))
            {
                usr_Username = false;
                usr_Password = false;
                userProfileWindow = false;
                characterWindow = true;
            }
            ImGui::PopStyleColor(3);
            ImGui::PopID();

            // todo - change to have its own window for each instead of current layout
            if(usr_Username)
            {
                updateUserUsername(profileWidth, profileHeight);
            }
            else if(usr_Password)
            {
                updateUserPassword(profileWidth, profileHeight);
            }

            // see progress
            // have a child window to list and selectg the problems to see how the user did
            draw_list->AddRectFilled(ImVec2(profileWidth + 20.f, profileHeight + 260.f), ImVec2(profileWidth + 405.f, profileHeight + 600.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);
            ImGui::SetCursorPos(ImVec2(30.f, 270.f));
            ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
            ImGui::Text("Solved Problems");
            ImGui::PopStyleColor();

            draw_list->AddRectFilled(ImVec2(profileWidth + 425.f, profileHeight + 260.f), ImVec2(profileWidth + 830.f, profileHeight + 600.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);
            ImGui::SetCursorPos(ImVec2(435.f, 270.f));
            ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
            ImGui::Text("Inventory");
            ImGui::PopStyleColor();
            // possibly see inventory
            // use a child window to view the items
        }
        ImGui::End();
    }
    else if(characterWindow)
    {
        drawCharacterCreatorWindow(image, charBuild, updateCharacter);
    }
    else if (logOutWindow)
    {
        drawLogOutWindow(reset);
    }
    else if(quitWindow)
    {
        drawQuitWindow(done);
    }
}

void pause::reset()
{
    settingsWindow = false;
    userProfileWindow = false;
    characterWindow = false;
    logOutWindow = false;
    quitWindow = false;

    usr_Username = false;
    usr_Password = false;

    createUsername[0] = '\0';
    currentPasswd[0] = '\0';
    createPasswd[0] = '\0';
    confirmPasswd[0] = '\0';
}

void pause::mainControls()
{
    ImGui::SetNextWindowSize({windowWidth, windowHeight});
    ImGui::SetNextWindowPos({paddingWidth, paddingHeight});

    ImGui::Begin("Pause", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        draw_list = ImGui::GetWindowDrawList();

        ImGui::SetCursorPos(ImVec2(85.f, (620.f / 6.f) * 1.f - 20.f));
        ImGui::PushID(1111);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("Settings", ImVec2(150.f, 40.f)))
        {
            settingsWindow = true;
            userProfileWindow = false;
            characterWindow = false;
            logOutWindow = false;
            quitWindow = false;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(85.f, (620.f / 6.f) * 2.f - 20.f));
        ImGui::PushID(2222);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("User Profile", ImVec2(150.f, 40.f)))
        {
            settingsWindow = false;
            userProfileWindow = true;
            characterWindow = false;
            logOutWindow = false;
            quitWindow = false;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(85.f, (620.f / 6.f) * 3.f - 20.f));
        ImGui::PushID(3333);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("Change Character", ImVec2(150.f, 40.f)))
        {
            settingsWindow = false;
            userProfileWindow = false;
            characterWindow = true;
            logOutWindow = false;
            quitWindow = false;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(85.f, (620.f / 6.f) * 4.f - 20.f));
        ImGui::PushID(4444);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("Log Out", ImVec2(150.f, 40.f)))
        {
            settingsWindow = false;
            userProfileWindow = false;
            characterWindow = false;
            logOutWindow = true;
            quitWindow = false;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(85.f, (620.f / 6.f) * 5.f - 20.f));
        ImGui::PushID(5555);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f, 1.0f, 0.76f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 360.f, 1.f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 360.f, 1.f, 0.384f));
        if(ImGui::Button("Quit", ImVec2(150.f, 40.f)))
        {
            settingsWindow = false;
            userProfileWindow = false;
            characterWindow = false;
            logOutWindow = false;
            quitWindow = true;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(90.f, ((620.f / 6.f) * 1.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_GEAR);

        ImGui::SetCursorPos(ImVec2(90.f, ((620.f / 6.f) * 2.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_USER);

        ImGui::SetCursorPos(ImVec2(86.f, ((620.f / 6.f) * 3.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_PERSON);

        ImGui::SetCursorPos(ImVec2(90.f, ((620.f / 6.f) * 4.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_CIRCLE_ARROW_LEFT);

        ImGui::SetCursorPos(ImVec2(90.f, ((620.f / 6.f) * 5.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_POWER_OFF);
    }
    ImGui::End();
}
void pause::drawSettingsWindow()
{
    // todo - change to real audio controls from audio class
    static int tempMain = 20;
    static int tempMusic = 20;
    static int tempSoundEffect = 20;

    ImGui::SetNextWindowSize({850.f, 620.f});
    ImGui::SetNextWindowPos({windowWidth + paddingWidth + 10.f, paddingHeight});

    ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::SetCursorPos(ImVec2(20.f, 40.f));
        ImGui::Text("Change Screen Resolution");

        ImGui::SetCursorPos(ImVec2(30.f, 60.f));
        static int e = 0;
        static int k = 0;
#if defined(__APPLE__)
        ImGui::RadioButton("2480 x 1440", &e, 0);
        ImGui::SameLine();
        ImGui::RadioButton("2560 x 1800", &e, 1);
#else
        ImGui::RadioButton("1280 x 720", &e, 0);
            ImGui::SameLine();
            ImGui::RadioButton("1440 x 900", &e, 1);
            ImGui::SameLine();
            ImGui::RadioButton("1920 x 1080", &e, 2);
#endif
        // Disabled due to issues with SDL and MAC Retina Displays
/*
            if(k != e)
            {
                switch(e)
                {
                    case 0:
                        width_px = 1280;
                        height_px = 720;
                        Login.updateResolution(1280, 720);
                        break;
                    case 1:
                        width_px = 1440;
                        height_px = 800;
                        Login.updateResolution(1440, 800);
                        break;
                    case 2:
                        width_px = 1920;
                        height_px = 1080;
                        Login.updateResolution(1920, 1080);
                        break;
                }
                changeResolution = true;
                k = e;
            }
*/
        ImGui::SetCursorPos(ImVec2(20.f, 100.f));
        ImGui::Text("Volume Controls: ");

        ImGui::SetCursorPos(ImVec2(30.f, 120.f));
        ImGui::SliderInt("Main", &tempMain, 0, 20);
        ImGui::SetCursorPos(ImVec2(30.f, 145.f));
        ImGui::SliderInt("Music", &tempMusic, 0, 20);
        ImGui::SetCursorPos(ImVec2(30.f, 170.f));
        ImGui::SliderInt("Sound Effects", &tempSoundEffect, 0, 20);
        // Change resolution
        // give warning on mac that resolution exceeds display size on anything over 1440x900 or just double the number displayed
    }
    ImGui::End();
}

void pause::updateUserUsername(float profileWidth, float profileHeight)
{
    //todo - get rid of profile
    userProfile& usrProfile = userProfile::getInstance();
    draw_list->AddRectFilled(ImVec2(profileWidth + 350.f, profileHeight + 70.f), ImVec2(profileWidth + 800.f, profileHeight + 200.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);

    ImGui::SetCursorPos(ImVec2(380.f, 80.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Enter New Username");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(380.f, 105.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText(" ",createUsername, IM_ARRAYSIZE(createUsername), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(380.f, 140.f));
    ImGui::PushID(44);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
    if(ImGui::Button("Update Username", ImVec2(130.f, 30.f)))
    {
        authentication auth = authentication();
        if(auth.changeUsername(createUsername, usrProfile.getId()))
        {
            // todo - display success
            cout << "Username Change Success" << endl;
            usr_Username = false;
        }
        else
        {
            cout << "Username Error\n";
            // todo - display error message
        }
        createUsername[0] = '\0';
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();
}
void pause::updateUserPassword(float profileWidth, float profileHeight)
{
    draw_list->AddRectFilled(ImVec2(profileWidth + 350.f, profileHeight + 70.f), ImVec2(profileWidth + 800.f, profileHeight + 255.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);

    ImGui::SetCursorPos(ImVec2(380.f, 75.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Enter Current Password");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(380.f, 95.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText(" ",currentPasswd, IM_ARRAYSIZE(currentPasswd), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(380.f, 120.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Enter New Password");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(380.f, 140.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText("  ",createPasswd, IM_ARRAYSIZE(createPasswd), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(380.f, 170.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Confirm New Password");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(380.f, 190.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText("   ",confirmPasswd, IM_ARRAYSIZE(confirmPasswd), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(380.f, 220.f));
    ImGui::PushID(44);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
    if(ImGui::Button("Update Password", ImVec2(130.f, 30.f)))
    {
        if(strcmp(createPasswd,confirmPasswd) != 0) // if new password and confirmation dont match
        {
            // todo - display error message
            cout << "New Passwords Dont Match" << endl;
        }
        else
        {
            authentication auth = authentication();
            if(auth.changePassword(currentPasswd, createPasswd))
            {
                // todo - display success
                cout << "Success" << endl;
                usr_Password = false;
            }
        }
        currentPasswd[0] = '\0';
        createPasswd[0] = '\0';
        confirmPasswd[0] = '\0';
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void pause::drawCharacterCreatorWindow(imageHandler *image, characterBuilder *charBuild, bool *updateCharacter)
{
    float factor = 4.f;
    const float frameLength = 5.f / 10.f; // In seconds, so  FPS
    static float frameTimer = frameLength;

    ImGui::SetNextWindowSize({850.f, 620.f});
    ImGui::SetNextWindowPos({windowWidth + paddingWidth + 10.f, paddingHeight});

    ImGui::Begin("Change Character", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        frameTimer -= ImGui::GetIO().DeltaTime;
        ImVec2 characterPos = ImVec2((ImGui::GetContentRegionAvail() - ImVec2((32.f * factor), (64.f * factor))) * 0.25f);
        charBuild->drawPos = ImVec2(characterPos.x * 3.f, characterPos.y * 2.f);
        charBuild->drawCharacterBuilder(image, frameTimer);

        if (frameTimer <= 0.f)
        {
            frameTimer = 5.f / 10.f;
        }

        ImGui::SetCursorPos(ImVec2(350.f, 550.f));
        ImGui::PushID(8);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("Select Character", ImVec2(150.f, 40.f)))
        {
            *updateCharacter = true;
            //todo that character changed instead of closing
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
    ImGui::End();
}

void pause::drawLogOutWindow(bool* reset)
{
    ImGui::SetNextWindowSize({320.f, 110.f});
    ImGui::SetNextWindowPos({windowWidth + paddingWidth + 10.f, paddingHeight});
    ImGui::Begin("Log Out", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::SetCursorPos(ImVec2(20.f, 20.f));
        ImGui::Text("Are You Sure You Want to Log Out?");

        ImGui::SetCursorPos(ImVec2( 20.f,50.f));
        ImGui::PushID(77);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("Keep Playing", ImVec2(120.f, 40.f)))
        {
            settingsWindow = false;
            userProfileWindow = false;
            characterWindow = false;
            logOutWindow = false;
            quitWindow = false;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2( 160.f,50.f));
        ImGui::PushID(66);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f, 1.0f, 0.76f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 360.f, 1.f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 360.f, 1.f, 0.384f));
        if(ImGui::Button("Log Out", ImVec2(120.f, 40.f)))
        {
            *reset = true;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
    ImGui::End();
}
void pause::drawQuitWindow(bool* done)
{
    ImGui::SetNextWindowSize({320.f, 110.f});
    ImGui::SetNextWindowPos({windowWidth + paddingWidth + 10.f, paddingHeight});
    ImGui::Begin("Quit", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::SetCursorPos(ImVec2(20.f, 20.f));
        ImGui::Text("Are You Sure You Want to Quit?");

        ImGui::SetCursorPos(ImVec2( 20.f,50.f));
        ImGui::PushID(77);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("Keep Playing", ImVec2(120.f, 40.f)))
        {
            settingsWindow = false;
            userProfileWindow = false;
            characterWindow = false;
            logOutWindow = false;
            quitWindow = false;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2( 160.f,50.f));
        ImGui::PushID(66);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f, 1.0f, 0.76f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 360.f, 1.f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 360.f, 1.f, 0.384f));
        if(ImGui::Button("Quit", ImVec2(120.f, 40.f)))
        {
            *done = true;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
    ImGui::End();
}