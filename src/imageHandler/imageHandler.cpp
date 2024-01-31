#include "imageHandler.h"
#include "imagePath.h"
#include <stdio.h>

#if defined(__APPLE__)
#include <iostream>
using std::string;
using std::vector;
using std::cout;
using std::endl;
#else
#include <bits/stdc++.h>
#endif


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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

imageHandler::imageHandler(){}

imageHandler::imageHandler(const char* filepath){
    this->filepath = filepath;
}

imageHandler::imageHandler(GLuint texture, int width, int height){
    this->texture = texture;
    this->width = width;
    this->height = height;
}

bool imageHandler::loadTexture(const char *filename, imageHandler* image){
    int image_width = 0;
    int image_height = 0;
    int image_channel = 0;
    auto image_data = stbi_load(filename, &image_width, &image_height, &image_channel, 0);
    
    if(image_data == NULL)
    {
        cout << filename << endl;
        printf("Error loading image: %s\n", stbi_failure_reason());
        return false;
    }

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    image->texture = image_texture;
    image->width= image_width;
    image->height = image_height;

    return true;
}

void imageHandler::DrawImage(imageHandler _image)
{
    ImGui::Image((void*)(intptr_t)_image.texture, ImVec2(_image.width, _image.height));
}

void imageHandler::DrawImage(imageHandler& _image, float scaleFactor)
{
    ImGui::Image((void*)(intptr_t)_image.texture, ImVec2((_image.width * scaleFactor), (_image.height * scaleFactor)));
}

void imageHandler::DrawAniamtionFrame(imageHandler _image, pair<ImVec2,ImVec2> cords, float scaleFactor)
{
    ImGui::Image((void*)(intptr_t)_image.texture, ImVec2((32.f * scaleFactor), (64.f * scaleFactor)),cords.first, cords.second);
}

