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

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

//#include <bits/stdc++.h>

#include "graphic.h"
#include "ImageHandler.h"
#include "CharacterManager.h"

#include <iostream>
using std::cout;
using std::endl;

void graphic::setup(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
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

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Create Characters
    characterManager.createCharacter("Bob", false, true, &imageHandler);
    characterManager.setMainPlayer("Bob");
    

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
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                io.DisplaySize.x = static_cast<float>(event.window.data1);
                io.DisplaySize.y = static_cast<float>(event.window.data2);
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if(show_MainCharacter)
        {
            makeMainCharacter();
        }
        /*
        if(show_display){
            makeDisplay();
        }
        
        if(show_process){
            makeProcess(); 
        }

        if(show_config){
            makeConfig();
        }
         */

        // if(emulate){
        //     while(chip.pc < sizeof(chip.memory)){
        //         chip.emulate_cycle();
        //         std::this_thread::sleep_for(std::chrono::seconds(1));
        //     }
        // }
        
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void graphic::makeDisplay(){
    // Graphics window calculation
    ImGui::SetNextWindowSize({(float)width_px /2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 0});

    // Window - Graphics
    ImGui::Begin("Graphics", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    {
        
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

void graphic::makeConfig(){
    // Config window calculation
    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 320});

    // Window - Config
    ImGui::Begin("Config", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
    }
    ImGui::End();

        
}

void graphic::makeMainCharacter()
{
    // The window below is the screen where the player is drawn
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
    window_flags |= ImGuiWindowFlags_NoScrollbar;

    //Used to control frame rate of animations
    const float frameLength = 2.5f / 10.f; // In seconds, so 4 FPS
    static float frameTimer = frameLength;

    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("Window Name", &show_MainCharacter,window_flags);

    frameTimer -= ImGui::GetIO().DeltaTime;

    ImGui::SetCursorPos(ImGui::GetCursorPos() + (ImGui::GetContentRegionAvail() - ImVec2(32, 64)) * 0.5f);
    characterManager.moveMainCharacter(&imageHandler, frameTimer);

    ImGui::Text("Window SIZE ");
    ImGui::Text("\"%f\" %f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

    if (frameTimer <= 0.f)
    {
        frameTimer = frameLength;
    }
    ImGui::End();
    ImGui::PopStyleVar();


}