#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "../model/stock.hpp"
#include "../view/gameRender.hpp"

class MouseHandler {
public:
    static void handleMouseClick(
        sf::Vector2f mousePos,
        GameRender& gameRender,
        std::vector<Stock>& market,
        GameState& currentState,
        float balance,
        int& day,
        bool& showBuyPopup,
        int& selectedStockIndex,
        int& buyShares,
        std::string& buyMessage,
        bool& showSellPopup,
        std::string& sellSelectedSymbol,
        int& sellShares,
        std::string& sellMessage,
        const std::unordered_map<std::string, int>& portfolio
    );
};
