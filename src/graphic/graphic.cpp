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
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
#endif

#include "graphic.h"
#include "../imageHandler/imageHandler.h"
#include "../character/characterManager.h"
#include "../character/characterBuilder.h"
#include "../movement/movementHandler.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

void graphic::setup(){
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


    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width_px, this->height_px, window_flags);
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

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    imageHandler image = imageHandler();

    characterManager character = characterManager();
    characterBuilder builder = characterBuilder(&image);

    imageHandler background = imageHandler("../src/abc.png");
    background.loadTexture(background.filepath, &background);

    character.createCharacter("Bob", false, true, &image);
    character.setMainPlayer("Bob");

    movementHandler move = movementHandler("../src/abc.png");
    // auto gr = obs.getGrid();
    // for(uint i = 0; i < gr.size(); i++){
    //     for(uint j = 0; j < gr[0].size(); j++){
    //         cout << gr[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    float mapGridX = 15.0f;
    float mapGridY = 8.0f;
    
    // Main loop
    bool done = false;
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


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if(show_display){
            makeBackground(background, move, mapGridX, mapGridY);
            makeDisplay(image, character, builder);
        }
        
        if(show_process){
            makeProcess(); 
        }

        if(show_config){
            makeConfig();
        }

        if(show_charSelector)
        {
            makeCharacterSelector(image, character, builder);
        }

        if(ImGui::IsKeyDown(ImGuiKey_Escape))
        {
            //todo move settings menu when created
            show_charSelector = true;
            characterCreated = false;
        }


        // if(emulate){
        //     while(chip.pc < sizeof(chip.memory)){
        //         chip.emulate_cycle();
        //         std::this_thread::sleep_for(std::chrono::seconds(1));
        //     }
        // }
        
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


void graphic::makeDisplay(imageHandler& image, characterManager &character, characterBuilder& charBuild)
{
    // Graphics window calculation
    ImGui::SetNextWindowSize({(float)width_px /2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 0});

    const float frameLength = 1.f / 10.f; // In seconds, so  FPS
    static float frameTimer = frameLength;

    // Window - Graphics
    ImGui::Begin("Graphics", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
    {
        if(characterCreated)
        {
            frameTimer -= ImGui::GetIO().DeltaTime;

            ImVec2 characterPos = ImVec2((ImGui::GetContentRegionAvail() - ImVec2(32, 64)) * 0.5f);
            character.drawPos = characterPos;

            ImGui::SetCursorPos(characterPos);
            character.moveMainCharacter(&image, &charBuild, frameTimer);

            if (frameTimer <= 0.f)
            {
                frameTimer = 2.5f / 10.f;
            }
        }
    }
    ImGui::End();
}

void graphic::makeProcess(){
    // Processor information window calculation
    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px});
    ImGui::SetNextWindowPos({(float)width_px / 2, 0});

    // Window - Processor Information
    ImGui::Begin("Processor Information", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
 
    }
    ImGui::End();
}

void graphic::makeCharacterSelector(imageHandler& image, characterManager &character, characterBuilder& charBuild)
{
    ImGui::SetNextWindowSize({ImGui::GetIO().DisplaySize.x-550.f, ImGui::GetIO().DisplaySize.y-200.f});
    ImGui::SetNextWindowPos({275.f,100.f});

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
        ImGui::SetCursorPos(ImVec2(290.f,390.f));
        ImGui::PushID(8);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(219.f / 360.f, 0.289f, 0.475f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(211.f / 360.f, 0.346f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(228.f / 360.f, 0.153f, 0.384f));
        if(ImGui::Button("Select Character", ImVec2(150.f, 40.f)))
        {
            character.selectMainCharacter(&charBuild);
            characterCreated = true;
            show_charSelector = false;
        }
        ImGui::PopStyleColor(3);
        
        ImGui::PopID();
    }
    ImGui::End();
}


void graphic::makeBackground(imageHandler background, movementHandler move, float &gridX, float &gridY){
    ImGui::SetNextWindowSize({(float)width_px /2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 0});

    static auto lastKeyEventTime = std::chrono::steady_clock::now();

    // Get the current time
    auto currentTime = std::chrono::steady_clock::now();

    // Calculate the time elapsed since the last key event
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastKeyEventTime).count();

    // Define the cooldown duration between key events in milliseconds
    const int cooldownMilliseconds = 1000; // 1 second cooldown


    #ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
    struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
            const ImGuiKey key_first = ImGuiKey_NamedKey_BEGIN;
    #else
        struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } };
        const ImGuiKey key_first = 0;
    #endif

    int keyDown = 0; // used to identify which direction the character is moving
    for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++)
    {
        if (elapsedTime >= cooldownMilliseconds) {
            if (funcs::IsLegacyNativeDupe(key)) continue;

            if(ImGui::IsKeyDown(ImGuiKey_UpArrow) || ImGui::IsKeyDown(ImGuiKey_W)) { keyDown = 1; }
            else if(ImGui::IsKeyDown(ImGuiKey_DownArrow) || ImGui::IsKeyDown(ImGuiKey_S)) { keyDown = 2; }
            else if(ImGui::IsKeyDown(ImGuiKey_RightArrow) || ImGui::IsKeyDown(ImGuiKey_D)) { keyDown = 3; }
            else if(ImGui::IsKeyDown(ImGuiKey_LeftArrow) || ImGui::IsKeyDown(ImGuiKey_A)) { keyDown = 4; }
            
        }
    }

    ImGui::Begin("Background", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
    {
        move.mapMovement(keyDown, background, gridX, gridY);
    }

    ImGui::End();
}


void graphic::makeConfig(){
    // Config window calculation
    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px / 2});

    ImGui::SetNextWindowPos({0, (float)height_px/2});

    // Window - Config
    ImGui::Begin("Config", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
    }
    ImGui::End();

        
}