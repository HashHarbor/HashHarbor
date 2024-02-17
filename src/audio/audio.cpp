#include "audio.h"

bool audio::load(const std::string& fileName) {
    if (audioBuffer != nullptr) {
        SDL_FreeWAV(audioBuffer);
        audioBuffer = nullptr;
        audioLength = 0;
    }

    SDL_AudioSpec wavSpec;
    if (SDL_LoadWAV(fileName.c_str(), &wavSpec, &audioBuffer, &audioLength) == NULL) {
        std::cerr << "Failed to load WAV file! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set the audio callback and userdata
    wavSpec.callback = audioCallback;
    wavSpec.userdata = this;

    // Open the audio device
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (deviceId == 0) {
        std::cerr << "Failed to open audio device! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    audioPos = audioBuffer; // Set the initial audio position
    remainingLength = audioLength; // Set the initial remaining length

    return true;
}

void audio::play(){
    if (deviceId != 0) {
        std::cout << "reached" << std::endl;
        playing = true;
        SDL_PauseAudioDevice(deviceId, 0); // Start audio playback
    }
}

// pauses and resets playback
void audio::stop() {
    if (deviceId != 0) {
        playing = false;
        SDL_PauseAudioDevice(deviceId, 1);
        resetPlayback();
    }
}

void audio::gameMusic() {}


// void audio::characterSound(characterManager &character) {
//     // start walking audio after key input and if audio is not currently playing
//     if(character.getMovementStatus() && !playing){
//         if(load("/Users/lucaspereira/CLionProjects/hash_harbor/HashHarbor-main/assets/soundEffects/walking_on_grass.wav")){
//             play();
//             playing = true;
//         }
//         // need to restart audio if character walks longer than audio sequence
//     }
//     else if(!character.getMovementStatus() && playing){
//         playing = false;
//         stop();
//         // stop and reset audio
//     }
//     //
// }

// private helper functions

void audio::audioCallback(void* userdata, Uint8* stream, int len){
    audio* player = static_cast<audio*>(userdata);

    if (player->remainingLength == 0) {
        // for audio looping
        player->audioPos = player->audioBuffer;
        player->remainingLength = player->audioLength;
    }

    len = (len > player->remainingLength ? player->remainingLength : len);
    SDL_memcpy(stream, player->audioPos, len); // Copy the audio data
    player->audioPos += len;
    player->remainingLength -= len;
}

void audio::resetPlayback() {
    audioPos = audioBuffer; // Reset position to the start of the buffer
    remainingLength = audioLength; // Reset remaining length to full length
}

bool audio::getPlayStatus() {
    return playing;
}

void audio::setPlayStatus(bool playStatus){
    playing = playStatus;
}


