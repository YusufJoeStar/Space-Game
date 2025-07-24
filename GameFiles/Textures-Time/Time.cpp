#include "Textures-Time/Time.h"

Time::Time() {
    deltareset();
}

Time::~Time() {}

void Time::deltareset() {
    delta = clock.restart().asSeconds();
}

float Time::getdelta() const {
    return delta;
}
