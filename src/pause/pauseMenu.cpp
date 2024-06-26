#include "pauseMenu.h"

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
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(226.f / 360.f,1.0f,0.88f));
    ImGui::Text(ICON_FA_CIRCLE_QUESTION);
    ImGui::PopStyleColor();
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

pauseMenu::pauseMenu(int width, int height, ImFont* font_15, ImFont* font_18, ImFont* font_21)
{
    width_px = width;
    height_px = height;

    switch(width_px)
    {
        case 1180:
            e = 0;
            break;
        case 1320:
            e = 1;
            break;
        case 1760:
            e = 2;
            break;
        case 2048:
            e = 3;
            break;
    }

    windowWidth = 320.f;
    windowHeight = 620.f; // allow for 50px padding on a 1280x720 window
    paddingHeight = ((float)height_px - windowHeight) / 2.f;
    paddingWidth = ((float)width_px - (320.f + 860.f)) / 2.f;

    noto_15 = font_15;
    noto_18 = font_18;
    noto_21 = font_21;
}

void pauseMenu::drawPauseMenu(imageHandler *image, characterManager *character, characterBuilder *charBuild, bool *changeScreenRes, pair<int,int>* res, bool *updateCharacter, bool *reset, bool *done, int* font)
{
    // userProfile& usrProfile = userProfile::getInstance();

    mainControls();

    if(settingsWindow)
    {
        drawSettingsWindow(image, charBuild, character, changeScreenRes, res, font);
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

void pauseMenu::reset()
{
    settingsWindow = false;
    notebookWindow = false;
    characterWindow = false;
    logOutWindow = false;
    quitWindow = false;

    usr_Username = false;
    usr_Password = false;

    usr_Password_Success = false;
    usr_Password_Fail = false;
    usr_Password_FailCmp = false;
    usr_Username_Success = false;
    usr_Username_Fail = false;

    createUsername[0] = '\0';
    currentPasswd[0] = '\0';
    createPasswd[0] = '\0';
    confirmPasswd[0] = '\0';
}

void pauseMenu::mainControls()
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
void pauseMenu::drawSettingsWindow(imageHandler *image, characterBuilder *charBuild, characterManager *character, bool* changeScreenRes, pair<int,int>* res, int* font)
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
                settingsMain(changeScreenRes, res, font);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("User Account"))
            {
                settingsUser(image, charBuild, character);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Credits"))
            {
                settingsCredits();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void pauseMenu::drawNotebookWindow()
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

        static long unsigned int page = 0;
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
        if((int)(page + 1) >= maxPage) // update page limit on display
        {
            maxPage = page + 1;
        }
        ImGui::Text("Characters: %d / 2048           Page: %d / %d", textSize, (int)(page + 1), maxPage);

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

void pauseMenu::updateUserUsername(float profileWidth, float profileHeight)
{
    userProfile& usrProfile = userProfile::getInstance();
    draw_list->AddRectFilled(ImVec2(profileWidth + 20.f, profileHeight + 260.f), ImVec2(profileWidth + 830.f, profileHeight + 600.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Create New Username").x / 2.f) , 280.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Create New Username");

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("New Username").x / 2.f) , 390.f));
    ImGui::Text("New Username");
    ImGui::SameLine(); HelpMarker("A username should be between 3 and 16 character\nA Username MAY contain:\n  Lowercase letters: a-z\n  Uppercase letters: A-Z\n  Digits: 0-9\n  Special character: - _ .");
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
        usr_Username_Success = false;
        usr_Username_Fail = false;
        authentication auth = authentication();
        if(auth.changeUsername(createUsername, usrProfile.getId()))
        {
            usr_Username_Success = true;
        }
        else
        {
            usr_Username_Fail = true;
        }
        createUsername[0] = '\0';
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    if(usr_Username_Success)
    {
        updateUsernameSuccess(profileWidth, profileHeight);
    }
    else if(usr_Username_Fail)
    {
        updateUsernameError(profileWidth, profileHeight);
    }
}
void pauseMenu::updateUserPassword(float profileWidth, float profileHeight)
{
    draw_list->AddRectFilled(ImVec2(profileWidth + 20.f, profileHeight + 260.f), ImVec2(profileWidth + 830.f, profileHeight + 600.f), ImColor(ImVec4(1.f, 1.f, 1.f, 0.6f)), 20.0f);

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Create New Password").x / 2.f) , 280.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Create New Password");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Enter Current Password").x / 2.f) , 310.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Enter Current Password");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Enter New Password").x / 2.f) , 360.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Enter New Password");
    ImGui::SameLine(); HelpMarker("Password must be at least 8 characters\nPassword must be no more than 48 character\nPassword MUST contain at least one from each group below:\n  lowercase letter: a-z\n  Uppercase letter: A-Z\n  Digit: 0-9\n  Special character: ~ ` ! @ # $ % & * ^ ( ) _ - + = \n                   { } : ; < > ?");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(425.f - (ImGui::CalcTextSize("Confirm New Password").x / 2.f) , 410.f));
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.0f));
    ImGui::Text("Confirm New Password");
    ImGui::PopStyleColor();


    if(!viewPasswd)
    {
        ImGui::SetCursorPos(ImVec2(425.f + 200.f, 375.f));
        ImGui::PushID(54321);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        if(ImGui::Button(ICON_FA_EYE_SLASH, ImVec2(50.f,30.f)))
        {
            viewPasswd = true;
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(425.f - 150.f, 330.f));
        ImGui::PushItemWidth(300);
        ImGui::InputText(" ",currentPasswd, IM_ARRAYSIZE(currentPasswd), ImGuiInputTextFlags_Password);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(425.f - 150.f, 380.f));
        ImGui::PushItemWidth(300);
        ImGui::InputText("  ",createPasswd, IM_ARRAYSIZE(createPasswd), ImGuiInputTextFlags_Password);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(425.f - 150.f, 430.f));
        ImGui::PushItemWidth(300);
        ImGui::InputText("   ",confirmPasswd, IM_ARRAYSIZE(confirmPasswd), ImGuiInputTextFlags_Password);
        ImGui::PopItemWidth();
    }
    else
    {
        ImGui::SetCursorPos(ImVec2(425.f + 200.f, 375.f));
        ImGui::PushID(654321);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        if(ImGui::Button(ICON_FA_EYE, ImVec2(50.f,30.f)))
        {
            viewPasswd = false;
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();

        ImGui::SetCursorPos(ImVec2(425.f - 150.f, 330.f));
        ImGui::PushItemWidth(300);
        ImGui::InputText(" ",currentPasswd, IM_ARRAYSIZE(currentPasswd), ImGuiInputTextFlags_None);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(425.f - 150.f, 380.f));
        ImGui::PushItemWidth(300);
        ImGui::InputText("  ",createPasswd, IM_ARRAYSIZE(createPasswd), ImGuiInputTextFlags_None);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(425.f - 150.f, 430.f));
        ImGui::PushItemWidth(300);
        ImGui::InputText("   ",confirmPasswd, IM_ARRAYSIZE(confirmPasswd), ImGuiInputTextFlags_None);
        ImGui::PopItemWidth();
    }

    ImGui::SetCursorPos(ImVec2(425.f - 65.f, 550.f));
    ImGui::PushID(44);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
    if(ImGui::Button("Update Password", ImVec2(130.f, 30.f)))
    {
        usr_Password_Success = false;
        usr_Password_Fail = false;
        usr_Password_FailCmp = false;
        viewPasswd = false;
        if(strcmp(createPasswd,confirmPasswd) != 0) // if new password and confirmation dont match
        {
            usr_Password_FailCmp = true;
        }
        else
        {
            authentication auth = authentication();
            if(auth.changePassword(currentPasswd, createPasswd))
            {
                usr_Password_Success = true;
            }
            else
            {
                usr_Password_Fail = true;
            }
        }
        currentPasswd[0] = '\0';
        createPasswd[0] = '\0';
        confirmPasswd[0] = '\0';
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    if(usr_Password_Success)
    {
        updatePasswordSuccess(profileWidth, profileHeight);
    }
    else if(usr_Password_Fail)
    {
        updatePasswordError(profileWidth, profileHeight);
    }
    else if(usr_Password_FailCmp)
    {
        updatePasswordError_Cmp(profileWidth, profileHeight);
    }
}

