#include "../../imgui/imgui.h"
#include "../../backends/imgui_impl_sdl.h"
#include "../../backends/imgui_impl_opengl3.h"
// #include "cpptoml.h"
#include <stdio.h>
#include <thread>

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
#include "../character/characterConfig.h"
#include "../movement/movementHandler.h"
#include "../assets/font/IconsFontAwesome6.h"
#include "../login/login.h"
#include "../pause/pauseMenu.h"
#include "userProfile/userProfile.h"
#include "database/database.h"
#include "authentication/authentication.h"
#include "configReader/configReader.h"

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
    //string font_3 = imgPth.currentPath.string() + "/assets/font/NotoSans-Medium.ttf";
    //string font_4 = imgPth.currentPath.string() + "/assets/font/NovaFlat-Regular.ttf";
    string font_3 = "../assets/font/NotoSans-Medium.ttf";
    string font_4 = "../assets/font/NovaFlat-Regular.ttf";
#else
    string font_1 = string("..") + FONT_ICON_FILE_NAME_FAR;
    string font_2 = string("..") + FONT_ICON_FILE_NAME_FAS;
    string font_3 = "../assets/font/NotoSans-Medium.ttf";
    string font_4 = "../assets/font/NovaFlat-Regular.ttf";
#endif
    io.Fonts->AddFontFromFileTTF(font_1.c_str(), iconFontSize, &icons_config, icons_ranges );
    io.Fonts->AddFontFromFileTTF( font_2.c_str(), iconFontSize, &icons_config, icons_ranges );

    ImFont* noto_15 = io.Fonts->AddFontFromFileTTF(font_3.c_str(), 15.f, NULL, io.Fonts->GetGlyphRangesDefault());
    ImFont* noto_18 = io.Fonts->AddFontFromFileTTF(font_3.c_str(), 18.f, NULL, io.Fonts->GetGlyphRangesDefault());
    ImFont* noto_21 = io.Fonts->AddFontFromFileTTF(font_3.c_str(), 21.f, NULL, io.Fonts->GetGlyphRangesDefault());
    ImFont* noto_25 = io.Fonts->AddFontFromFileTTF(font_3.c_str(), 25.f, NULL, io.Fonts->GetGlyphRangesDefault());

    ImFont* noto_Flat = io.Fonts->AddFontFromFileTTF(font_4.c_str(), 25.f, NULL, io.Fonts->GetGlyphRangesDefault());

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
    pauseMenu Pause = pauseMenu(width_px, height_px, noto_15, noto_18, noto_21);

#if defined(__APPLE__)
    //pathMap = imgPth.currentPath.string() + "/assets/map/town1/house6/map.png";
    //intMap = imgPth.currentPath.string() + + "/assets/map//town1/house6/int.png";
    //obsMap = imgPth.currentPath.string() + "/assets/map//town1/house6/obs.png";
    //overlapMap = imgPth.currentPath.string() + "/assets/map//town1/house6/overlap.png";

    pathMap = "../assets/map/town1/house6/map.png";
    intMap = "../assets/map/town1/house6/int.png";
    obsMap = "../assets/map/town1/house6/obs.png";
    overlapMap = "../assets/map/town1/house6/overlap.png";
#else
    pathMap = "../assets/map/town1/house6/map.png";
    intMap = "../assets/map/town1/house6/int.png";
    obsMap = "../assets/map/town1/house6/obs.png";
    overlapMap = "../assets/map/town1/house6/overlap.png";
