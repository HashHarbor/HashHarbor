#include "login.h"

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

#include <regex>
#include "../assets/font/IconsFontAwesome6.h"
#include "../backends/authentication/authentication.h"
#include "../backends/database/database.h"
#include "../imageHandler/imageHandler.h"

using std::string;
using std::vector;
using std::regex;

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled(ICON_FA_CIRCLE_QUESTION);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

login::login(int width, int height, imageHandler* imgHandler)
{
    width_px = width;
    height_px = height;

    minWidth = (width / 2.f) - 200.f;
    minHeight = (height / 2.f) - 240.f;
    // todo - add linux Support
    string path = std::filesystem::current_path().string() + "/assets/other/login.png";
    img = new imageHandler();
    bool ret = imgHandler->loadTexture(path.c_str(), img);
}

void login::drawLoginScreen(imageHandler* imgHandler)
{
    ImGui::SetNextWindowSize({(float)width_px, (float)height_px});
    ImGui::SetNextWindowPos({0, 0});

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    ImGui::Begin("Log In", NULL, window_flags);
    {
        drawBackground(imgHandler);

        draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(ImVec2(minWidth, minHeight), ImVec2(minWidth + 400.f, minHeight + 500.f), ImColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 20.0f);

        if(!createAccount)
        {
            drawLogin();
            if(errorAuth)
            {
                error_Auth();
            }
        }
        else
        {
            drawCreateUser();
            if(errorCmp)
            {
                error_Comp();
            }
            else if(errorCreate)
            {
                error_Create();
            }
        }
    }
    ImGui::End();
}

