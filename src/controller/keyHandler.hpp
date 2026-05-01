#pragma once

#include <SFML/Window/Event.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "../model/stock.hpp"
#include "../view/gameRender.hpp"

class KeyHandler {
public:
    /// N / P / S switch tabs; pressing the same key again returns to Market (stock list).
    static void handleNavigationKeyPress(const sf::Event::KeyPressed& keyEvent, GameState& currentState);

    static void handleBuyPopupKeyPress(
        const sf::Event::KeyPressed& keyEvent,
        const std::vector<Stock>& market,
        float& balance,
        std::unordered_map<std::string, int>& portfolio,
        bool& showBuyPopup,
        int selectedStockIndex,
        int& buyShares,
        std::string& buyMessage
    );

    static void handleSellPopupKeyPress(
        const sf::Event::KeyPressed& keyEvent,
        const std::vector<Stock>& market,
        float& balance,
        std::unordered_map<std::string, int>& portfolio,
        bool& showSellPopup,
        std::string& sellSelectedSymbol,
        int& sellShares,
        std::string& sellMessage
    );
};
