#pragma once
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include "../model/stock.hpp"

class StockRenderer {
public:
static void drawStockRow(sf::RenderWindow& window, sf::Font& font, const Stock& stock, int index) 
    {
        float yPos = 100.0f + (index * 70.0f);

        // Draw Background
        sf::RectangleShape bg({740.f, 60.f});
        bg.setFillColor(sf::Color(45, 45, 45));
        bg.setOutlineThickness(1.0f);
        bg.setOutlineColor(sf::Color::White);
        bg.setPosition({30.f, yPos});

        // Format the Base Information
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << stock.getName() << " | $" << stock.getCurrentPrice();

        if (stock.getIsPriceChanged()) {
            float percent = stock.getPriceChangePercentage() * 100.0f;
            
            // Add sign ('+' or '-') for readability
            std::string sign = (percent >= 0) ? "+" : ""; 
            oss << " | " << sign << percent << "%";
        }

        // Draw the Text
        sf::Text text(font, oss.str(), 20u);
        text.setPosition({50.f, yPos + 18.f});
        text.setFillColor(sf::Color::White);

        // Add colour based on performance
        if (stock.getIsPriceChanged()) {
            if (stock.getPriceChangePercentage() > 0) text.setFillColor(sf::Color::Green);
            else if (stock.getPriceChangePercentage() < 0) text.setFillColor(sf::Color::Red);
        }

        window.draw(bg);
        window.draw(text);
    }

    static bool isClicked(sf::Vector2f mousePos, int index) {
        sf::FloatRect bounds({30.f, 100.0f + (index * 70.0f)}, {740.f, 60.f});
        return bounds.contains(mousePos);
    }
};