#include "imageHandler.h"
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

imageHandler::imageHandler(const char* filepath){
    this->filepath = filepath;
}

bool imageHandler::loadTexture(){
    unsigned char* image_data = stbi_load(filepath, width, height, NULL, channels);
        if(image_data == NULL){
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

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        return true;

}

bool imageHandler::CreateAnimation(vector<string>& paths, vector<image*> &frames)
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
    return true;
}

void imageHandler::DrawImage(image _image)
{
    ImGui::Image((void*)(intptr_t)_image.texture, ImVec2(_image.width, _image.height));
}

