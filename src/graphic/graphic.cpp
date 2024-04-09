#include "../../imgui/imgui.h"
#include "../../backends/imgui_impl_sdl.h"
#include "../../backends/imgui_impl_opengl3.h"
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
#include <fstream>
#include <thread>
#include <chrono>
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::ofstream;
#else
#include <bits/stdc++.h>
#endif

#include "graphic.h"
#include "textEditor/TextEditor.h"
#include "../imageHandler/imageHandler.h"
#include "../imageHandler/imagePath.h"
#include "../character/characterManager.h"
#include "../character/characterBuilder.h"
#include "../movement/movementHandler.h"
#include "../assets/font/IconsFontAwesome6.h"
#include "../login/login.h"
#include "../pause/pauseMenu.h"
#include "userProfile/userProfile.h"
#include "database/database.h"
#include "authentication/authentication.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"


void graphic::setup(){
    TextEditor editor;
    languages.push_back(TextEditor::LanguageDefinition::CPlusPlus());
    languages.push_back(TextEditor::LanguageDefinition::Python());
    languages.push_back(TextEditor::LanguageDefinition::Java());
    languages.push_back(TextEditor::LanguageDefinition::CSharp());

    editor.SetLanguageDefinition(languages[3]);
	// editor.SetPalette(TextEditor::GetLightPalette());

    vector<string> cppStart;
    cppStart.push_back("#include <iostream>");
    cppStart.push_back("int main() {");
    cppStart.push_back("\tstd::cout << \"Hello HashHarbor!\";");
    cppStart.push_back("\treturn 0;");
    cppStart.push_back("}");
    editor.SetTextLines(cppStart);

    static const char* fileToEdit = "solution";

    //texteditor setup ends here

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version;
#if defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    glsl_version = "#version 130";

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

#if defined(__APPLE__)
    this->width_px = 1320;
    this->height_px = 768;
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("HashHarbor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width_px, this->height_px, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    imagePath imgPth = imagePath();
    // Font Icon set up
    io.Fonts->AddFontDefault();
    float baseFontSize = 25.0f; // 13.0f is the size of the default font. Change to the font size you use.
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
#if defined(__APPLE__)
    string font_1 = imgPth.currentPath.string() + FONT_ICON_FILE_NAME_FAR;
    string font_2 = imgPth.currentPath.string() + FONT_ICON_FILE_NAME_FAS;
    string font_3 = imgPth.currentPath.string() + "/assets/font/NotoSans-Medium.ttf";
#else
    string font_1 = string("..") + FONT_ICON_FILE_NAME_FAR;
    string font_2 = string("..") + FONT_ICON_FILE_NAME_FAS;
    string font_3 = "../assets/font/NotoSans-Medium.ttf";
#endif
    io.Fonts->AddFontFromFileTTF(font_1.c_str(), iconFontSize, &icons_config, icons_ranges );
    io.Fonts->AddFontFromFileTTF( font_2.c_str(), iconFontSize, &icons_config, icons_ranges );

    ImFont* notoLarge = io.Fonts->AddFontFromFileTTF(font_3.c_str(), 25.f, NULL, io.Fonts->GetGlyphRangesDefault());
    ImFont* notoSmall = io.Fonts->AddFontFromFileTTF(font_3.c_str(), 15.f, NULL, io.Fonts->GetGlyphRangesDefault());

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    imageHandler image = imageHandler();
    characterManager character = characterManager();
    characterBuilder builder = characterBuilder(&image);
    userProfile& usrProfile = userProfile::getInstance();
    database& db = database::getInstance();
    db.connect();
    login Login = login(width_px, height_px, &image);
    pauseMenu Pause = pauseMenu(width_px, height_px);

    string pathMap;
    string intMap;
    string obsMap;
    string overlapMap;    
#if defined(__APPLE__)
    pathMap = imgPth.currentPath.string() + "/assets/map/abc.png";
    intMap = imgPth.currentPath.string() + + "/assets/map/int.png";
    obsMap = imgPth.currentPath.string() + "/assets/map/obs.png";
    overlapMap = imgPth.currentPath.string() + "/assets/map/overlap.png";
#else
    pathMap = "../assets/map/abc.png";
    intMap = "../assets/map/int.png";
    obsMap = "../assets/map/obs.png";
    overlapMap = "../assets/map/overlap.png";
#endif
    imageHandler background = imageHandler(pathMap.c_str());
    imageHandler overlap = imageHandler(overlapMap.c_str());
    imageHandler interactions = imageHandler(intMap.c_str());
    background.loadTexture(background.filepath, &background);
    overlap.loadTexture(overlap.filepath, &overlap);
    interactions.loadTexture(interactions.filepath, &interactions);

    character.createCharacter("USER", false, true, &image);
    character.setMainPlayer("USER");

    movementHandler move = movementHandler(obsMap, intMap, width_px, height_px);
    int lastAction = 0;
    // auto gr = move.getGrid();
    // cout << gr.size() << ", " << gr[0].size() << endl;
    // for(uint i = 0; i < gr.size(); i++){
    //     for(uint j = 0; j < gr[0].size(); j++){
    //         cout << gr[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    double mapGridX = 44.0f;
    double mapGridY = 42.0f;
    
    // Main loop
    bool done = false;
    auto lastFrameTime = std::chrono::steady_clock::now();
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if(changeScreenRes)
        {
            this->width_px = res.first;
            this->height_px = res.second;

            SDL_SetWindowSize(window, width_px, height_px);

            Login.updateResolution(res.first, res.second);
            move.adjustResolution(res.first, res.second);
            changeScreenRes = false;
        }

        if(reset)
        {
            usrProfile.clear();
            Login.reset();
            builder.reset();
            character.selectMainCharacter(&builder);
            // todo - add anything else that needs to be reeset

            allowMovement = false;
            show_login = true;
            show_display = false;
            show_codeEditor = false;
            show_config = false;
            show_charSelector = false;
            show_settings = false;
            show_blur = false;
            show_userProfile = false;

            reset = false;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        if (deltaTime < 1.0f / 60.0f) {
            std::this_thread::sleep_for(std::chrono::duration<float>(1.0f / 70.0f - deltaTime));
            currentTime = std::chrono::steady_clock::now();
            deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        }
        lastFrameTime = currentTime;

        if(ImGui::IsKeyPressedEx(ImGuiKey_Escape, false))
        {
            if(!show_charSelector && !show_login)
            {
                show_blur = !show_blur;
                show_settings = !show_settings;
                allowMovement= !allowMovement;

                resetPauseScreen = true;
            }
        }

        if(show_login)
        {
            makeLogIn(Login, image, character, builder);
        }

        if(show_display){
            makeCharacter(image, overlap, editor, mapGridX, mapGridY, move, lastAction, character, builder,  allowMovement);
            makeBackground(background, move.getGrid(), mapGridX, mapGridY, allowMovement);
        }

        if(show_codeEditor){
            makeCodeEditor(editor, fileToEdit, notoSmall);
        }

        if(show_userProfile){
            makeUserProfile(notoLarge, notoSmall, image, character, builder);
        }

        if(show_config){
            makeConfig(cppStart, editor);
        }

        if(show_blur){
            makeBlur();
        }

        if(show_settings)
        {
            makeSettings(Pause, image, character, builder, move, Login, done);
        }

        if(show_charSelector)
        {
            makeCharacterSelector(image, character, builder);
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup

    builder.cleanUp();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void graphic::makeBlur(){
    ImGui::SetNextWindowSize({(float)width_px, (float)height_px});
    ImGui::SetNextWindowPos({0, 0});

    ImGui::Begin("Blur", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs);
    {
    }

    ImGui::End();
}

void graphic::makeCharacter(imageHandler& image, imageHandler& overlap, TextEditor& editor, double &gridX, double &gridY, movementHandler& move, int &lastAction, characterManager &character, characterBuilder& charBuild, bool canMove)
{
    // Graphics window calculation
    ImGui::SetNextWindowSize({(float)width_px /2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 0});

    const float frameLength = 1.f / 10.f; // In seconds, so  FPS
    static float frameTimer = frameLength;
    static float arrowTimer = frameLength;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | 
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav;
    if(show_blur){
        flags |= ImGuiWindowFlags_NoInputs;
    }

    static int interact = false;

    // Window - Character
    ImGui::Begin("Character", NULL, flags);
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        if(characterCreated)
        {
            frameTimer -= ImGui::GetIO().DeltaTime;
            arrowTimer = frameTimer;

            ImVec2 characterPos = ImVec2((ImGui::GetContentRegionAvail() - ImVec2(32, 64)) * 0.5f) + ImVec2(8, 0) - ImVec2(0, 8);
            character.drawPos = characterPos;

            draw_list->AddCircleFilled(ImVec2(characterPos.x + 16.f, characterPos.y + 60.f), 13.f, ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f))); // shadow

            ImGui::SetCursorPos(characterPos);
            character.moveMainCharacter(&image, &charBuild, frameTimer, canMove);

            if (frameTimer <= 0.f)
            {
                frameTimer = frameLength;
            }
        }

        ImGui::SetCursorPos(ImVec2(0, 0));

        int keyDown = 0; // used to identify which direction the character is moving

        if(canMove){
            if(ImGui::IsKeyDown(ImGuiKey_W)) { keyDown = 1; }
            else if(ImGui::IsKeyDown(ImGuiKey_S)) { keyDown = 2; }
            else if(ImGui::IsKeyDown(ImGuiKey_D)) { keyDown = 3; }
            else if(ImGui::IsKeyDown(ImGuiKey_A)) { keyDown = 4; }
        }

        move.mapMovement(keyDown, overlap, gridX, gridY, move.getGrid().size(), move.getGrid()[0].size(), lastAction, interact);

        // cout << gridX << ", " << gridY << " and last action " << lastAction << endl;

        if(interact != 0){
            // ImGui::SetCursorPos(ImVec2((float)width_px/ 4 , (float)height_px / 4 ));
            ImGui::SetCursorPos(ImVec2((float)width_px/ 4 - 16, (float)height_px / 4 - 54));

            move.drawArrows(arrowTimer, lastAction);
            if (arrowTimer <= 0.f){
                arrowTimer = frameLength;
            }

            if(ImGui::IsKeyDown(ImGuiKey_W) && show_codeEditor == false){
                // cout << "trigger interaction here" << endl;
                show_codeEditor = !show_codeEditor;
                show_userProfile = !show_userProfile;
                allowMovement = !allowMovement;

                // codeStarter.clear();

                // codeStarter.push_back("#include <iostream>");
                // codeStarter.push_back("int main() {");
                // codeStarter.push_back("\tstd::cout << \"Hello HashHarbor!\";");
                // codeStarter.push_back("\treturn 0;");
                // codeStarter.push_back("}");
                // cout << selectedLanguageIndex << endl;
                editor.SetTextLines(codeStarter);

                result = "";
                
            }


            // cout << "interact true and timer is: " << frameTimer << endl;
        }

        ImGui::SetCursorPos(ImVec2(0, 0));

    }
    ImGui::End();
}

void graphic::triggerQuestion(int question){

    result = "";

}

void graphic::makeBackground(imageHandler background, vector<vector<int>> grid, double gridX, double gridY, bool canMove){
    ImGui::SetNextWindowSize({(float)width_px /2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 0});

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
    if(show_blur){
        flags |= ImGuiWindowFlags_NoInputs;
    }

    ImGui::Begin("Background", NULL, flags);
    {
        ImGui::SetCursorPos(ImVec2(0,0));
        background.DrawMap(background, gridX, gridY, (width_px / 2), (height_px / 2), grid.size(), grid[0].size());

    }

    ImGui::End();
}


void graphic::makeCodeEditor(TextEditor &editor, const char* fileToEdit, ImFont* fontSmall){
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar ;
    if(show_blur){
        flags |= ImGuiWindowFlags_NoInputs;
    }

    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px / 4});
    ImGui::SetNextWindowPos({(float)width_px / 2, (float)height_px / 4 * 3});

    // Submission window
    ImGui::Begin("Submission details", NULL, flags |= ImGuiWindowFlags_NoTitleBar );
    {

        if (ImGui::BeginTabBar("SubWindowTabs"))
        {

            if (ImGui::BeginTabItem("Code Execution"))
            {
                if (ImGui::Button("Run Code"))
                {
                    string textToSave = editor.GetText();
                    result = executeCPP(textToSave);
                }

                ImGui::Text("Results:");
                ImGui::InputTextMultiline(" ", const_cast<char*>(result.c_str()), result.size() + 1, ImVec2((float)width_px / 2 - 20, (height_px / 4) - 100), ImGuiInputTextFlags_ReadOnly);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Custom Test Case"))
            {
                ImGui::Text("To be implemented");

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Run All Tests"))
            {

                ImGui::Text("To be implemented");

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();

    int horizontalIndent = 20;
    int verticalIndent = 20;
    ImGui::SetNextWindowSize({(float)width_px / 2 - (2 * horizontalIndent), (float)height_px - (2 * verticalIndent)});
    ImGui::SetNextWindowPos({(float)horizontalIndent, (float)verticalIndent});

    flags |= ImGuiWindowFlags_NoTitleBar;
    if(show_blur){
        flags |= ImGuiWindowFlags_NoInputs;
    }

    

    ImGui::Begin("Question", NULL, flags);
    {
        static const char* languages[] = { "C++", "Python", "Java", "C#" }; // Add more languages as needed
        const char* currentLanguage = languages[selectedLanguageIndex];

        if (ImGui::Button("X")) {
            show_codeEditor = !show_codeEditor;
            show_userProfile = !show_userProfile;
            allowMovement = !allowMovement;
        }
        ImGui::SameLine(); // Place subsequent widgets on the same line

        // Open combo box
        if (ImGui::BeginCombo("Choose Language", currentLanguage))
        {
            for (int i = 0; i < IM_ARRAYSIZE(languages); ++i)
            {
                bool isSelected = (selectedLanguageIndex == i);
                if (ImGui::Selectable(languages[i], isSelected))
                {
                    selectedLanguageIndex = i;
                    currentLanguage = languages[i];
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus(); // Set the initially selected item
            }

            ImGui::EndCombo();
        }

        ImGui::PushFont(fontSmall);
        // TODO - implement code for all questions
        database &db = database::getInstance();
        database::questionData qes;
        db.getQuestion(1, qes);
        ImGui::TextWrapped(qes.question.c_str());
        ImGui::PopFont();
    }
    ImGui::End();


    editor.SetLanguageDefinition(languages[selectedLanguageIndex]);
    
    // Code Editor
    ImGui::SetNextWindowSize({(float)width_px / 2,(float)height_px / 4 * 3});
    ImGui::SetNextWindowPos({(float)width_px / 2, 0});

    auto cpos = editor.GetCursorPosition();
    editor.GetText();

    ImGui::Begin("Sandbox", NULL, flags);
    {

        ImGui::PushFont(fontSmall);
        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? " " : " ",
			editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

        if(!show_blur){
            editor.Render("TextEditor");
        }
        ImGui::PopFont();
		
    }
    ImGui::End();
}

void graphic::makeUserProfile(ImFont* fontLarge, ImFont* fontSmall, imageHandler& image, characterManager& character, characterBuilder& charBuild)
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
    if(show_blur){
        flags |= ImGuiWindowFlags_NoInputs;
    }

    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px / 3});
    ImGui::SetNextWindowPos({(float)width_px / 2, (float)height_px / 3 * 2});

    // Submission window
    ImGui::Begin("Inventory", NULL, flags);
    {

    }
    ImGui::End();

    float windowWidth = (float)width_px / 2;
    float windowHeight = (float)height_px / 3 * 2;
    // Code Editor
    ImGui::SetNextWindowSize({windowWidth,windowHeight});
    ImGui::SetNextWindowPos({(float)width_px / 2, 0});

    float profileWidth = ((float)width_px / 2) + (windowWidth / 2.f);
    float profileHeight = windowHeight / 2.f;

    ImGui::Begin("Profile", NULL, flags);
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        userProfile& usrProfile = userProfile::getInstance();

        // card gradient Blue
        ImU32 col_card_top = ImGui::GetColorU32(IM_COL32(140, 200, 255, 255));
        ImU32 col_card_bottom = ImGui::GetColorU32(IM_COL32(70, 150, 230, 255));

        draw_list->AddRectFilled(ImVec2(profileWidth  - 150.f, profileHeight - 200.f), ImVec2(profileWidth + 150.f, profileHeight + 200.f), ImColor(ImVec4(1.0f, 0.8f, 0.2f, 1.0f)), 20.f); // outer border
        draw_list->AddRectFilledMultiColor(ImVec2(profileWidth - 140.f, profileHeight - 190.f), ImVec2(profileWidth + 140.f, profileHeight + 190.f), col_card_bottom, col_card_top, col_card_bottom, col_card_top); // inner fill

        ImU32 col_gold = ImGui::GetColorU32(IM_COL32(255, 215, 0, 255));
        ImU32 col_pastel_gold = ImGui::GetColorU32(IM_COL32(255, 236, 139, 255));
        draw_list->AddRectFilled(ImVec2(profileWidth - 110.f, profileHeight - 150.f), ImVec2(profileWidth + 116.f, profileHeight + 16.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.2f)), 5.f); // character shade
        draw_list->AddRectFilledMultiColor(ImVec2(profileWidth - 110.f, profileHeight - 150.f), ImVec2(profileWidth + 110.f, profileHeight + 10.f), col_pastel_gold, col_gold, col_pastel_gold, col_gold); // character Border

        draw_list->AddRectFilled(ImVec2(profileWidth - 100.f, profileHeight + 18.f), ImVec2(profileWidth + 100.f, profileHeight + 32.f), ImColor(ImVec4(1.0f, 0.8f, 0.2f, 1.0f)), 10.f); // character shade

        // background behind character
        ImU32 col_sky_top = ImGui::GetColorU32(IM_COL32(190, 220, 255, 255));
        ImU32 col_sky_bottom = ImGui::GetColorU32(IM_COL32(100, 180, 255, 255));
        ImU32 col_grass_top = ImGui::GetColorU32(IM_COL32(120, 180, 120, 255));
        ImU32 col_grass_bottom = ImGui::GetColorU32(IM_COL32(160, 220, 160, 255));
        draw_list->AddRectFilledMultiColor(ImVec2(profileWidth - 105.f, profileHeight - 145.f), ImVec2(profileWidth + 105.f, profileHeight + 5.f), col_sky_top, col_sky_bottom, col_grass_top, col_grass_bottom); // background behind character

        draw_list->AddCircleFilled(ImVec2(profileWidth + 116.5f, profileHeight + 169.5f), 13.5f, ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.2f))); // shadow behind circle
        draw_list->AddCircleFilled(ImVec2(profileWidth + 115.f, profileHeight + 168.f), 12.f, ImColor(ImVec4(0.1f, 0.5f, 1.0f, 1.0f))); // circle behind logo
        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 108.f, (windowHeight / 2.f) + 163.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text(ICON_FA_BOLT_LIGHTNING);
        ImGui::PopStyleColor();

        draw_list->AddLine(ImVec2(profileWidth - 120.f, profileHeight + 93.f), ImVec2(profileWidth + 120.f, profileHeight + 95.f), IM_COL32(0, 0, 0, 255), 2.f);

        charBuild.drawCharacterAnimation(&image, ImVec2((windowWidth / 2.f) - 32.f, (windowHeight / 2.f) - 128.f), {ImVec2(64.1f / 192.f, 0.1f/512.f),ImVec2(95.99f/192.f, 64.f/512.f)}, 2.f, character.getMainPlayer()->dynamicIndex);

        // --- Write Text With Large Font --- //
        ImGui::PushFont(fontLarge);

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - (ImGui::CalcTextSize(usrProfile.getUsername().c_str()).x / 2.f), (windowHeight / 2.f) - 180.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text("%s", usrProfile.getUsername().c_str());
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 40.f, (windowHeight / 2.f) + 157.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
        ImGui::Text("123 HP");
        ImGui::PopStyleColor();

        ImGui::PopFont();

        // --- Write Text With Small Font --- //
        ImGui::PushFont(fontSmall);

        string join = "Joined: " + usrProfile.getJoinDate();
        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - (ImGui::CalcTextSize(join.c_str()).x / 2.f), (windowHeight / 2.f) + 17.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text("%s", join.c_str());

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 120.f, (windowHeight / 2.f) + 35.f));
        ImGui::PushTextWrapPos(fontSmall->FontSize * 35.f);
        ImGui::TextWrapped("Insert User Profile Text Here Insert User Profile Text Here Insert User Profile Text Here Insert User Profile Text Here Insert User Profile Text Here ");

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 120.f, (windowHeight / 2.f) + 105.f));
        ImGui::TextWrapped("Insert User Profile Text Here Insert User Profile Text Here Insert User Profile Text Here Insert User Profile Text Here Insert User Profile Text Here ");
        ImGui::PopTextWrapPos();
        ImGui::PopStyleColor();

        ImGui::PopFont();

    }
    ImGui::End();

}

