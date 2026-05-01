#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "../model/stock.hpp"

enum class GameState { Market, News, Portfolio, Sell };

class GameRender {
public:
    explicit GameRender(const sf::Font& font);

    void drawFrame(
        sf::RenderWindow& window,
        GameState currentState,
        int day,
        float balance,
        const std::vector<Stock>& market,
        const std::unordered_map<std::string, int>& portfolio,
        bool showBuyPopup,
        int selectedStockIndex,
        int buyShares,
        const std::string& buyMessage,
        bool showSellPopup,
        const std::string& sellSelectedSymbol,
        int sellShares,
        const std::string& sellMessage
    );

    bool isNextDayClicked(sf::Vector2f mousePos) const;
    int getClickedStockIndex(sf::Vector2f mousePos, int stockCount) const;
    int getClickedHoldingRowIndex(sf::Vector2f mousePos, int rowCount) const;

private:
    const sf::Font& font;

    sf::RectangleShape header;
    sf::RectangleShape btnNextDay;
    sf::RectangleShape mainArea;
    sf::RectangleShape btnNews;
    sf::RectangleShape btnPortfolio;
    sf::RectangleShape btnSell;

    sf::Text headerText;
    sf::Text nextDayLabel;
    sf::Text newsLabel;
    sf::Text portLabel;
    sf::Text sellLabel;

    std::string getStateName(GameState currentState) const;
};
