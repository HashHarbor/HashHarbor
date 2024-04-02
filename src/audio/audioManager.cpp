//
// Created by Lucas Pereira on 2/16/24.
//

#include "audioManager.h"
#include "audio.h"

#if defined(__APPLE__)
#include <iostream>
#include <map>
using std::string;
using std::cout;
using std::endl;
#else
#include <bits/stdc++.h>
#endif



audioManager::audioManager() {
    audio walking = audio();
    walking.load("/Users/lucaspereira/CLionProjects/hash_harbor/HashHarbor-main/assets/soundEffects/walking_on_grass.wav");
    audioMap.insert("walking", walking);

    audio interaction = audio();
    interaction.load("");
    songList.push_back(interaction);
}

void audioManager::handleSound(string soundName, bool play) {
    audio toPlay = audioMap.find(soundName);

    if(!toPlay.getPlayStatus() && play){
        toPlay.play();
    }
    else if(toPlay.getPlayStatus() && !play){
        toPlay.stop();
    }
}


