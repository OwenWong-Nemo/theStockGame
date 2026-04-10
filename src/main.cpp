#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <random>
#include <string>
#include "model/stock.hpp"
#include "view/StockRenderer.hpp"

enum class GameState { Market, News, Portfolio, Sell };

int main() {
    // Setup Window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Stock Game MVP");
    window.setFramerateLimit(60);

    // Load Assets
    sf::Font font;
    if (!font.openFromFile("assets/font.ttf")) return -1;

    // Game Data
    GameState currentState = GameState::Market;
    int day = 1;
    float balance = 100.0f;

    std::vector<Stock> market;
    market.reserve(5);
    market.emplace_back("TECH NOVA");
    market.emplace_back("BIO ROOT");
    market.emplace_back("SOLARIS");
    market.emplace_back("CYBER CORE");
    market.emplace_back("AERO DYNAMICS");

    // UI Elements: Header & Backgrounds
    sf::RectangleShape header({780.f, 60.f});
    header.setFillColor(sf::Color(35, 35, 35));
    header.setPosition({10.f, 10.f});

    sf::Text headerText(font);
    headerText.setCharacterSize(20);
    headerText.setPosition({25.f, 25.f});
    headerText.setFillColor(sf::Color::White);

    // UI Elements: Next Day Button
    sf::RectangleShape btnNextDay({120.f, 40.f});
    btnNextDay.setFillColor(sf::Color(0, 120, 0));
    btnNextDay.setPosition({650.f, 20.f});

    sf::Text nextDayLabel(font, "NEXT DAY");
    nextDayLabel.setCharacterSize(14);
    nextDayLabel.setPosition({670.f, 32.f});

    // UI Elements: Main Content Area
    sf::RectangleShape mainArea({780.f, 400.f});
    mainArea.setFillColor(sf::Color(20, 20, 20));
    mainArea.setPosition({10.f, 80.f});

    // Navigation Buttons (Bottom)
    sf::RectangleShape btnNews({250.f, 80.f});
    sf::RectangleShape btnPortfolio({250.f, 80.f});
    sf::RectangleShape btnSell({250.f, 80.f});

    btnNews.setPosition({10.f, 500.f});
    btnPortfolio.setPosition({275.f, 500.f});
    btnSell.setPosition({540.f, 500.f});

    btnNews.setFillColor(sf::Color(0, 50, 150));
    btnPortfolio.setFillColor(sf::Color(100, 0, 150));
    btnSell.setFillColor(sf::Color(150, 0, 0));

    sf::Text newsLabel(font, "NEWS", 20);
    newsLabel.setPosition({105.f, 525.f});

    sf::Text portLabel(font, "PORTFOLIO", 20);
    portLabel.setPosition({340.f, 525.f});

    sf::Text sellLabel(font, "SELL", 20);
    sellLabel.setPosition({640.f, 525.f});

    // Main Game Loop
    while (window.isOpen()) {
        // Event Polling
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Next Day Logic
                if (btnNextDay.getGlobalBounds().contains(mousePos)) {
                    day++;
                    for (auto& s : market) s.updatePrice();
                    currentState = GameState::Market; 
                }

                // Nav Clicks
                if (btnNews.getGlobalBounds().contains(mousePos)) currentState = GameState::News;
                if (btnPortfolio.getGlobalBounds().contains(mousePos)) currentState = GameState::Portfolio;
                if (btnSell.getGlobalBounds().contains(mousePos)) currentState = GameState::Sell;
            }
        }

        // Update Dynamic Header Text
        headerText.setString("Day: " + std::to_string(day) + "  |  Balance: $" + std::to_string((int)balance));

        // --- Rendering ---
        window.clear(sf::Color::Black);

        window.draw(header);
        window.draw(headerText);
        window.draw(btnNextDay);
        window.draw(nextDayLabel);
        window.draw(mainArea);

        // State-Based Content Rendering
        if (currentState == GameState::Market) {
            for (int i = 0; i < market.size(); ++i) {
                // Using our static View class to draw the Model data
                StockRenderer::drawStockRow(window, font, market[i], i);
            }
        } else {
            std::string stateName = "View";
            if(currentState == GameState::News) stateName = "News Feed";
            if(currentState == GameState::Portfolio) stateName = "My Portfolio";
            if(currentState == GameState::Sell) stateName = "Sell Center";

            sf::Text msg(font, stateName, 30);
            msg.setPosition({300.f, 250.f});
            window.draw(msg);
        }

        // Draw Navigation
        window.draw(btnNews); window.draw(newsLabel);
        window.draw(btnPortfolio); window.draw(portLabel);
        window.draw(btnSell); window.draw(sellLabel);

        window.display();
    }

    return 0;
}