#pragma once
#include <random>

class RandomHelper {
private:
    // A static engine ensures we don't reset the seed repeatedly
    static std::mt19937& getEngine() {
        static std::random_device rd;
        static std::mt19937 engine(rd());
        return engine;
    }

public:
    // Generate a float between min and max
    static float getFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(getEngine());
    }

    // Generate an int between min and max
    static int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(getEngine());
    }

    // A helper specifically for stock market "flips" (e.g., 50/50 chance)
    static bool flipCoin() {
        return getInt(0, 1) == 1;
    }
};