#endif
    background = imageHandler(pathMap.c_str());
    overlap = imageHandler(overlapMap.c_str());
    interactions = imageHandler(intMap.c_str());
    background.loadTexture(background.filepath, &background);
    overlap.loadTexture(overlap.filepath, &overlap);
    interactions.loadTexture(interactions.filepath, &interactions);

    world = "town1/";
    room = "house6/";

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

    double mapGridX = 28.0f;
    double mapGridY = 19.0f;
    
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
            Pause.updateResolution(res.first, res.second);
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

                resetPauseScreen = true;
            }
        }

        if(show_login)
        {
            makeLogIn(Login, image, character, builder);
        }

        if(show_display){
            makeCharacter(image, editor, mapGridX, mapGridY, move, lastAction, character, builder);
            makeBackground(background, move.getGrid(), mapGridX, mapGridY, image, character, builder, lastAction);
        }

        if(show_codeEditor){
            makeCodeEditor(editor, fileToEdit, noto_15, noto_18, noto_21);

        }

        if(show_userProfile){
            makeUserProfile(noto_25, noto_21, noto_Flat,  image, character, builder);
        }

        if(show_config){
            makeConfig(noto_18);
        }

        if(show_codeEditor || show_blur){
            allowMovement = false;
        }
        else{
            allowMovement = true;
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

void graphic::makeCharacter(imageHandler& image, TextEditor& editor, double &gridX, double &gridY, movementHandler& move, int &lastAction, characterManager &character, characterBuilder& charBuild)
{
    // Graphics window calculation
    ImGui::SetNextWindowSize({(float)width_px /2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 0});

    const float frameLength = 1.f / 10.f; // In seconds, so  FPS
    static float frameTimer = frameLength;
    static float arrowTimer = frameLength;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | 
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMouseInputs;
    if(show_blur){
        flags |= ImGuiWindowFlags_NoInputs;
    }

    // Window - Character
    ImGui::Begin("Character", NULL, flags);
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        frameTimer -= ImGui::GetIO().DeltaTime;
        arrowTimer = frameTimer;

        characterPos = ImVec2((ImGui::GetContentRegionAvail() - ImVec2(32, 64)) * 0.5f) + ImVec2(8, 0) - ImVec2(0, 8);
        character.drawPos = characterPos;

        if(characterCreated && !overlapCharacter)
        {
            ImGui::SetCursorPos(characterPos);
            character.moveMainCharacter(&image, &charBuild, frameTimer, allowMovement, draw_list);
        }

        ImGui::SetCursorPos(ImVec2(0, 0));

        int keyDown = 0; // used to identify which direction the character is movin
        if(allowMovement){
            if(ImGui::IsKeyDown(ImGuiKey_W)) { keyDown = 1; }
            else if(ImGui::IsKeyDown(ImGuiKey_S)) { keyDown = 2; }
            else if(ImGui::IsKeyDown(ImGuiKey_D)) { keyDown = 3; }
            else if(ImGui::IsKeyDown(ImGuiKey_A)) { keyDown = 4; }
        }

        move.mapMovement(keyDown, overlap, gridX, gridY, move.getGrid().size(), move.getGrid()[0].size(), lastAction, interact);
        //cout << gridX << ", " << gridY << " and last action " << lastAction << endl;

        if(interact != 0){
            // ImGui::SetCursorPos(ImVec2((float)width_px/ 4 , (float)height_px / 4 ));
            if(interact == 1){
                switch (lastAction){
                    case 1:
                        ImGui::SetCursorPos(ImVec2((float)width_px/ 4 - 16, (float)height_px / 4 - 54));
                        break;
                    case 2:
                        ImGui::SetCursorPos(ImVec2((float)width_px/ 4 - 16, (float)height_px / 4 + 16));
                        break;

                    default:
                        break;
                }

                move.drawArrows(arrowTimer, lastAction);
                if (arrowTimer <= 0.f){
                    arrowTimer = frameLength;
                }
            }

            if(ImGui::IsKeyDown(ImGuiKey_Q) && show_blur == false){
                // cout << "trigger interaction here" << endl;
                if(interact == 1){
                    //update the visuals and locations of character

                    configReader config = configReader();
                    string newLocation = config.check("../assets/map/" + world + room + "config.toml", to_string((int)gridX) + "," + to_string((int)gridY));

                    if(newLocation.substr(0,1) == "+"){ //enter room
                        room = newLocation.substr(1) + "/";
                    }

                    if(newLocation.substr(0,1) == "-"){  //enter world
                        world = newLocation.substr(1) + "/";
                        room = "";
                    }

                    gridX = config.gridX;
                    gridY = config.gridY;

                    pathMap = "../assets/map/" + world + room + "map.png";
                    intMap = "../assets/map/" + world + room + "int.png";
                    obsMap = "../assets/map/" + world + room + "obs.png";
                    overlapMap = "../assets/map/" + world + room + "overlap.png";

                    loadMapUpdate(move);
                    character.setNpc("../assets/map/" + world + room + "config.toml");

                    interact = 0;
                    lastAction = 0;

                }
                else if(interact == 2)
                {
                    bool hasQuestion = false;
                    int question = 0;

                    auto npcRight = character.getNpc()->find(make_pair((int)gridX - 1, (int)gridY));
                    auto npcLeft = character.getNpc()->find(make_pair((int)gridX + 1, (int)gridY));
                    auto npcUp = character.getNpc()->find(make_pair((int)gridX, (int)gridY - 1));
                    auto npcDown = character.getNpc()->find(make_pair((int)gridX, (int)gridY + 1));

                    if(npcRight != character.getNpc()->end())
                    {
                        hasQuestion = npcRight->second.hasQuestion;
                        question = npcRight->second.question;
                    }
                    else if(npcLeft != character.getNpc()->end())
                    {
                        hasQuestion = npcLeft->second.hasQuestion;
                        question = npcLeft->second.question;
                    }
                    else if(npcUp != character.getNpc()->end())
                    {
                        hasQuestion = npcUp->second.hasQuestion;
                        question = npcUp->second.question;
                    }
                    else if(npcDown != character.getNpc()->end())
                    {
                        hasQuestion = npcDown->second.hasQuestion;
                        question = npcDown->second.question;
                    }

                    if(hasQuestion)
                    {
                        //do NPC text or whatever first hten do the rest
                        show_codeEditor = !show_codeEditor;
                        show_userProfile = !show_userProfile;

                    triggerQuestion(question);

                    editor.SetTextLines(qes.boiler);
                    cout << qes.boiler.size() << endl;

                        result = "";
                    }
                }                
            }

        }

        if (frameTimer <= 0.f)
        {
            frameTimer = frameLength;
        }

        ImGui::SetCursorPos(ImVec2(0, 0));

    }
    ImGui::End();
}