void pauseMenu::updateUsernameSuccess(float profileWidth, float profileHeight)
{
    draw_list->AddRectFilled(ImVec2(profileWidth + 150, profileHeight + 470.f), ImVec2(profileWidth + 700.f, profileHeight + 530.f), ImColor(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), 20.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
    ImGui::SetCursorPos(ImVec2(175.f, 495.f));
    ImGui::Text(ICON_FA_USER_CHECK);
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("Hooray!").x) / 2.f), 482.f));
    ImGui::Text("Hooray!");
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("You've successfully set up your new username.").x) / 2.f), 502.f));
    ImGui::Text("You've successfully set up your new username.");
    ImGui::PopStyleColor();
}
void pauseMenu::updateUsernameError(float profileWidth, float profileHeight)
{
    draw_list->AddRectFilled(ImVec2(profileWidth + 150, profileHeight + 470.f), ImVec2(profileWidth + 700.f, profileHeight + 530.f), ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 20.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
    ImGui::SetCursorPos(ImVec2(175.f, 495.f));
    ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("Oops! We couldn't process your username request.").x) / 2.f), 482.f));
    ImGui::Text("Oops! We couldn't process your username request.");
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("Please try again later.").x) / 2.f), 502.f));
    ImGui::Text("Please try again later.");
    ImGui::PopStyleColor();
}
void pauseMenu::updatePasswordError(float profileWidth, float profileHeight)
{
    draw_list->AddRectFilled(ImVec2(profileWidth + 150, profileHeight + 470.f), ImVec2(profileWidth + 700.f, profileHeight + 530.f), ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 20.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
    ImGui::SetCursorPos(ImVec2(175.f, 495.f));
    ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("Account creation failed.").x) / 2.f), 482.f));
    ImGui::Text("Account creation failed.");
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("Please double-check your information and try again.").x) / 2.f), 502.f));
    ImGui::Text("Please double-check your information and try again.");
    ImGui::PopStyleColor();
}
void pauseMenu::updatePasswordError_Cmp(float profileWidth, float profileHeight)
{
    draw_list->AddRectFilled(ImVec2(profileWidth + 150, profileHeight + 470.f), ImVec2(profileWidth + 700.f, profileHeight + 530.f), ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 20.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
    ImGui::SetCursorPos(ImVec2(175.f, 495.f));
    ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("Uh-oh! The passwords entered do not match.").x) / 2.f), 482.f));
    ImGui::Text("Uh-oh! The passwords entered do not match.");
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("Please re-enter them.").x) / 2.f), 502.f));
    ImGui::Text("Please re-enter them.");
    ImGui::PopStyleColor();
}
void pauseMenu::updatePasswordSuccess(float profileWidth, float profileHeight)
{
    draw_list->AddRectFilled(ImVec2(profileWidth + 150, profileHeight + 470.f), ImVec2(profileWidth + 700.f, profileHeight + 530.f), ImColor(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), 20.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
    ImGui::SetCursorPos(ImVec2(175.f, 495.f));
    ImGui::Text(ICON_FA_KEY);
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("Password updated!").x) / 2.f), 482.f));
    ImGui::Text("Password updated!");
    ImGui::SetCursorPos(ImVec2(425.f - ((ImGui::CalcTextSize("You can now access your account with your new password.").x) / 2.f), 502.f));
    ImGui::Text("You can now access your account with your new password.");
    ImGui::PopStyleColor();
}

