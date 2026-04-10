#pragma once
#include <random>

class RandomHelper {
private:
    static std::mt19937& getEngine();

public:
    static float getFloat(float min, float max);
    static int getInt(int min, int max);
    static bool flipCoin();
};