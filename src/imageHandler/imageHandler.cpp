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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

void imageHandler::DrawAnimationFrame(imageHandler _image, pair<ImVec2,ImVec2> cords, float scaleFactor)
{
    ImGui::Image((void*)(intptr_t)_image.texture, ImVec2((32.f * scaleFactor), (64.f * scaleFactor)),cords.first, cords.second);
}

pair<ImVec2, ImVec2> imageHandler::generateCords(int animation, int frame, float spriteWidth, float spriteHeight, float imageWidth, float imageHeight)
{
    float minX = ((float)frame * spriteWidth);
    float minY = ((float)animation * spriteHeight);
    float maxX = (minX + spriteWidth) / imageWidth;
    float maxY = (minY + spriteHeight) / imageHeight;

    minX /= imageWidth;
    minY /= imageHeight;

    minX += 0.1f;
    minY += 0.1f;
    if(maxX != 0.0f)
    {
        maxX -= 0.1f;
    }
    return make_pair(ImVec2(minX,minY), ImVec2(maxX,maxY));
    // Example if character: imgHandler->generateCords(1,frameCount_6,32.f,64.f,192.f,320.f)
}

void imageHandler::cleanUp()
{
    glDeleteTextures(1, &texture);
}
void imageHandler::DrawMap(imageHandler _image, double tileX, double tileY, float width, float height, int mapRows, int mapCols){
    //ImGui::Image((void*)(intptr_t)_image.texture, ImVec2(_image.width, _image.height));
    
    //((height / 32) - 1) / 2 finds the number of pixels our resolution can support and takes the half of it needed relative from the center
    
    float topLeft = (float)(tileX - ((width / 32) - 1) / 2)/ mapCols;  //ex. topLeft corner is 15 pixels to the left out of the total number of columns
    float top = (float)(tileY - ((height / 32) - 1) / 2) / mapRows;

    float botRight = (float)(tileX + ((width / 32) - 1) / 2 + 1)/ mapCols;
    float bot = (float)(tileY + ((height / 32) - 1) / 2 + 1) / mapRows;

    ImGui::Image((void*)(intptr_t)_image.texture, ImVec2(width, height), ImVec2(topLeft, top), ImVec2(botRight, bot));

    //calculated by where the corners of the grid are, out of the total grid size, done by percentages
}