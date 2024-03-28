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

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled(ICON_FA_CIRCLE_QUESTION);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 50.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::PopStyleColor();
        ImGui::EndTooltip();
    }
}

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
        drawSettingsWindow(image, charBuild, character);
    }
    else if(notebookWindow)
    {
        drawNotebookWindow();
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
    notebookWindow = false;
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
            notebookWindow = false;
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
        if(ImGui::Button("Notebook", ImVec2(150.f, 40.f)))
        {
            settingsWindow = false;
            notebookWindow = true;
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
            notebookWindow = false;
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
            notebookWindow = false;
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
            notebookWindow = false;
            characterWindow = false;
            logOutWindow = false;
            quitWindow = true;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(90.f, ((620.f / 6.f) * 1.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_GEAR);

        ImGui::SetCursorPos(ImVec2(90.f, ((620.f / 6.f) * 2.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_BOOK);

        ImGui::SetCursorPos(ImVec2(86.f, ((620.f / 6.f) * 3.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_PERSON);

        ImGui::SetCursorPos(ImVec2(90.f, ((620.f / 6.f) * 4.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_CIRCLE_ARROW_LEFT);

        ImGui::SetCursorPos(ImVec2(90.f, ((620.f / 6.f) * 5.f - 20.f) + 15.f));
        ImGui::Text(ICON_FA_POWER_OFF);
    }
    ImGui::End();
}
void pause::drawSettingsWindow(imageHandler *image, characterBuilder *charBuild, characterManager *character)
{
    // todo - change to real audio controls from audio class

    ImGui::SetNextWindowSize({850.f, 620.f});
    ImGui::SetNextWindowPos({windowWidth + paddingWidth + 10.f, paddingHeight});

    ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("Settings", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Main Game"))
            {
                settingsMain();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("User Account"))
            {
                settingsUser(image, charBuild, character);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void pause::drawNotebookWindow()
{
    const float profileWidth = windowWidth + paddingWidth + 10.f;
    const float profileHeight = paddingHeight;
    ImGui::SetNextWindowSize({850.f, 620.f});
    ImGui::SetNextWindowPos({profileWidth, profileHeight});

    ImGui::Begin("Notebook", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
    {
        // NOTEBOOK ISSUES
        // Notebook does not save to file
        // The multiline input does nto support text wrapping, another solution would be required to handle this

        // TODO - Load notebook from local storage

        draw_list = ImGui::GetWindowDrawList();

        static int page = 0;
        static int maxPage = 1;
        static char text[2048] = "";
        static bool edit = true;

        int textSize = strlen(text);

        if(edit)
        {
            ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("EDITING MODE").x / 2.f) , 20.f));
            ImGui::Text("EDITING MODE", textSize, (page + 1), ((int)notebook.size() + 1));

            ImGui::SetCursorPos(ImVec2(225.f, 40.f));
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
            ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(400, ImGui::GetTextLineHeight() * 40), flags);

            ImGui::SetCursorPos(ImVec2(55.f, 215.f));
            ImGui::PushID(1);
            if(ImGui::Button("READ", ImVec2(60.f, 40.f)))
            {
                edit = !edit;
            }
            ImGui::PopID();
        }
        else // used to wrap text on page due to limitation of ImGui
        {
            ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("READING MODE").x / 2.f) , 20.f));
            ImGui::Text("READING MODE", textSize, (page + 1), ((int)notebook.size() + 1));

            draw_list->AddRectFilled(ImVec2(profileWidth + 225.f, profileHeight + 40.f), ImVec2(profileWidth + 625.f, profileHeight +(ImGui::GetTextLineHeight() * 40) + 40.f), ImColor(ImVec4(0.1f, 0.1f, 0.1f, 1.0f)));

            ImGui::SetCursorPos(ImVec2(229.f, 43.f));
            static float wrap_width = 400.0f;
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
            ImGui::Text(text, wrap_width);
            ImGui::PopTextWrapPos();

            ImGui::SetCursorPos(ImVec2(55.f, 215.f)); // button to chnage modes
            ImGui::PushID(1);
            if(ImGui::Button("EDIT", ImVec2(60.f, 40.f)))
            {
                edit = !edit;
            }
            ImGui::PopID();
        }

        ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Characters: %d / 2048           Page: %d / %d").x / 2.f) , (ImGui::GetTextLineHeight() * 40) + 40.f));
        if((page + 1) >= maxPage) // update page limit on display
        {
            maxPage = page + 1;
        }
        ImGui::Text("Characters: %d / 2048           Page: %d / %d", textSize, (page + 1), maxPage);

        ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("In Editing mode the text will write as one big line, to view it  with edge wrapping use Reading mode").x / 2.f) , 600.f));
        ImGui::Text("In Editing mode the text will write as one big line, to view it  with edge wrapping use Reading mode");

        ImGui::SetCursorPos(ImVec2(55.f, 275.f));
        ImGui::PushID(2);
        if(ImGui::Button("SAVE", ImVec2(60.f, 40.f)))
        {
            if(page == notebook.size())
            {
                if(textSize > 0 && notebook.size() < 10)
                {
                    notebook.push_back(text);
                }
                else if(textSize > 0 && notebook.size() == 10)
                {
                    notebook.at(page) = text;
                }
            }
            else if (page < notebook.size())
            {
                notebook.at(page) = text;
            }
        }
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(55.f, 335.f));
        ImGui::PushID(3);
        if(ImGui::Button("EXPORT", ImVec2(60.f, 40.f)))
        {
            // TODO - Implement saving to file to ../../assets/other
        }
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(170.f, 40.f));
        ImGui::PushID(12345);
        if(ImGui::Button(ICON_FA_ARROW_LEFT, ImVec2(50.f, ImGui::GetTextLineHeight() * 40)))
        {
            if(page != 0)
            {
                if(page == notebook.size())
                {
                    if(textSize > 0 && notebook.size() < 10)
                    {
                        notebook.push_back(text);
                    }
                    else if(textSize > 0 && notebook.size() == 10)
                    {
                        notebook.at(page) = text;
                    }
                }
                else
                {
                    notebook.at(page) = text;
                }
                page --;
                text[0] = '\0';
                std::strcpy(text, notebook.at(page).c_str());
            }
            else
            {
                page = 0;
            }
        }
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(630.f, 40.f));
        ImGui::PushID(123456);
        if(ImGui::Button(ICON_FA_ARROW_RIGHT, ImVec2(50.f, ImGui::GetTextLineHeight() * 40)))
        {
            if(page == notebook.size())
            {
                if(textSize > 0)
                {
                    if(notebook.size() <= 8)
                    {
                        notebook.push_back(text);
                        page ++;
                        text[0] = '\0';
                    }
                }
            }
            else if (page < notebook.size())
            {
                notebook.at(page) = text;
                if(page < 9)
                {
                    text[0] = '\0';
                    page ++;
                    cout << page << endl;
                    if(page < notebook.size())
                    {
                        std::strcpy(text, notebook.at(page).c_str());
                    }
                }
            }
        }
        ImGui::PopID();
    }
    ImGui::End();
}

