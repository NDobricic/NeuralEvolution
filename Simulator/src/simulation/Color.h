#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <assert.h>
#include "../Random.h"

namespace evol
{
    struct Color
    {
        uint8_t R;
        uint8_t G;
        uint8_t B;
        uint8_t A;

        static constexpr Color red() { return Color(255, 0, 0); }
        static constexpr Color green() { return Color(0, 255, 0); }
        static constexpr Color blue() { return Color(0, 0, 255); }
        static constexpr Color black() { return Color(0, 0, 0); }
        static constexpr Color white() { return Color(255, 255, 255); }

        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
            : R(r), G(g), B(b), A(a)
        { }

        static Color Random(int seed)
        {
            srand(seed);
            return Color(Random::Next(0, 255), Random::Next(0, 255), Random::Next(0, 255));
        }
        static Color Random() { return Random(Random::Next(INT_MIN, INT_MAX)); }

        static Color Combine(Color col1, Color col2)
        {
            uint8_t r = (col1.R + col2.R) / 2;
            uint8_t g = (col1.G + col2.G) / 2;
            uint8_t b = (col1.B + col2.B) / 2;
            uint8_t a = (col1.A + col2.A) / 2;

            return Color(r, g, b, a);
        }

        static constexpr Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return Color(r, g, b, a); }
        static constexpr Color FromRGBA(Color color, uint8_t a) { return Color(color.R, color.G, color.B, a); }

        static Color FromHex(std::string hex)
        {
            if (hex[0] == '#')
                hex.erase(hex.begin());

            _ASSERT(hex.length() == 6 || hex.length() == 8);

            if (hex.length() == 6)
            {
                return FromRGBA(255,
                    stoi(hex.substr(0, 2), nullptr, 16),
                    stoi(hex.substr(2, 2), nullptr, 16),
                    stoi(hex.substr(4, 2), nullptr, 16));
            }
            else //hex.length() == 8
            {
                return FromRGBA(stoi(hex.substr(0, 2), nullptr, 16),
                    stoi(hex.substr(2, 2), nullptr, 16),
                    stoi(hex.substr(4, 2), nullptr, 16),
                    stoi(hex.substr(6, 2), nullptr, 16));
            }
        }
    };
}