void login::drawLogin()
{
    static char username[64] = "";
    static char passwd[64] = "";

    static bool viewPasswd = false; // keep password hidden by default

    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("LOGIN").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 30.f));
    ImGui::Text("LOGIN");

    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Username").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 120.f));
    ImGui::Text("Username");

    ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 140.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText(" ",username, IM_ARRAYSIZE(username), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Password").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 180.f));
    ImGui::Text("Password");

    ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 210.f));
    ImGui::PushItemWidth(300);
    if(!viewPasswd)
    {
        ImGui::InputText("  ",passwd, IM_ARRAYSIZE(passwd), ImGuiInputTextFlags_Password);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(minWidth + 350.f, minHeight + 201.f));
        ImGui::PushID(8);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
        if(ImGui::Button(ICON_FA_EYE_SLASH, ImVec2(30.f,16.f)))
        {
            viewPasswd = true;
        }
        ImGui::PopStyleColor(1);
        ImGui::PopID();
    }
    else
    {
        ImGui::InputText("  ",passwd, IM_ARRAYSIZE(passwd), ImGuiInputTextFlags_None);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(minWidth + 350.f, minHeight + 201.f));
        ImGui::PushID(9);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
        if(ImGui::Button(ICON_FA_EYE, ImVec2(30.f,16.f)))
        {
            viewPasswd = false;
        }
        ImGui::PopStyleColor(1);
        ImGui::PopID();
    }

    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Don't have an account?").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x - 50.f, minHeight + 350.f));
    ImGui::Text("Don't have an account?");
    ImGui::SameLine();
    ImGui::PushID(2);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.71f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,0.33f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(200.f / 360.f,1.0f,0.61f));
    if(ImGui::Button("Sign Up", ImVec2(100.f,20.f)))
    {
        createAccount = true;
        viewPasswd = false;
        errorAuth = false;
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::SetCursorPos(ImVec2((width_px / 2.f - 50.f),minHeight + 400.f));
    ImGui::PushID(1);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(200.f / 360.f,1.0f,1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(200.f / 360.f,0.8f,1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(200.f / 360.f,1.0f,0.81f));
    if(ImGui::Button("Login", ImVec2(100.f,50.f)))
    {
        authentication auth = authentication();

        if(auth.inputValidation( username, passwd, true)) // this will validate the input and then authenticate the user
        {
            auth.getUser(_username, _id);
            STATUS = true;
        }
        else
        {
            errorAuth = true;
        }
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void login::drawCreateUser()
{
    static char createUsername[64] = "";
    static char createPasswd[64] = "";
    static char confirmPasswd[64] = "";

    static bool viewPasswd = false; // keep password hidden by default

    ImGui::SetCursorPos(ImVec2(minWidth + 20.f, minHeight + 20.f));
    ImGui::PushID(7);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
    if(ImGui::Button(ICON_FA_CHEVRON_LEFT, ImVec2(23.f,18.f)))
    {
        createAccount = false;
        errorCreate = false;
        errorCmp = false;
        viewPasswd = false; // turn off both the new account and viewing passwd
    }
    ImGui::PopStyleColor(1);
    ImGui::PopID();

    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Create Your Hash Harbor Account").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 30.f));
    ImGui::Text("Create Your Hash Harbor Account");

    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Username").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 70.f));
    ImGui::Text("Username");
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
    ImGui::SameLine(); HelpMarker("A username should be between 3 and 48 character\nA Username may contain:\n  Lowercase letters: a-z\n  Uppercase letters: A-Z\n  Digits: 0-9\n  Special character: - _ .");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 90.f));
    ImGui::PushItemWidth(300);
    ImGui::InputText(" ",createUsername, IM_ARRAYSIZE(createUsername), ImGuiInputTextFlags_None);
    ImGui::PopItemWidth();

    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Password").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 130.f));
    ImGui::Text("Password");
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
    ImGui::SameLine(); HelpMarker("Password must be at least 8 characters\nPassword must be no more than 48 character\nPassword MUST contain at least one from each group below:\n  lowercase letter: a-z\n  Uppercase letter: A-Z\n  Digit: 0-9\n  Special character: ~ ` ! @ # $ % & * ^ ( ) _ - + = \n                   { } : ; < > ?");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 150.f));
    ImGui::PushItemWidth(300);
    if(!viewPasswd)
    {
        ImGui::InputText("  ",createPasswd, IM_ARRAYSIZE(createPasswd), ImGuiInputTextFlags_Password);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(minWidth + 350.f, minHeight + 151.f));
        ImGui::PushID(5);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
        if(ImGui::Button(ICON_FA_EYE_SLASH, ImVec2(30.f,16.f)))
        {
            viewPasswd = true;
        }
        ImGui::PopStyleColor(1);
        ImGui::PopID();

        txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Confirm Password").x) / 2.f);
        ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 270.f));
        ImGui::Text("Confirm Password");

        ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 290.f));
        ImGui::PushItemWidth(300);
        ImGui::InputText("   ",confirmPasswd, IM_ARRAYSIZE(confirmPasswd), ImGuiInputTextFlags_Password);
        ImGui::PopItemWidth();
    }
    else
    {
        ImGui::InputText("  ",createPasswd, IM_ARRAYSIZE(createPasswd), ImGuiInputTextFlags_None);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(minWidth + 350.f, minHeight + 151.f));
        ImGui::PushID(6);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
        if(ImGui::Button(ICON_FA_EYE, ImVec2(30.f,16.f)))
        {
            viewPasswd = false;
        }
        ImGui::PopStyleColor(1);
        ImGui::PopID();

        txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Confirm Password").x) / 2.f);
        ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 270.f));
        ImGui::Text("Confirm Password");

        ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 290.f));
        ImGui::PushItemWidth(300);
        ImGui::InputText("   ",confirmPasswd, IM_ARRAYSIZE(confirmPasswd), ImGuiInputTextFlags_None);
        ImGui::PopItemWidth();
    }

    passwordGuide(createPasswd);

    ImGui::SetCursorPos(ImVec2((width_px / 2.f - 50.f),minHeight + 420.f));
    ImGui::PushID(3);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(200.f / 360.f,1.0f,1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(200.f / 360.f,0.44f,1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(200.f / 360.f,1.0f,0.61f));
    if(ImGui::Button("Sign Up", ImVec2(100.f,50.f)))
    {
        errorCmp = false;
        errorCreate = false;
        if(strcmp(createPasswd,confirmPasswd) != 0) // confirm passwords match
        {
            errorCmp = true;
            createPasswd[0] = '\0';
            confirmPasswd[0] = '\0';
        }
        else
        {
            authentication auth = authentication();
            if(auth.inputValidation( createUsername, createPasswd, false))
            {
                auth.getUser(_username, _id);
                STATUS = true;
            }
            else
            {
                errorCreate = true;
            }
        }
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void login::passwordGuide(string tempPasswd)
{
    if(tempPasswd.size() < 8)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 172.f));
        ImGui::Text(ICON_FA_XMARK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 170.f));
        ImGui::Text("Password Minimum Length: 8");
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,180,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 172.f));
        ImGui::Text(ICON_FA_CHECK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 170.f));
        ImGui::Text("Password Minimum Length: 8");
        ImGui::PopStyleColor();
    }

    if(!regex_search(tempPasswd, regex("[A-Z]")))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 192.f));
        ImGui::Text(ICON_FA_XMARK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 190.f));
        ImGui::Text("Uppercase Letters: A-Z");
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,180,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 192.f));
        ImGui::Text(ICON_FA_CHECK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 190.f));
        ImGui::Text("Uppercase Letters: A-Z");
        ImGui::PopStyleColor();
    }

    if(!regex_search(tempPasswd, regex("[a-z]")))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 212.f));
        ImGui::Text(ICON_FA_XMARK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 210.f));
        ImGui::Text("Lowercase Letters: a-z");
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,180,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 212.f));
        ImGui::Text(ICON_FA_CHECK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 210.f));
        ImGui::Text("Lowercase Letters: a-z");
        ImGui::PopStyleColor();
    }

    if(!regex_search(tempPasswd, regex("[0-9]")))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 232.f));
        ImGui::Text(ICON_FA_XMARK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 230.f));
        ImGui::Text("Numbers: 0-9");
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,180,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 232.f));
        ImGui::Text(ICON_FA_CHECK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 230.f));
        ImGui::Text("Numbers: 0-9");
        ImGui::PopStyleColor();
    }

    if(!regex_search(tempPasswd, regex("[~`!@#$%^&*()_+={}:;<>?-]")))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 252.f));
        ImGui::Text(ICON_FA_XMARK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 250.f));
        ImGui::Text("Special Characters: ~`!@#$%&^*()_-+={}:;<>?");
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,180,0,255));
        ImGui::SetCursorPos(ImVec2(minWidth + 60.f, minHeight + 252.f));
        ImGui::Text(ICON_FA_CHECK);
        ImGui::SetCursorPos(ImVec2(minWidth + 80.f, minHeight + 250.f));
        ImGui::Text("Special Characters: ~`!@#$%&^*()_-+={}:;<>?");
        ImGui::PopStyleColor();
    }
}