void pauseMenu::settingsMain(bool* changeScreenRes, pair<int,int>* res, int* font)
{
    draw_list = ImGui::GetWindowDrawList();

    static int tempMain = 20;
    static int tempMusic = 20;
    static int tempSoundEffect = 20;

    ImGui::SetCursorPos(ImVec2(20.f, 60.f));
    ImGui::Text("Change Screen Resolution");

    ImGui::SetCursorPos(ImVec2(30.f, 80.f));
#if defined(__APPLE__)
    const float profileWidth = windowWidth + paddingWidth + 10.f;
    const float profileHeight = paddingHeight;

    static int k = 0;

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Screen Resolution", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Mac Retina Display"))
        {
            if(e > 1)
            {
                draw_list->AddRectFilled(ImVec2(profileWidth + 2.f, profileHeight + 100.f), ImVec2(profileWidth + (ImGui::CalcTextSize("Current Settings Exceed recommended settings for Mac Retina Displays").x) + 15.f, profileHeight + 119.f), ImColor(ImVec4(1.f, 0.0f, 0.0f, 0.5f)), 20.0f);
                ImGui::Text("Current Settings Exceed recommended settings for Mac Retina Displays");
            }

            ImGui::RadioButton("Small", &e, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Mac Retina Default", &e, 1);

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("External Display"))
        {
            ImGui::RadioButton("Small", &e, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Medium", &e, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Default", &e, 2);
            ImGui::SameLine();
            ImGui::RadioButton("Large", &e, 3);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
#else
    static int k = 0;
    ImGui::RadioButton("Small", &e, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Medium", &e, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Default", &e, 2);
    ImGui::SameLine();
    ImGui::RadioButton("Large", &e, 3);
#endif
    if(k != e)
    {
        switch(e)
        {
            case 0:
                width_px = 1180;
                height_px = 686;
                *changeScreenRes = true;
                *res = {1180, 686};
                break;
            case 1:
                width_px = 1320;
                height_px = 768;
                *changeScreenRes = true;
                *res = {1320, 768};
                break;
            case 2:
                width_px = 1760;
                height_px = 1024;
                *changeScreenRes = true;
                *res = {1760, 1024};
                break;
            case 3:
                width_px = 2048;
                height_px = 1191;
                *changeScreenRes = true;
                *res = {2048, 1191};
                break;
        }
        k = e;
        paddingHeight = ((float)height_px - windowHeight) / 2.f;
        paddingWidth = ((float)width_px - (320.f + 860.f)) / 2.f;
    }

    ImGui::Separator();

    ImGui::SetCursorPos(ImVec2(20.f, 150.f));
    ImGui::Text("Volume Controls: ");

    ImGui::SetCursorPos(ImVec2(30.f, 170.f));
    ImGui::SliderInt("Main", &tempMain, 0, 20);
    ImGui::SetCursorPos(ImVec2(30.f, 195.f));
    ImGui::SliderInt("Music", &tempMusic, 0, 20);
    ImGui::SetCursorPos(ImVec2(30.f, 220.f));
    ImGui::SliderInt("Sound Effects", &tempSoundEffect, 0, 20);

    ImGui::Separator();

    ImGui::SetCursorPos(ImVec2(30.f, 250.f));
    ImGui::Text("Code Editor: ");
    ImGui::SetCursorPos(ImVec2(40.f, 270.f));
    ImGui::Text("Font Size: ");

    ImGui::SetCursorPos(ImVec2(50.f, 290.f));
    ImGui::RadioButton("Default", font, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Medium", font, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Large", font, 2);

    ImGui::SetCursorPos(ImVec2(50.f, 310.f));
    switch(*font)
    {
        case 0:
            ImGui::PushTextWrapPos(noto_15->FontSize * 35.f);
            ImGui::PushFont(noto_15);
            break;
        case 1:
            ImGui::PushTextWrapPos(noto_18->FontSize * 30.f);
            ImGui::PushFont(noto_18);
            break;
        case 2:
            ImGui::PushTextWrapPos(noto_21->FontSize * 25.f);
            ImGui::PushFont(noto_21);
            break;
        default:
            ImGui::PushTextWrapPos(noto_15->FontSize * 35.f);
            ImGui::PushFont(noto_15);
    }
    ImGui::Text("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.", 30.f);
    draw_list->AddRect(ImVec2(windowWidth + paddingWidth + 45.f, paddingHeight + 310.f), ImVec2(windowWidth + paddingWidth + 540.f, paddingHeight + 360.f),IM_COL32(190, 190, 190, 255));
    ImGui::PopTextWrapPos();
    ImGui::PopFont();

    ImGui::SetCursorPos(ImVec2(30.f, 380.f));
    ImGui::Separator();
    ImGui::SetCursorPos(ImVec2(30.f, 390.f));
    ImGui::Text("%s", "Game Controls");
    ImGui::SetCursorPos(ImVec2(40.f, 410.f));
    ImGui::Text("%s", "Movement");

    draw_list->AddRect(ImVec2(windowWidth + paddingWidth + 80.f, paddingHeight + 430.f), ImVec2(windowWidth + paddingWidth + 100.f, paddingHeight + 450.f),IM_COL32(190, 190, 190, 255), 2.f); // W
    draw_list->AddRect(ImVec2(windowWidth + paddingWidth + 50.f, paddingHeight + 460.f), ImVec2(windowWidth + paddingWidth + 70.f, paddingHeight + 480.f),IM_COL32(190, 190, 190, 255), 2.f); // A
    draw_list->AddRect(ImVec2(windowWidth + paddingWidth + 80.f, paddingHeight + 460.f), ImVec2(windowWidth + paddingWidth + 100.f, paddingHeight + 480.f),IM_COL32(190, 190, 190, 255), 2.f); // S
    draw_list->AddRect(ImVec2(windowWidth + paddingWidth + 110.f, paddingHeight + 460.f), ImVec2(windowWidth + paddingWidth + 130.f, paddingHeight + 480.f),IM_COL32(190, 190, 190, 255), 2.f); // D
    draw_list->AddRect(ImVec2(windowWidth + paddingWidth + 350.f, paddingHeight + 460.f), ImVec2(windowWidth + paddingWidth + 370.f, paddingHeight + 480.f),IM_COL32(190, 190, 190, 255), 2.f); // Q

    draw_list->AddLine(ImVec2(windowWidth + paddingWidth + 100.f, paddingHeight + 440.f), ImVec2(windowWidth + paddingWidth + 175.f, paddingHeight + 440.f),IM_COL32(190, 190, 190, 255), 1.f);
    draw_list->AddLine(ImVec2(windowWidth + paddingWidth + 130.f, paddingHeight + 470.f), ImVec2(windowWidth + paddingWidth + 175.f, paddingHeight + 470.f),IM_COL32(190, 190, 190, 255), 1.f);
    draw_list->AddLine(ImVec2(windowWidth + paddingWidth + 90.f, paddingHeight + 500.f), ImVec2(windowWidth + paddingWidth + 175.f, paddingHeight + 500.f),IM_COL32(190, 190, 190, 255), 1.f);
    draw_list->AddLine(ImVec2(windowWidth + paddingWidth + 60.f, paddingHeight + 530.f), ImVec2(windowWidth + paddingWidth + 175.f, paddingHeight + 530.f),IM_COL32(190, 190, 190, 255), 1.f);
    draw_list->AddLine(ImVec2(windowWidth + paddingWidth + 90.f, paddingHeight + 480.f), ImVec2(windowWidth + paddingWidth + 90.f, paddingHeight + 500.f),IM_COL32(190, 190, 190, 255), 1.f);
    draw_list->AddLine(ImVec2(windowWidth + paddingWidth + 60.f, paddingHeight + 480.f), ImVec2(windowWidth + paddingWidth + 60.f, paddingHeight + 530.f),IM_COL32(190, 190, 190, 255), 1.f);
    draw_list->AddLine(ImVec2(windowWidth + paddingWidth + 370.f, paddingHeight + 470.f), ImVec2(windowWidth + paddingWidth + 390.f, paddingHeight + 470.f),IM_COL32(190, 190, 190, 255), 1.f);

    ImGui::PushFont(noto_18);
    ImGui::SetCursorPos(ImVec2(73.5f, 430.f));
    ImGui::Text("%s", "W");
    ImGui::SetCursorPos(ImVec2(46.f, 460.f));
    ImGui::Text("%s", "A");
    ImGui::SetCursorPos(ImVec2(76.5f, 460.f));
    ImGui::Text("%s", "S");
    ImGui::SetCursorPos(ImVec2(105.f, 460.f));
    ImGui::Text("%s", "D");
    ImGui::SetCursorPos(ImVec2(345.f, 460.f));
    ImGui::Text("%s", "Q");
    ImGui::PopFont();

    ImGui::SetCursorPos(ImVec2(170.f, 432.f));
    ImGui::Text("%s", "Up");
    ImGui::SetCursorPos(ImVec2(170.f, 462.f));
    ImGui::Text("%s", "Right");
    ImGui::SetCursorPos(ImVec2(170.f, 492.f));
    ImGui::Text("%s", "Down");
    ImGui::SetCursorPos(ImVec2(170.f, 522.f));
    ImGui::Text("%s", "Left");
    ImGui::SetCursorPos(ImVec2(385.f, 462.f));
    ImGui::Text("%s", "Interact");
}
void pauseMenu::settingsUser(imageHandler *image, characterBuilder *charBuild, characterManager *character)
{
    userProfile& usrProfile = userProfile::getInstance();
    const float profileWidth = windowWidth + paddingWidth + 10.f;
    const float profileHeight = paddingHeight;

    draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(ImVec2(profileWidth + 20.f, profileHeight + 70.f), ImVec2(profileWidth + 320.f, profileHeight + 230.f), ImColor(ImVec4(0.6f, 0.6f, 0.6f, 1.0f)), 20.0f);
    draw_list->AddRectFilled(ImVec2(profileWidth + 35.f, profileHeight + 85.f), ImVec2(profileWidth + 109.f, profileHeight + 218.f), ImColor(ImVec4(0.9f, 0.9f, 0.9f, 1.0f)), 20.0f);

    charBuild->drawCharacterAnimation(image, ImVec2(40.f, 85.f), {ImVec2(0.1f / 192.f, 0.1f/512.f),ImVec2(31.99f/192.f, 64.f/512.f)}, 2.f, character->getMainPlayer()->dynamicIndex);
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

        usr_Password_Success = false;
        usr_Password_Fail = false;
        usr_Password_FailCmp = false;
        usr_Username_Success = false;
        usr_Username_Fail = false;
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
        viewPasswd = false;

        currentPasswd[0] = '\0';
        createPasswd[0] = '\0';
        confirmPasswd[0] = '\0';

        usr_Password_Success = false;
        usr_Password_Fail = false;
        usr_Password_FailCmp = false;
        usr_Username_Success = false;
        usr_Username_Fail = false;
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();


    if(usr_Username)
    {
        updateUserUsername(profileWidth, profileHeight);
    }
    else if(usr_Password)
    {
        updateUserPassword(profileWidth, profileHeight);
    }
}
void pauseMenu::settingsCredits()
{
    ImGui::Text("%s", "INSERT CREDITS");
}

void pauseMenu::drawCharacterCreatorWindow(imageHandler *image, characterBuilder *charBuild, bool *updateCharacter)
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
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
    ImGui::End();
}

void pauseMenu::drawLogOutWindow(bool* reset)
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
void pauseMenu::drawQuitWindow(bool* done)
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

void pauseMenu::updateResolution(int w, int h)
{
    width_px = w;
    height_px = h;

    switch(width_px)
    {
        case 1180:
            e = 0;
            break;
        case 1320:
            e = 1;
            break;
        case 1760:
            e = 2;
            break;
        case 2048:
            e = 3;
            break;
    }

    paddingHeight = ((float)height_px - windowHeight) / 2.f;
    paddingWidth = ((float)width_px - (320.f + 860.f)) / 2.f;
}