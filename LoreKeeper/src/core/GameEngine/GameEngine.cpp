//
// Created by bobobo on 2025/12/13.
//

#include "GameEngine.h"

namespace core {


GameEngine& GameEngine::instance() noexcept {
    static GameEngine instance;
    return instance;
}


}