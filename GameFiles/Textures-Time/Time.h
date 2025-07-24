#pragma once
#include <SFML/Graphics.hpp>

#include <chrono>

class Time {
private:
    sf::Clock clock;
    float delta;

public:
    Time();
    ~Time();

    void deltareset();
    float getdelta() const;


    std::chrono::milliseconds::rep getDtEpoch() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }
};