void graphic::makeConfig(vector<string> &codeStarter, TextEditor &editor){
    // Config window calculation
    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px / 2});

    ImGui::SetNextWindowPos({0, (float)height_px/2});

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
    if(show_blur){
        flags |= ImGuiWindowFlags_NoInputs;
    }

    // Window - Config
    ImGui::Begin("Interactions", NULL, flags);
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


        // ImGui::Text(" ");
        // if(ImGui::Button("leetcode")){
        //     show_codeEditor = !show_codeEditor;
        //     show_userProfile = !show_userProfile;
        //     allowMovement = !allowMovement;

        //     codeStarter.clear();

        //     codeStarter.push_back("#include <iostream>");
        //     codeStarter.push_back("int main() {");
        //     codeStarter.push_back("\tstd::cout << \"Hello HashHarbor!\";");
        //     codeStarter.push_back("\treturn 0;");
        //     codeStarter.push_back("}");
        //     editor.SetTextLines(codeStarter);

        //     result = "";
        // }

    }
    ImGui::End();

}

void graphic::makeCharacterSelector(imageHandler& image, characterManager &character, characterBuilder& charBuild)
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 7.5f;

    ImGui::SetNextWindowSize({850.f, 520.f});
    ImGui::SetNextWindowPos({((float)width_px - 850.f) / 2.f,((float)height_px - 520.f) / 2.f});

    float factor = 4.f;
    const float frameLength = 5.f / 10.f; // In seconds, so  FPS
    static float frameTimer = frameLength;

    // Window - Config
    ImGui::Begin("Character Selector", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav);
    {
        frameTimer -= ImGui::GetIO().DeltaTime;
        ImVec2 characterPos = ImVec2((ImGui::GetContentRegionAvail() - ImVec2((32.f * factor), (64.f * factor))) * 0.25f);
        charBuild.drawPos = ImVec2(characterPos.x * 3.f, characterPos.y * 2.f);
        charBuild.drawCharacterBuilder(&image, frameTimer);

        if (frameTimer <= 0.f)
        {
            frameTimer = 5.f / 10.f;
        }

        //---setchar
        ImGui::SetCursorPos(ImVec2(characterPos.x * 3.f - 10.f, 450.f));
        ImGui::PushID(8);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("Select Character", ImVec2(150.f, 40.f)))
        {
            database& db = database::getInstance();
            character.selectMainCharacter(&charBuild);
            characterCreated = true;
            show_charSelector = false;
            show_blur = false;
            allowMovement = true;
            db.updateCharacter();
        }
        ImGui::PopStyleColor(3);
        
        ImGui::PopID();
    }
    ImGui::End();
    style.FrameRounding = 0.f;
}

