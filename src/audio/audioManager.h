//
// Created by Lucas Pereira on 2/16/24.
//

#ifndef HASH_HARBOR_AUDIOMANAGER_H
#define HASH_HARBOR_AUDIOMANAGER_H

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


class audioManager {
    std::map<string, audio> audioMap;

public:
    audioManager();

    void handleSound(string soundName);


};


#endif //HASH_HARBOR_AUDIOMANAGER_H
