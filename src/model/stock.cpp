#include "stock.hpp"
#include <iostream>
#include "../utils/randomHelper.hpp"

Stock::Stock(std::string n) {
    name = n;
    startingPrice = setStartingPrice();
    currentPrice = startingPrice;
    priceChangePercentage = 0.0f;
    isPriceChanged = false;
}

float Stock::setStartingPrice() {
    return RandomHelper::getFloat(1.0f, 15.0f);
}

float Stock::setPriceChangePercentage() {
    return RandomHelper::getFloat(-0.8f, 1.0f);
}

void Stock::updatePrice() {
    float p = setPriceChangePercentage();
    currentPrice *= (1.0f + p);
    priceChangePercentage = p;
    isPriceChanged = true;
}

void Stock::applyPriceMultiplier(float factor) {
    if (factor <= 0.f) {
        return;
    }
    currentPrice *= factor;
    priceChangePercentage = factor - 1.0f;
    isPriceChanged = true;
}

std::string Stock::getName() const { return name; }
float Stock::getCurrentPrice() const { return currentPrice; }
float Stock::getPriceChangePercentage() const { return priceChangePercentage; }
bool Stock::getIsPriceChanged() const { return isPriceChanged; }
