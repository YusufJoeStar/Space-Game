#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

class Colors {
private:

public:
    Colors() {}
    ~Colors() {}

    sf::Color HSVToRGB(float h, float s, float v) {
        h = fmod(h, 360.0f);
        if (h < 0) h += 360.0f;
        s = std::clamp(s, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);

        float hPrime = (h / 60.0f);
        int hIndex = static_cast<int>(hPrime) % 6;
        float chroma = s * v;
        float min = v - chroma;
        float x = chroma * (1.0f - std::abs(fmod(hPrime, 2.0f) - 1.0f));

        float outRGB[6][3] = {
            {chroma, x, 0.0f},
            {x, chroma, 0.0f},
            {0.0f, chroma, x},
            {0.0f, x, chroma},
            {x, 0.0f, chroma},
            {chroma, 0.0f, x}
        };

        float rF = (outRGB[hIndex][0] + min) * 255.0f;
        float gF = (outRGB[hIndex][1] + min) * 255.0f;
        float bF = (outRGB[hIndex][2] + min) * 255.0f;

        std::uint8_t rI = std::uint8_t(std::clamp(rF, 0.0f, 255.0f));
        std::uint8_t gI = std::uint8_t(std::clamp(gF, 0.0f, 255.0f));
        std::uint8_t bI = std::uint8_t(std::clamp(bF, 0.0f, 255.0f));

        return sf::Color(rI, gI, bI);
    }
};
