#pragma once
#include <string>

class Stock {
private:
    std::string name;
    float startingPrice;
    float currentPrice;
    float priceChangePercentage;
    bool isPriceChanged;

public:
    // Constructor method
    Stock(std::string n);

    // Mutator methods
    float setStartingPrice();
    float setPriceChangePercentage();  
    void updatePrice();
    /// One-time multiplier (e.g. news gap) before the regular `updatePrice` random walk.
    void applyPriceMultiplier(float factor);
    
    // Accessor methods (const: safe to call on const Stock&, e.g. from UI)
    std::string getName() const;
    float getCurrentPrice() const;
    float getPriceChangePercentage() const;
    bool getIsPriceChanged() const;
};