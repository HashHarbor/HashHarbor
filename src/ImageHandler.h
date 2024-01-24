#pragma once

#define GL_SILENCE_DEPRECATION

#include "imgui.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include <iostream>
#include <vector>
using std::vector;
using std::string;

struct image // stores individual images and their required data
{
public:
    int width;
    int height;
    GLuint texture;

    image(GLuint texture, int width, int height)
    {
        this->width = width;
        this->height = height;
        this->texture = texture;
    }
};

class ImageHandler
{
public:
    ImageHandler();
    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
    // create animation
    bool CreateAnimation(vector<string>& paths, vector <image*> &frames);
    //draw image
    void DrawImage(image _image);
};

