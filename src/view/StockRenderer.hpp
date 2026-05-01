#pragma once
#include <SFML/Graphics.hpp>
#include "../model/stock.hpp"

class StockRenderer {
public:
    static void drawStockRow(sf::RenderWindow& window, const sf::Font& font, const Stock& stock, int index);
    static bool isClicked(sf::Vector2f mousePos, int index);
};