void graphic::triggerQuestion(int question){
    database &db = database::getInstance();
    db.getQuestion(question, qes);
}

void graphic::loadMapUpdate(movementHandler &move){
    background = imageHandler(pathMap.c_str());
    overlap = imageHandler(overlapMap.c_str());
    interactions = imageHandler(intMap.c_str());
    background.loadTexture(background.filepath, &background);
    overlap.loadTexture(overlap.filepath, &overlap);
    interactions.loadTexture(interactions.filepath, &interactions);

    move = movementHandler(obsMap, intMap, width_px, height_px);
}

void graphic::makeBackground(imageHandler background, vector<vector<int>> grid, double gridX, double gridY, imageHandler& image, characterManager &character, characterBuilder& charBuild, int &lastAction)
{
    ImGui::SetNextWindowSize({(float)width_px /2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 0});

    const float frameLength = 1.f / 10.f; // In seconds, so  FPS
    static float frameTimer = frameLength;
    static int frameCount_6 = 3;
    static int frameCount_8 = 0;

    characterConfig charConfig;

    //cout << world << room << " | " << character.getNpc()->size() << endl;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMouseInputs;
    if(show_blur){
        flags |= ImGuiWindowFlags_NoInputs;
    }

    ImGui::Begin("Background", NULL, flags);
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        frameTimer -= ImGui::GetIO().DeltaTime;

        int mapCols = grid[0].size();
        int mapRows = grid.size();

        ImGui::SetCursorPos(ImVec2(0,0));
        background.DrawMap(background, gridX, gridY, (width_px / 2), (height_px / 2), mapRows, mapCols);

        // calcluate what grids are on the edge of the screen
        float topX = gridX - (((float)width_px / 2.f) / 32.f) / 2.f;
        float topY = gridY - (((float)height_px / 2.f) / 32.f) / 2.f;

        int botX = (int)(gridX + (((float)width_px / 2.f) / 32.f)/ 2.f);
        int botY = (int)(gridY + (((float)height_px / 2.f) / 32.f)/ 2.f);

        for(auto iter : *character.getNpc())
        {
            bool npcContact = false;

            int charX = iter.second.cordX; // get character position
            int charY = iter.second.cordY - 1;
            if(charX >= (int)topX && charX <= botX && charY + 1 >= (int)topY && charY <= botY) // check if the npc should be on screen
            {
                float posX = (float)charX - topX; // calculate how many grids the npc needs to be from the top left
                float posY = (float)charY - topY;

                ImVec2 npcPos = ImVec2((float)posX * 32.f - 16.f,(float)posY * 32.f - 16.f); // multiply by 32 for grid size, subtract 16 to adjust the npc to the exact grid location

                //cout << gridX << "==" << charX << " "<< posX <<" | " << gridY << "==" << charY <<" "<< posY << endl;
                if(iter.second.direction == 0)
                {
                    if((int)gridY == charY && (int)gridX == charX) // if character above npc
                    {
                        draw_list->AddRectFilled(ImVec2(npcPos.x + 0.f, npcPos.y + 55.f), ImVec2(npcPos.x + 32.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                        if(characterCreated)
                        {
                            ImGui::SetCursorPos(characterPos);
                            character.moveMainCharacter(&image, &charBuild, frameTimer, allowMovement, draw_list);
                            overlapCharacter = true;
                        }
                        charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleUp.at(frameCount_6), 1.f, iter.second.character, 0.4f);
                        npcContact = true;
                    }
                    else if((int)gridY == charY + 2 && (int)gridX == charX) // if character is below npc
                    {
                        draw_list->AddRectFilled(ImVec2(npcPos.x + 0.f, npcPos.y + 55.f), ImVec2(npcPos.x + 32.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                        charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleDown.at(frameCount_6), 1.f, iter.second.character);
                        npcContact = true;
                        overlapCharacter = false;
                    }
                    else if((int)gridX == charX + 1 && (int)gridY == charY + 1) // if character is right of npc
                    {
                        draw_list->AddRectFilled(ImVec2(npcPos.x + 5.f, npcPos.y + 55.f), ImVec2(npcPos.x + 27.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                        charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleRight.at(frameCount_6), 1.f, iter.second.character);
                        npcContact = true;
                        overlapCharacter = false;
                    }
                    else if((int)gridX == charX - 1 && (int)gridY == charY + 1) //if character is left of npc
                    {
                        draw_list->AddRectFilled(ImVec2(npcPos.x + 5.f, npcPos.y + 55.f), ImVec2(npcPos.x + 27.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                        charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleLeft.at(frameCount_6), 1.f, iter.second.character);
                        npcContact = true;
                        overlapCharacter = false;
                    }
                    else
                    {
                        draw_list->AddRectFilled(ImVec2(npcPos.x + 0.f, npcPos.y + 55.f), ImVec2(npcPos.x + 32.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                        charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleDown.at(frameCount_6), 1.f, iter.second.character);
                        overlapCharacter = false;
                    }
                }
                else
                {
                    switch(iter.second.direction)
                    {
                        case 1: // facing up
                            draw_list->AddRectFilled(ImVec2(npcPos.x + 0.f, npcPos.y + 55.f), ImVec2(npcPos.x + 32.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                            if(characterCreated && ((int)gridY == charY && (int)gridX == charX))
                            {
                                ImGui::SetCursorPos(characterPos);
                                character.moveMainCharacter(&image, &charBuild, frameTimer, allowMovement, draw_list);
                                overlapCharacter = true;
                                charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleUp.at(frameCount_6), 1.f, iter.second.character, 0.4f);
                            }
                            else
                            {
                                charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleUp.at(frameCount_6), 1.f, iter.second.character);
                                overlapCharacter = false;
                            }
                            break;
                        case 2: // facing down
                            draw_list->AddRectFilled(ImVec2(npcPos.x + 0.f, npcPos.y + 55.f), ImVec2(npcPos.x + 32.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                            if(characterCreated && ((int)gridY == charY && (int)gridX == charX))
                            {
                                ImGui::SetCursorPos(characterPos);
                                character.moveMainCharacter(&image, &charBuild, frameTimer, allowMovement, draw_list);
                                overlapCharacter = true;
                                charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleDown.at(frameCount_6), 1.f, iter.second.character, 0.4f);
                            }
                            else
                            {
                                charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleDown.at(frameCount_6), 1.f, iter.second.character);
                                overlapCharacter = false;
                            }
                            break;
                        case 3: // facing right
                            draw_list->AddRectFilled(ImVec2(npcPos.x + 5.f, npcPos.y + 55.f), ImVec2(npcPos.x + 27.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                            if(characterCreated && ((int)gridY == charY && (int)gridX == charX))
                            {
                                ImGui::SetCursorPos(characterPos);
                                character.moveMainCharacter(&image, &charBuild, frameTimer, allowMovement, draw_list);
                                overlapCharacter = true;
                                charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleRight.at(frameCount_6), 1.f, iter.second.character, 0.4f);
                            }
                            else
                            {
                                charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleRight.at(frameCount_6), 1.f, iter.second.character);
                                overlapCharacter = false;
                            }
                            break;
                        case 4: // facing left
                            draw_list->AddRectFilled(ImVec2(npcPos.x + 5.f, npcPos.y + 55.f), ImVec2(npcPos.x + 27.f, npcPos.y + 64.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)), 20.f);
                            if(characterCreated && ((int)gridY == charY && (int)gridX == charX))
                            {
                                ImGui::SetCursorPos(characterPos);
                                character.moveMainCharacter(&image, &charBuild, frameTimer, allowMovement, draw_list);
                                overlapCharacter = true;
                                charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleLeft.at(frameCount_6), 1.f, iter.second.character, 0.4f);
                            }
                            else
                            {
                                charBuild.drawCharacterAnimation(&image, npcPos, charConfig.cordsIdleLeft.at(frameCount_6), 1.f, iter.second.character);
                                overlapCharacter = false;
                            }
                            break;
                    }
                }

                if(interact == 2 && iter.second.hasQuestion && npcContact) // draw buble
                {
                    switch(lastAction)
                    {
                        case 1:
                            ImGui::SetCursorPos(ImVec2(npcPos.x + 24.f, npcPos.y + 5.f));
                            character.drawTalkBubble(frameCount_8, 1.f);
                            break;
                        case 2:
                            ImGui::SetCursorPos(ImVec2(npcPos.x + 24.f, npcPos.y + 5.f));
                            character.drawTalkBubble(frameCount_8, 1.f);
                            break;
                        case 3:
                            ImGui::SetCursorPos(ImVec2(npcPos.x + 15.f, npcPos.y - 24.f));
                            character.drawTalkBubble(frameCount_8, -1.f);
                            break;
                        case 4:
                            ImGui::SetCursorPos(ImVec2(npcPos.x + 17.f, npcPos.y - 23.f));
                            character.drawTalkBubble(frameCount_8, 1.f);
                            break;
                    }
                }
            }
        }
        //cout << "Map: " << grid[gridY][gridX] << " | Interact: " << interact <<  " | X:" << gridX << " TOPX:" << topX <<" BOTX:" << botX <<" Y:" << gridY<< " TOPY:" << topY <<" BOTY:" << botY<< endl;

        if (frameTimer <= 0.f)
        {
            frameTimer = frameLength;
            frameCount_6 ++;
            frameCount_8 ++;
            if(frameCount_6 % 6 == 0)
            {
                frameCount_6 = 0;
            }
            if(frameCount_8 % 8 == 0)
            {
                frameCount_8 = 0;
            }
        }
    }

    ImGui::End();
}


void graphic::makeCodeEditor(TextEditor &editor, const char* fileToEdit, ImFont* font_15, ImFont* font_18, ImFont* font_21){
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

        // TODO - implement code for all questions
        ImGui::TextWrapped("%s", qes.question.c_str());
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
        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? " " : " ",
			editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

        if(!show_blur){
            editor.Render("TextEditor");
        }
		
    }
    ImGui::End();
}

void graphic::makeUserProfile(ImFont* fontLarge, ImFont* fontSmall, ImFont* fontLogo, imageHandler& image, characterManager& character, characterBuilder& charBuild)
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
    float profileHeight = 0.f;

    ImGui::Begin("Profile", NULL, flags);
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        userProfile& usrProfile = userProfile::getInstance();

        // ----- Colors ----- //
        // -- Card -- //
        ImU32 col_card_top_left = ImGui::GetColorU32(IM_COL32(140, 200, 255, 255));
        ImU32 col_card_top_right = ImGui::GetColorU32(IM_COL32(20, 100, 180, 255));
        ImU32 col_card_bottom_left = ImGui::GetColorU32(IM_COL32(120, 180, 255, 255));
        ImU32 col_card_bottom_right = ImGui::GetColorU32(IM_COL32(0, 128, 128, 255));
        // -- Banner -- //
        ImU32 col_banner_top = ImGui::GetColorU32(IM_COL32(30, 90, 160, 255));
        ImU32 col_banner_bottom = ImGui::GetColorU32(IM_COL32(10, 30, 80, 255));
        // -- Image Border -- //
        ImU32 col_silver = ImGui::GetColorU32(IM_COL32(192, 192, 192, 255));
        ImU32 col_pastel_silver = ImGui::GetColorU32(IM_COL32(230, 230, 230, 255));
        // -- Character Background --//
        // background behind character
        ImU32 col_sky_top = ImGui::GetColorU32(IM_COL32(190, 220, 255, 255));
        ImU32 col_sky_bottom = ImGui::GetColorU32(IM_COL32(100, 180, 255, 255));
        ImU32 col_grass_top = ImGui::GetColorU32(IM_COL32(120, 180, 120, 255));
        ImU32 col_grass_bottom = ImGui::GetColorU32(IM_COL32(160, 220, 160, 255));

        // outer border
        draw_list->AddRectFilled(ImVec2(profileWidth - 210.f, profileHeight + 30.f), ImVec2(profileWidth + 210.f, profileHeight + 300.f), ImColor(ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), 20.f);
        // color gradient fill
        draw_list->AddRectFilledMultiColorRound(ImVec2(profileWidth - 205.f, profileHeight + 35.f), ImVec2(profileWidth + 205.f, profileHeight + 295.f), col_card_top_left, col_card_top_right, col_card_bottom_right, col_card_bottom_left, 20.f, ImDrawFlags_None);
        // banner gradient fill
        draw_list->AddRectFilledMultiColorRound(ImVec2(profileWidth - 200.f, profileHeight + 40.f), ImVec2(profileWidth + 200.f, profileHeight + 65.f), col_banner_top, col_banner_top, col_banner_bottom, col_banner_bottom, 15.f, ImDrawFlags_None);

        // shadow below character box
        draw_list->AddRectFilled(ImVec2(profileWidth + 85.f, profileHeight + 140.f), ImVec2(profileWidth + 197.f, profileHeight + 290.f), ImColor(ImVec4(0.0f, 0.0f, 0.0f, 0.2f)), 10.f);
        // character box border
        draw_list->AddRectFilledMultiColorRound(ImVec2(profileWidth + 85.f, profileHeight + 140.f), ImVec2(profileWidth + 190.f, profileHeight + 285.f), col_silver, col_pastel_silver, col_silver, col_pastel_silver, 5.f, ImDrawFlags_None);
        // character background
        draw_list->AddRectFilledMultiColor(ImVec2(profileWidth + 90.f, profileHeight + 145.f), ImVec2(profileWidth + 185.f, profileHeight + 280.f), col_sky_top, col_sky_bottom, col_grass_top, col_grass_bottom); // background behind character
        charBuild.drawCharacterAnimation(&image, ImVec2((windowWidth / 2.f) + 108.7f, 154.9f), {ImVec2(64.1f / 192.f, 0.1f/512.f),ImVec2(95.99f/192.f, 64.f/512.f)}, 1.8f, character.getMainPlayer()->dynamicIndex);

        draw_list->AddRectFilled(ImVec2(profileWidth - 195.f, profileHeight + 75.f), ImVec2(profileWidth + 195.f, profileHeight + 100.f), IM_COL32(255, 255, 255, 100), 10.f); // name background

        draw_list->AddRectFilled(ImVec2(profileWidth - 195.f, profileHeight + 112.f), ImVec2(profileWidth + 195.f, profileHeight + 137.f), IM_COL32(255, 255, 255, 100), 10.f); // ID background
        draw_list->AddRectFilled(ImVec2(profileWidth - 195.f, profileHeight + 149.f), ImVec2(profileWidth + 75.f, profileHeight + 174.f), IM_COL32(255, 255, 255, 100), 10.f); // name background
        draw_list->AddRectFilled(ImVec2(profileWidth - 195.f, profileHeight + 186.f), ImVec2(profileWidth + 75.f, profileHeight + 211.f), IM_COL32(255, 255, 255, 100), 10.f); // name background

        draw_list->AddRectFilled(ImVec2(profileWidth - 195.f, profileHeight + 223.f), ImVec2(profileWidth + 75.f, profileHeight + 248.f), IM_COL32(255, 255, 255, 100), 10.f); // name background
        draw_list->AddRectFilled(ImVec2(profileWidth - 195.f, profileHeight + 260.f), ImVec2(profileWidth + 75.f, profileHeight + 285.f), IM_COL32(255, 255, 255, 100), 10.f); // name background

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 190.f, 48.f));
        ImGui::Text(ICON_FA_ANCHOR);

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 40.f, 48.f));
        ImGui::Text(ICON_FA_ANCHOR);

        // --- Wrote Test with Stylized Logo Font --- //
        ImGui::PushFont(fontLogo);

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 165.f, 40.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
        ImGui::Text("%s", "Harbor Card");
        ImGui::PopStyleColor();

        ImGui::PopFont();

        // --- Write Text With Large Font --- //
        ImGui::PushFont(fontLarge);

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 190.f, 73.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text("%s", "Name: ");
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 190.f, 110.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text("%s", "ID #: ");
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 190.f, 147.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text("%s", "Problems Solved:");
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 190.f, 184.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text("%s", "Money:");
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 190.f, 221.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text("%s", "Member Since: ");
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) - 190.f, 258.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
        ImGui::Text("%s", "Current Streak: ");
        ImGui::PopStyleColor();

        ImGui::PopFont();

        // --- Write Text With Small Font --- //
        ImGui::PushFont(fontSmall);

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 190.f - ImGui::CalcTextSize(usrProfile.getUsername().c_str()).x, 76.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,60,70,255));
        ImGui::Text("%s", usrProfile.getUsername().c_str());
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 190.f - ImGui::CalcTextSize(usrProfile.getUserId().c_str()).x, 113.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,60,70,255));
        ImGui::Text("%s", usrProfile.getUserId().c_str());
        ImGui::PopStyleColor();

        // TODO - connect to number of problems solved
        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 70.f - ImGui::CalcTextSize("0").x, 150.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,60,70,255));
        ImGui::Text("%s", "0");
        ImGui::PopStyleColor();

        // TODO - need to add money
        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 70.f - ImGui::CalcTextSize("5543").x, 187.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,60,70,255));
        ImGui::Text("%s", "5543");
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 70.f - ImGui::CalcTextSize(usrProfile.getJoinDate().c_str()).x, 224.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,60,70,255));
        ImGui::Text("%s", usrProfile.getJoinDate().c_str());
        ImGui::PopStyleColor();

        //TODO - need to implement streak counter
        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 70.f - ImGui::CalcTextSize("0").x, 261.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,60,70,255));
        ImGui::Text("%s", "0");
        ImGui::PopStyleColor();

        ImGui::PopFont();

        ImGui::SetCursorPos(ImVec2((windowWidth / 2.f) + 70.f - ImGui::CalcTextSize("5543").x - 30.f, 194.f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,60,70,255));
        ImGui::Text(ICON_FA_SAILBOAT);
        ImGui::PopStyleColor();

    }
    ImGui::End();

}

