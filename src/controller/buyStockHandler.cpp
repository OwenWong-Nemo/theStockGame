#include "buyStockHandler.hpp"

int BuyStockHandler::maxAffordableBuyShares(float balance, float unitPrice) {
    if (unitPrice <= 0.f || balance <= 0.f) {
        return 0;
    }
    return static_cast<int>(balance / unitPrice);
}

void BuyStockHandler::tryExecute(
    const std::vector<Stock>& market,
    int selectedStockIndex,
    int buyShares,
    float& balance,
    std::unordered_map<std::string, int>& portfolio,
    bool& showBuyPopup,
    std::string& buyMessage
) {
    if (selectedStockIndex < 0 || selectedStockIndex >= static_cast<int>(market.size())) {
        return;
    }

    if (buyShares <= 0) {
        buyMessage = "Shares must be at least 1.";
        return;
    }

    const Stock& stock = market[selectedStockIndex];
    const float unitPrice = stock.getCurrentPrice();
    const float totalCost = unitPrice * static_cast<float>(buyShares);

    if (totalCost > balance) {
        buyMessage = "Not enough balance.";
        return;
    }

    balance -= totalCost;
    portfolio[stock.getName()] += buyShares;
    buyMessage = "Order executed.";
    showBuyPopup = false;
}