void pause::updateUserUsername(float profileWidth, float profileHeight)
{
    //todo - get rid of profile
    userProfile& usrProfile = userProfile::getInstance();
    draw_list->AddRectFilled(ImVec2(profileWidth + 20.f, profileHeight + 260.f), ImVec2(profileWidth + 830.f, profileHeight + 600.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Create New Username").x / 2.f) , 280.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Create New Username");

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("New Username").x / 2.f) , 390.f));
    ImGui::Text("New Username");
    ImGui::SameLine(); HelpMarker("A username should be between 3 and 48 character\nA Username MAY contain:\n  Lowercase letters: a-z\n  Uppercase letters: A-Z\n  Digits: 0-9\n  Special character: - _ .");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(425.f - 150.f, 410.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText(" ",createUsername, IM_ARRAYSIZE(createUsername), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(425.f - 65.f, 550.f));
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
    draw_list->AddRectFilled(ImVec2(profileWidth + 20.f, profileHeight + 260.f), ImVec2(profileWidth + 830.f, profileHeight + 600.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Create New Password").x / 2.f) , 280.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Create New Password");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Enter Current Password").x / 2.f) , 310.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Enter Current Password");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(425.f - 150.f, 330.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText(" ",currentPasswd, IM_ARRAYSIZE(currentPasswd), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Enter New Password").x / 2.f) , 360.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Enter New Password");
    ImGui::SameLine(); HelpMarker("Password must be at least 8 characters\nPassword must be no more than 48 character\nPassword MUST contain at least one from each group below:\n  lowercase letter: a-z\n  Uppercase letter: A-Z\n  Digit: 0-9\n  Special character: ~ ` ! @ # $ % & * ^ ( ) _ - + = \n                   { } : ; < > ?");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(425.f - 150.f, 380.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText("  ",createPasswd, IM_ARRAYSIZE(createPasswd), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Confirm New Password").x / 2.f) , 410.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Confirm New Password");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(425.f - 150.f, 430.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText("   ",confirmPasswd, IM_ARRAYSIZE(confirmPasswd), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    ImGui::SetCursorPos(ImVec2(425.f - 65.f, 550.f));
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

void pause::settingsMain()
{
    static int tempMain = 20;
    static int tempMusic = 20;
    static int tempSoundEffect = 20;

    ImGui::SetCursorPos(ImVec2(20.f, 60.f));
    ImGui::Text("Change Screen Resolution");

    ImGui::SetCursorPos(ImVec2(30.f, 80.f));
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
    ImGui::SetCursorPos(ImVec2(20.f, 120.f));
    ImGui::Text("Volume Controls: ");

    ImGui::SetCursorPos(ImVec2(30.f, 140.f));
    ImGui::SliderInt("Main", &tempMain, 0, 20);
    ImGui::SetCursorPos(ImVec2(30.f, 165.f));
    ImGui::SliderInt("Music", &tempMusic, 0, 20);
    ImGui::SetCursorPos(ImVec2(30.f, 190.f));
    ImGui::SliderInt("Sound Effects", &tempSoundEffect, 0, 20);
    // Change resolution
    // give warning on mac that resolution exceeds display size on anything over 1440x900 or just double the number displayed
}
void pause::settingsUser(imageHandler *image, characterBuilder *charBuild, characterManager *character)
{
    userProfile& usrProfile = userProfile::getInstance();
    const float profileWidth = windowWidth + paddingWidth + 10.f;
    const float profileHeight = paddingHeight;

    draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(ImVec2(profileWidth + 20.f, profileHeight + 70.f), ImVec2(profileWidth + 320.f, profileHeight + 230.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);
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
    ImGui::Text("%s", usrProfile.getJoinDate().c_str());

    ImGui::SetCursorPos(ImVec2(124.f, 150.f));
    ImGui::Text("Level: "); // todo - change to whatever gets implemented
    ImGui::SameLine();
    ImGui::Text("0");

    ImGui::SetCursorPos(ImVec2(124.f, 170.f));
    ImGui::Text("Problems Solved: "); // todo - get from database
    ImGui::SameLine();
    ImGui::Text("0");

    // change username

    ImGui::SetCursorPos(ImVec2(360.f, 90.f));
    ImGui::PushID(11);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
    if(ImGui::Button("Change Username", ImVec2(150.f, 50.f)))
    {
        usr_Username = !usr_Username;
        usr_Password = false;

        createUsername[0] = '\0';
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(360.f, 160.f));
    ImGui::PushID(22);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
    if(ImGui::Button("Change Password", ImVec2(150.f, 50.f)))
    {
        usr_Username = false;
        usr_Password = !usr_Password;

        currentPasswd[0] = '\0';
        createPasswd[0] = '\0';
        confirmPasswd[0] = '\0';
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2(530.f, 90.f));
    ImGui::PushID(33);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
    if(ImGui::Button("Change Character", ImVec2(150.f, 50.f)))
    {
        usr_Username = false;
        usr_Password = false;
        notebookWindow = false;
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
            notebookWindow = false;
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
            notebookWindow = false;
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