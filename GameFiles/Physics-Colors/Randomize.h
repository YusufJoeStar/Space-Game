#pragma once
#include <random>
#include "Textures-Time/Time.h"

class Randomize {
private:
    Time times;
    std::default_random_engine randomEngine;
    std::uniform_int_distribution<int> randomInt;
    std::uniform_real_distribution<float> randomFloat;

    void initRandomEngine();

public:
    Randomize();
    ~Randomize();

    void randomizeInt(int min, int max);
    void randomizeFloat(float min, float max);
    int randomI();
    float randomF();
};
