#include "keyHandler.hpp"

#include <algorithm>

#include "../utils/keybinds.hpp"
#include "buyStockHandler.hpp"
#include "sellStockHandler.hpp"

void KeyHandler::handleNavigationKeyPress(const sf::Event::KeyPressed& keyEvent, GameState& currentState) {
    const auto key = keyEvent.code;

    if (Keybinds::matchesNewsTab(key)) {
        currentState = (currentState == GameState::News) ? GameState::Market : GameState::News;
        return;
    }
    if (Keybinds::matchesPortfolioTab(key)) {
        currentState = (currentState == GameState::Portfolio) ? GameState::Market : GameState::Portfolio;
        return;
    }
    if (Keybinds::matchesSellTab(key)) {
        currentState = (currentState == GameState::Sell) ? GameState::Market : GameState::Sell;
    }
}

void KeyHandler::handleBuyPopupKeyPress(
    const sf::Event::KeyPressed& keyEvent,
    const std::vector<Stock>& market,
    float& balance,
    std::unordered_map<std::string, int>& portfolio,
    bool& showBuyPopup,
    int selectedStockIndex,
    int& buyShares,
    std::string& buyMessage
) {
    const auto key = keyEvent.code;

    int maxBuy = 0;
    if (selectedStockIndex >= 0 && selectedStockIndex < static_cast<int>(market.size())) {
        maxBuy = BuyStockHandler::maxAffordableBuyShares(balance, market[selectedStockIndex].getCurrentPrice());
    }
    if (maxBuy <= 0) {
        buyShares = 0;
    } else {
        buyShares = std::clamp(buyShares, 1, maxBuy);
    }

    if (Keybinds::matchesCancel(key)) {
        showBuyPopup = false;
        buyMessage.clear();
        return;
    }

    if (Keybinds::matchesConfirm(key)) {
        BuyStockHandler::tryExecute(
            market,
            selectedStockIndex,
            buyShares,
            balance,
            portfolio,
            showBuyPopup,
            buyMessage
        );
        return;
    }

    if (Keybinds::matchesIncreaseAmount(key)) {
        buyShares = std::min(buyShares + 1, maxBuy);
        return;
    }

    if (Keybinds::matchesDecreaseAmount(key)) {
        if (maxBuy <= 0) {
            return;
        }
        buyShares = std::max(1, buyShares - 1);
    }
}

void KeyHandler::handleSellPopupKeyPress(
    const sf::Event::KeyPressed& keyEvent,
    const std::vector<Stock>& market,
    float& balance,
    std::unordered_map<std::string, int>& portfolio,
    bool& showSellPopup,
    std::string& sellSelectedSymbol,
    int& sellShares,
    std::string& sellMessage
) {
    const auto key = keyEvent.code;

    int maxSell = 0;
    if (!sellSelectedSymbol.empty()) {
        const auto it = portfolio.find(sellSelectedSymbol);
        if (it != portfolio.end()) {
            maxSell = it->second;
        }
    }
    if (maxSell <= 0) {
        sellShares = 0;
    } else {
        sellShares = std::clamp(sellShares, 1, maxSell);
    }

    if (Keybinds::matchesCancel(key)) {
        showSellPopup = false;
        sellMessage.clear();
        sellSelectedSymbol.clear();
        return;
    }

    if (Keybinds::matchesConfirm(key)) {
        SellStockHandler::tryExecute(
            market,
            sellSelectedSymbol,
            sellShares,
            balance,
            portfolio,
            showSellPopup,
            sellMessage
        );
        if (!showSellPopup) {
            sellSelectedSymbol.clear();
        }
        return;
    }

    if (Keybinds::matchesIncreaseAmount(key)) {
        sellShares = std::min(sellShares + 1, maxSell);
        return;
    }

    if (Keybinds::matchesDecreaseAmount(key)) {
        if (maxSell <= 0) {
            return;
        }
        sellShares = std::max(1, sellShares - 1);
    }
}
