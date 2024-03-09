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
#include "../character/characterManager.h"
#include "../character/characterBuilder.h"
#include "../movement/movementHandler.h"
#include "../assets/font/IconsFontAwesome6.h"
#include "../login/login.h"
#include "database/database.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

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

void graphic::setup(){
        TextEditor editor;
	auto lang = TextEditor::LanguageDefinition::CPlusPlus();

	// set your own known preprocessor symbols...
	static const char* ppnames[] = { "NULL", "PM_REMOVE",
		"ZeroMemory", "DXGI_SWAP_EFFECT_DISCARD", "D3D_FEATURE_LEVEL", "D3D_DRIVER_TYPE_HARDWARE", "WINAPI","D3D11_SDK_VERSION", "assert" };
	// ... and their corresponding values
	static const char* ppvalues[] = { 
		"#define NULL ((void*)0)", 
		"#define PM_REMOVE (0x0001)",
		"Microsoft's own memory zapper function\n(which is a macro actually)\nvoid ZeroMemory(\n\t[in] PVOID  Destination,\n\t[in] SIZE_T Length\n); ", 
		"enum DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD = 0", 
		"enum D3D_FEATURE_LEVEL", 
		"enum D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE  = ( D3D_DRIVER_TYPE_UNKNOWN + 1 )",
		"#define WINAPI __stdcall",
		"#define D3D11_SDK_VERSION (7)",
		" #define assert(expression) (void)(                                                  \n"
        "    (!!(expression)) ||                                                              \n"
        "    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \n"
        " )"
		};

	for (long unsigned int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = ppvalues[i];
		lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
	}

	// set your own identifiers
	static const char* identifiers[] = {
		"HWND", "HRESULT", "LPRESULT","D3D11_RENDER_TARGET_VIEW_DESC", "DXGI_SWAP_CHAIN_DESC","MSG","LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
		"ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
		"ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
		"IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "TextEditor" };
	static const char* idecls[] = 
	{
		"typedef HWND_* HWND", "typedef long HRESULT", "typedef long* LPRESULT", "struct D3D11_RENDER_TARGET_VIEW_DESC", "struct DXGI_SWAP_CHAIN_DESC",
		"typedef tagMSG MSG\n * Message structure","typedef LONG_PTR LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
		"ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
		"ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
		"IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "class TextEditor" };
	for (long unsigned int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = std::string(idecls[i]);
		lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
	}
	editor.SetLanguageDefinition(lang);
	//editor.SetPalette(TextEditor::GetLightPalette());

	// error markers
	TextEditor::ErrorMarkers markers;
	markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
	markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
	editor.SetErrorMarkers(markers);

    vector<string> cppStart;
    cppStart.push_back("#include <iostream>");
    cppStart.push_back("int main() {");
    cppStart.push_back("\tstd::cout << \"Hello World!\";");
    cppStart.push_back("\treturn 0;");
    cppStart.push_back("}");
    editor.SetTextLines(cppStart);

    static const char* fileToEdit = "frog.cpp";

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

    // Font Icon set up
    io.Fonts->AddFontDefault();
    float baseFontSize = 25.0f; // 13.0f is the size of the default font. Change to the font size you use.
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAR, iconFontSize, &icons_config, icons_ranges );
    io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges );


    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    imageHandler image = imageHandler();
    database& db = database::getInstance();
    //db.connect();
    characterManager character = characterManager();
    characterBuilder builder = characterBuilder(&image);

    string pathMap;
#if defined(__APPLE__)
    pathMap = "/Users/david/CLionProjects/HashHarbor/src/abc.png";
#else
    pathMap = "../src/abc.png";
#endif
    imageHandler background = imageHandler(pathMap.c_str());
    background.loadTexture(background.filepath, &background);

    character.createCharacter("Bob", false, true, &image);
    character.setMainPlayer("Bob");

    movementHandler move = movementHandler(pathMap);
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

        if(show_login)
        {
            makeLogIn();
        }

        if(show_display){
            makeBackground(background, move, mapGridX, mapGridY);
            makeDisplay(image, character, builder);
        }
        
        if(show_process){
            makeProcess(editor, fileToEdit); 
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

string result = "";

void graphic::makeProcess(TextEditor &editor, const char* fileToEdit){
    // Processor information window calculation
    ImGui::SetNextWindowSize({(float)width_px / 2,(float)height_px / 4 * 3});
    ImGui::SetNextWindowPos({(float)width_px / 2, 0});

    auto cpos = editor.GetCursorPosition();
    editor.GetText();

    ImGui::Begin("Sandbox", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
    {
        // if (ImGui::BeginMenu("File")) {
        //     if (ImGui::MenuItem("New", "Ctrl+N")) {
        //         // Handle New action
        //     }
        //     if (ImGui::MenuItem("Open", "Ctrl+O")) {
        //         // Handle Open action
        //     }
        //     if (ImGui::MenuItem("Save", "Ctrl+S")) {
        //         // Handle Save action
        //     }
        //     ImGui::Separator();
        //     if (ImGui::MenuItem("Exit", "Alt+F4")) {
        //         // Handle Exit action
        //     }
        //     ImGui::EndMenu();
        // }

        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

		editor.Render("TextEditor");
    }
    ImGui::End();

    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px / 4});
    ImGui::SetNextWindowPos({(float)width_px / 2, (float)height_px / 4 * 3});

    // Window - Processor Information
    ImGui::Begin("Submission details", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        if(ImGui::Button("Run Code")){
            string textToSave = editor.GetText();

            result = executeCPP(textToSave);
            cout << result << endl;
        }

        ImGui::InputTextMultiline("Result", const_cast<char*>(result.c_str()), result.size() + 1, ImVec2(500, 200), ImGuiInputTextFlags_ReadOnly);
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

string graphic::executeCPP(string code){
    // Step 1: Write code to a temporary file
    std::ofstream file("temp.cpp");
    file << code;
    file.close();

        // Step 2: Invoke the compiler and capture output
    std::string compileCommand = "clang++ -o temp temp.cpp 2>&1"; // Redirect stderr to stdout
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

void graphic::makeLogIn()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 7.5f;
    style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    ImGui::SetNextWindowSize({(float)width_px, (float)height_px});
    ImGui::SetNextWindowPos({0, 0});

    // coordinates for the top left corner
    static float minWidth = (width_px / 2.f) - 200.f;
    static float minHeight = (height_px / 2.f) - 240.f;
    static float txtPos_x = 0.f;

    static login Login = login();

    static char username[64] = "";
    static char passwd[64] = "";

    static char createUsername[64] = "";
    static char createPasswd[64] = "";
    static char confirmPasswd[64] = "";

    static bool createAccount = false;
    static bool viewPasswd = false; // keep password hidden by default
    static bool errorAuth = false;
    static bool errorCmp = false;
    static bool errorCreate = false;

    ImGui::Begin("Log In", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(ImVec2(minWidth, minHeight), ImVec2(minWidth + 400.f, minHeight + 500.f), ImColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 20.0f);

        if(!createAccount)
        {
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

            ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 200.f));
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
            ImGui::SetCursorPos(ImVec2(txtPos_x - 50.f, minHeight + 320.f));
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

            ImGui::SetCursorPos(ImVec2((width_px / 2.f - 50.f),minHeight + 380.f));
            ImGui::PushID(1);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(200.f / 360.f,1.0f,1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(200.f / 360.f,0.8f,1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(200.f / 360.f,1.0f,0.81f));
            if(ImGui::Button("Login", ImVec2(100.f,50.f)))
            {
                if(Login.inputValidation( username, passwd, true)) // this will validate the input and then authenticate the user
                {
                    show_display = true;
                    show_process = true;
                    show_config = true;
                    show_charSelector = true;

                    style.FrameRounding = 0.f;
                    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

                    show_login = false;
                }
                else
                {
                    errorAuth = true;
                }
            }

            if(errorAuth)
            {
                draw_list->AddRectFilled(ImVec2(minWidth + 30.f, minHeight + 235.f), ImVec2(minWidth + 370.f, minHeight + 290.f), ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 20.0f);
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,0,255));
                ImGui::SetCursorPos(ImVec2(minWidth + 40.f, minHeight + 244.f));
                ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);
                txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Oops! Something went wrong.").x) / 2.f);
                ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 242.f));
                ImGui::Text("Oops! Something went wrong.");
                txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Please double-check your username and password.").x) / 2.f);
                ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 265.f));
                ImGui::Text("Please double-check your username and password.");
                ImGui::PopStyleColor();
            }
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        else
        {
            ImGui::SetCursorPos(ImVec2(minWidth + 20.f, minHeight + 20.f));
            ImGui::PushID(7);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
            if(ImGui::Button(ICON_FA_CHEVRON_LEFT, ImVec2(23.f,18.f)))
            {
                createAccount = false;
                viewPasswd = false; // turn off both the new account and viewing passwd
            }
            ImGui::PopStyleColor(1);
            ImGui::PopID();

            txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Create Your Hash Harbor Account").x) / 2.f);
            ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 30.f));
            ImGui::Text("Create Your Hash Harbor Account");

            txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Username").x) / 2.f);
            ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 120.f));
            ImGui::Text("Username");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
            ImGui::SameLine(); HelpMarker("A username should be between 3 and 40 character\nA Username may contain:\n  Lowercase letters: a-z\n  Uppercase letters: A-Z\n  Digits: 0-9\n  Special character: - _ .");
            ImGui::PopStyleColor();

            ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 140.f));
            ImGui::PushItemWidth(300);
            ImGui::InputText(" ",createUsername, IM_ARRAYSIZE(createUsername), ImGuiInputTextFlags_None);
            ImGui::PopItemWidth();

            txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Password").x) / 2.f);
            ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 180.f));
            ImGui::Text("Password");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
            ImGui::SameLine(); HelpMarker("Password must be at least 8 characters\nPassword must be no more than 40 character\nPassword MUST contain at least one from each group below:\n  lowercase letter: a-z\n  Uppercase letter: A-Z\n  Digit: 0-9\n  Special character: ~ ` ! @ # $ % ^ & * ( ) _ - + = \n                   { } : ; < > ?");
            ImGui::PopStyleColor();

            ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 200.f));
            ImGui::PushItemWidth(300);
            if(!viewPasswd)
            {
                ImGui::InputText("  ",createPasswd, IM_ARRAYSIZE(createPasswd), ImGuiInputTextFlags_Password);
                ImGui::PopItemWidth();

                ImGui::SetCursorPos(ImVec2(minWidth + 350.f, minHeight + 201.f));
                ImGui::PushID(5);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
                if(ImGui::Button(ICON_FA_EYE_SLASH, ImVec2(30.f,16.f)))
                {
                    viewPasswd = true;
                }
                ImGui::PopStyleColor(1);
                ImGui::PopID();

                txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Confirm Password").x) / 2.f);
                ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 240.f));
                ImGui::Text("Confirm Password");

                ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 260.f));
                ImGui::PushItemWidth(300);
                ImGui::InputText("   ",confirmPasswd, IM_ARRAYSIZE(confirmPasswd), ImGuiInputTextFlags_Password);
                ImGui::PopItemWidth();
            }
            else
            {
                ImGui::InputText("  ",createPasswd, IM_ARRAYSIZE(createPasswd), ImGuiInputTextFlags_None);
                ImGui::PopItemWidth();

                ImGui::SetCursorPos(ImVec2(minWidth + 350.f, minHeight + 201.f));
                ImGui::PushID(6);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 360.f,0.0f,1.0f));
                if(ImGui::Button(ICON_FA_EYE, ImVec2(30.f,16.f)))
                {
                    viewPasswd = false;
                }
                ImGui::PopStyleColor(1);
                ImGui::PopID();

                txtPos_x = (width_px / 2.f) - ((ImGui::CalcTextSize("Confirm Password").x) / 2.f);
                ImGui::SetCursorPos(ImVec2(txtPos_x, minHeight + 240.f));
                ImGui::Text("Confirm Password");

                ImGui::SetCursorPos(ImVec2(minWidth + 50.f, minHeight + 260.f));
                ImGui::PushItemWidth(300);
                ImGui::InputText("   ",confirmPasswd, IM_ARRAYSIZE(confirmPasswd), ImGuiInputTextFlags_None);
                ImGui::PopItemWidth();
            }

            ImGui::SetCursorPos(ImVec2((width_px / 2.f - 50.f),minHeight + 380.f));
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
                else if(!Login.inputValidation( createUsername, createPasswd, false)) // if account passes input validation, hashing and adding to database
                {
                    errorCreate = true;
                }
                else
                {
                    show_display = true;
                    show_process = true;
                    show_config = true;
                    show_charSelector = true;

                    style.FrameRounding = 0.f;
                    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

                    show_login = false;
                }
            }
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }

    }
    ImGui::End();
}