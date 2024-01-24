#define GL_SILENCE_DEPRECATION
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#include "ImageHandler.h"
#include <stdio.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
#include "ImagePath.h"

ImageHandler::ImageHandler() = default;

bool ImageHandler::LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height)
{
    int image_width = 0;
    int image_height = 0;
    int image_channel = 0;
    auto image_data = stbi_load(filename, &image_width, &image_height, &image_channel, 0);

    if (image_data == NULL) {
        printf("Error loading image: %s\n", stbi_failure_reason());
        return 0;
    }
    std::cout <<"W-" << image_width << " H-" << image_height << " #-" << image_channel << std::endl;

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

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

bool ImageHandler::CreateAnimation(vector<string>& paths, vector<image*> &frames)
{
    ImagePath abs = ImagePath();
    for(int i = 0; i < paths.size(); i++)
    {
        int height = 0;
        int width = 0;
        GLuint texture = 0;

        string absolute = abs.absolutePath + paths.at(i);
        //todo - remove print
        cout << "-----PATH:" << absolute << endl;

        bool ret = ImageHandler::LoadTextureFromFile(absolute.c_str(), &texture, &width, &height);
        IM_ASSERT(ret);

        image* frame = new image(texture,width,height);

        frames.push_back(frame);
    }
//todo - delete print
    cout << paths[0] << " ||||| " << frames.size() << endl;
    return true;
}

void ImageHandler::DrawImage(image _image)
{
    ImGui::Image((void*)(intptr_t)_image.texture, ImVec2(_image.width, _image.height));
}

