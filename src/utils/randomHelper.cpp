#include "randomHelper.hpp"

// No 'static' here!
std::mt19937& RandomHelper::getEngine() {
    static std::random_device rd;
    static std::mt19937 engine(rd());
    return engine;
}

float RandomHelper::getFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(getEngine());
}

int RandomHelper::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(getEngine());
}

bool RandomHelper::flipCoin() {
    return getInt(0, 1) == 1;
}