void login::error_Auth()
{
    draw_list->AddRectFilled(ImVec2(minWidth + 30.f, minHeight + 270.f), ImVec2(minWidth + 370.f, minHeight + 325.f), ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 20.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
    ImGui::SetCursorPos(ImVec2(minWidth + 40.f, minHeight + 279.f));
    ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);
    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Oops! Something went wrong.").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 277.f));
    ImGui::Text("Oops! Something went wrong.");
    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Please double-check your username and password.").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 300.f));
    ImGui::Text("Please double-check your username and password.");
    ImGui::PopStyleColor();
}
void login::error_Comp()
{
    draw_list->AddRectFilled(ImVec2(minWidth + 15.f, minHeight + 340.f), ImVec2(minWidth + 385.f, minHeight + 395.f), ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 20.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
    ImGui::SetCursorPos(ImVec2(minWidth + 25.f, minHeight + 349.f));
    ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);
    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Uh-oh! The passwords entered do not match.").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 347.f));
    ImGui::Text("Uh-oh! The passwords entered do not match.");
    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Please re-enter them.").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 370.f));
    ImGui::Text("Please re-enter them.");
    ImGui::PopStyleColor();
}
void login::error_Create()
{
    draw_list->AddRectFilled(ImVec2(minWidth + 15.f, minHeight + 340.f), ImVec2(minWidth + 385.f, minHeight + 395.f), ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 20.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
    ImGui::SetCursorPos(ImVec2(minWidth + 25.f, minHeight + 349.f));
    ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);
    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Account creation failed.").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 347.f));
    ImGui::Text("Account creation failed.");
    txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Please double-check your information and try again.").x) / 2.f);
    ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 370.f));
    ImGui::Text("Please double-check your information and try again.");
    ImGui::PopStyleColor();
}

void login::drawBackground(imageHandler* imgHandler)
{
    ImGui::SetCursorPos(ImVec2(0,0));
    imgHandler->DrawImage(*img, 1.0);
}
bool login::checkAuth()
{
    return STATUS;
}