void graphic::makeConfig(ImFont* font){
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
        ImGui::Text("%f", ImGui::GetIO().DeltaTime);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        static int index = 0;
        static bool disabled = false;
        int worldChange = 4;

        vector<string> tutorial = {"Welcome to Hash Harbor!",
                                   "To navigate your house, use the following keys: \nW (up), A (left), S (down), D (right)",
                                   "Access the pause menu anytime by pressing 'ESC'.\n Use it to jot down notes, customize your character, and adjust settings",
                                   "When you're ready to venture outside, head to the bottom of the house an arrow will appear when you're at the exit of the house",
                                   "Press 'Q' to exit the house and embark on your adventure",
                                   "Interact with characters and buildings in the world by pressing 'Q' when facing them",
                                   "As you explore the world, keep an eye out for characters wearing chefs hats. They often have coding problems for you to solve",
                                   "Best of luck on your journey!"};

        if(index < tutorial.size() && playTutorial)
        {
            draw_list->AddRectFilled(ImVec2(((float)width_px / 4.f) - 250.f, ((float)height_px / 2.f) + 50.f), ImVec2(((float)width_px / 4.f) + 250.f, ((float)height_px / 2.f) + 200.f), ImColor(ImVec4(0.65f, 0.65f, 0.65f, 1.0f)), 20.f);
            draw_list->AddRectFilled(ImVec2(((float)width_px / 4.f) - 247.f, ((float)height_px / 2.f) + 53.f), ImVec2(((float)width_px / 4.f) + 247.f, ((float)height_px / 2.f) + 197.f), ImColor(ImVec4(0.1f, 0.1f, 0.1f, 1.0f)), 20.f);

            if (disabled)
            {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }

            ImGui::PushFont(font);
            ImGui::SetCursorPos(ImVec2((ImGui::GetContentRegionAvail() / 2.f).x + 140.f, 160.f));
            ImGui::PushID(919191);
            if(disabled && index == (worldChange - 1))
            {
                ImGui::Button("Exit House", ImVec2(100.f, 30.f));
            }
            else if(disabled && index == worldChange)
            {
                ImGui::Button("Press \'Q\'", ImVec2(100.f, 30.f));
            }
            else
            {
                if(ImGui::Button("Continue ->", ImVec2(100.f, 30.f)))
                {
                    if(index < worldChange - 1)
                    {
                        index ++;
                    }
                    else if(index >= worldChange - 1 && world == "town1/" && room.empty())
                    {
                        index ++;
                    }
                }
            }
            ImGui::PopID();
            if (disabled)
            {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
            //cout << index << " | " << interact<< endl;

            if(interact == 1 && index == (worldChange - 1))
            {
                index = 4;
            }

            if(index == (worldChange - 1) && world == "town1/" && !room.empty())
            {
                disabled = true;
            }
            else if(world == "town1/" && room.empty() && disabled)
            {
                index = 5;
                disabled = false;
            }

            if(index < tutorial.size())
            {
                ImGui::SetCursorPos(ImVec2((ImGui::GetContentRegionAvail() / 2.f).x - 220.f, 85.f));
                ImGui::PushTextWrapPos(580.f);
                if(index >= worldChange && world == "town1/" && room == "")
                {
                    ImGui::TextWrapped("%s", tutorial.at(index).c_str());
                }
                else
                {
                    ImGui::TextWrapped("%s", tutorial.at(index).c_str());
                }
                ImGui::PopTextWrapPos();
            }
            ImGui::PopFont();
        }

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
    string setup = "#include <bits/stdc++.h> \n";
    setup += "using namespace std; \n";

    code = setup + code + qes.exeCode;

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
        playTutorial = false;

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

    Pause.drawPauseMenu(&image, &character, &charBuild, &changeScreenRes, &res, &updateCharacter, &reset, &done, &codeEditorFont);

    if(updateCharacter)
    {
        database& db = database::getInstance();
        character.selectMainCharacter(&charBuild);
        db.updateCharacter();
        show_settings = !show_settings;
        show_blur = !show_blur;

        resetPauseScreen = true;
        updateCharacter = false;
    }
    style.FrameRounding = 0.f;
}