#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

class audio {
    bool playing;

    SDL_AudioDeviceID deviceId;
    Uint8* audioBuffer;
    Uint32 audioLength;
    Uint8* audioPos;
    Uint32 remainingLength;


    static void audioCallback(void* userdata, Uint8* stream, int len);

    void resetPlayback();

public:

    audio() : deviceId(0), audioBuffer(nullptr), audioLength(0), audioPos(nullptr) {
        SDL_Init(SDL_INIT_AUDIO); // Initialize SDL2 audio subsystem
        playing = false;
    }

    ~audio() {
        if (deviceId != 0) {
            SDL_CloseAudioDevice(deviceId);
        }
        if (audioBuffer != nullptr) {
            SDL_FreeWAV(audioBuffer);
        }
        SDL_Quit();
    }

    void gameMusic();

    bool load(const std::string& fileName);

    void play();

    void stop();

    bool getPlayStatus();

    void setPlayStatus(bool playStatus);


};