string graphic::executeCPP(string code){
    // Step 1: Write code to a temporary file
    std::ofstream file("temp.cpp");
    file << code;
    file.close();

    // Step 2: Invoke the compiler and capture output
    std::string compileCommand = "g++ -o temp temp.cpp 2>&1"; // Redirect stderr to stdout
    FILE* pipe = popen(compileCommand.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error invoking compiler command." << std::endl;
        return "Error invoking compiler command.";
    }

    char buffer[128];
    std::string compileOutput = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            compileOutput += buffer;
    }
    pclose(pipe);

    if (!compileOutput.empty()) {  //has error
        remove("temp.cpp");
        return compileOutput;
    }

    FILE* executePipe = popen("./temp 2>&1", "r");
    if (!executePipe) {
        std::cerr << "Error invoking execute command." << std::endl;
        remove("temp.cpp");
        remove("temp");
        return "Error invoking execute command.";
    }

    char executeBuffer[128];
    std::string executeOutput = "";
    while (!feof(executePipe)) {
        if (fgets(executeBuffer, 128, executePipe) != NULL)
            executeOutput += executeBuffer;
    }
    pclose(executePipe);

    // Clean up temporary files
    remove("temp.cpp");
    remove("temp");

    return executeOutput;
}

void graphic::makeLogIn(login& Login, imageHandler& image, characterManager &character, characterBuilder& charBuild)
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 7.5f;
    style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    Login.drawLoginScreen(&image);
    if(Login.checkAuth())
    {
        show_display = true;
        show_userProfile = true;
        show_config = true;

        if(Login.checkChar())
        {
            charBuild.setCharacterFromDb();
            character.selectMainCharacter(&charBuild);
            characterCreated = true;
            show_charSelector = false;
            allowMovement = true;
        }
        else
        {
            characterCreated = false;
            show_charSelector = true;
        }

        style.FrameRounding = 0.f;
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

        show_login = false;
    }
}

void graphic::makeSettings(pauseMenu& Pause, imageHandler& image, characterManager &character, characterBuilder& charBuild, movementHandler& movement, login& Login, bool& done)
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 7.5f;

    static bool updateCharacter = false;

    if(resetPauseScreen)
    {
        Pause.reset();
        resetPauseScreen = false;
    }

    Pause.drawPauseMenu(&image, &character, &charBuild, &changeScreenRes, &res, &updateCharacter, &reset, &done);

    if(updateCharacter)
    {
        database& db = database::getInstance();
        character.selectMainCharacter(&charBuild);
        db.updateCharacter();
        show_settings = !show_settings;
        show_blur = !show_blur;
        allowMovement = !allowMovement;

        resetPauseScreen = true;
        updateCharacter = false;
    }
    style.FrameRounding = 0.f;
}