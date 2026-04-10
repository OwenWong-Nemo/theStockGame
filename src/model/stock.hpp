#pragma once
#include <string>
#include "../utils/randomHelper.hpp"

class Stock {
private:
    std::string name;
    float startingPrice;
    float currentPrice;
    float priceChangePercentage;
    bool isPriceChanged;

public:
    // Constructors method
    Stock(std::string n)
    {
        name = n;
        startingPrice = setStartingPrice();
        currentPrice = startingPrice;
        priceChangePercentage = 0.0f;
        isPriceChanged = false;
    }

    float setStartingPrice() {
        int minPrice = 1;
        int maxPrice = 30; 
        float startingPrice = RandomHelper::getFloat(1.0f, 15.0f);

        return startingPrice;
    }

    // Mutator methods
    float setPriceChangePercentage() 
    {
        // Can change for more fair game play
        float change = RandomHelper::getFloat(-0.8f, 1.0f);
        return change;
    }
        
    void updatePrice() 
    {
        float p = setPriceChangePercentage();
        currentPrice *= (1.0f + p);
        isPriceChanged = true;
    }

    // Accessor methods 
    std::string getName() const { return name; }
    float getCurrentPrice() const { return currentPrice; }
    float getPriceChangePercentage() const { return priceChangePercentage; }
    bool getIsPriceChanged() const { return isPriceChanged; }

    // Helper function
    float randomFloat()
    {
        return (float)rand() / (float)RAND_MAX;
    }
};