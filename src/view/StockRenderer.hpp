#pragma once
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include "../model/stock.hpp"

class StockRenderer {
public:
    static void drawStockRow(sf::RenderWindow& window, sf::Font& font, const Stock& stock, int index) {
        float yPos = 100.0f + (index * 70.0f);

        sf::RectangleShape bg({740.f, 60.f});
        bg.setFillColor(sf::Color(45, 45, 45));
        bg.setOutlineThickness(1.0f);
        bg.setOutlineColor(sf::Color::White);
        bg.setPosition({30.f, yPos});

        std::ostringstream priceStr;
        priceStr << std::fixed << std::setprecision(2) << stock.getCurrentPrice();
        sf::Text text(font, stock.getName() + " | $" + priceStr.str(), 20);
        text.setPosition({50.f, yPos + 18.f});
        text.setFillColor(sf::Color::White);

        window.draw(bg);
        window.draw(text);
    }

    static bool isClicked(sf::Vector2f mousePos, int index) {
        sf::FloatRect bounds({30.f, 100.0f + (index * 70.0f)}, {740.f, 60.f});
        return bounds.contains(mousePos);
    }
};