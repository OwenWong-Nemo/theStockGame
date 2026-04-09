#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <random>

enum class GameState { Market, News, Portfolio, Sell };

// --- OOP Stock Class ---
class Stock {
private:
    std::string name;
    float price;
    sf::RectangleShape rowBg;
    sf::Text displayText;

public:
    Stock(std::string n, float p, sf::Font& font, int index) 
        : name(n), price(p), displayText(font) 
    {
        float yPos = 100.0f + (index * 70.0f);
        rowBg.setSize({740.f, 60.f});
        rowBg.setFillColor(sf::Color(45, 45, 45));
        rowBg.setOutlineColor(sf::Color::White);
        rowBg.setOutlineThickness(1.0f);
        rowBg.setPosition({30.f, yPos});

        displayText.setCharacterSize(20);
        displayText.setFillColor(sf::Color::White);
        displayText.setPosition({50.f, yPos + 15.f});
        updateUI();
    }

    void updatePrice() {
        static std::default_random_engine engine(std::random_device{}());
        std::uniform_real_distribution<float> dist(0.90f, 1.10f);
        price *= dist(engine);
        updateUI();
    }

    void updateUI() {
        displayText.setString(name + " | Price: $" + std::to_string((int)price));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(rowBg);
        window.draw(displayText);
    }

    bool isClicked(sf::Vector2f mousePos) const {
        return rowBg.getGlobalBounds().contains(mousePos);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Stock Game MVP");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("assets/font.ttf")) return -1;

    // --- Data ---
    GameState currentState = GameState::Market;
    int day = 1;
    float balance = 100.0f;
    std::vector<Stock> market;
    market.reserve(5);
    market.emplace_back("TECH NOVA", 150.f, font, 0);
    market.emplace_back("BIO ROOT", 45.f, font, 1);
    market.emplace_back("SOLARIS", 210.f, font, 2);

    // --- UI Elements ---
    sf::RectangleShape header({780.f, 60.f});
    header.setFillColor(sf::Color(35, 35, 35));
    header.setPosition({10.f, 10.f});

    sf::Text headerText(font);
    headerText.setCharacterSize(20);
    headerText.setPosition({25.f, 25.f});

    sf::RectangleShape btnNextDay({120.f, 40.f});
    btnNextDay.setFillColor(sf::Color(0, 120, 0));
    btnNextDay.setPosition({650.f, 20.f});
    sf::Text nextDayLabel(font, "NEXT DAY");
    nextDayLabel.setCharacterSize(14);
    nextDayLabel.setPosition({670.f, 32.f});

    sf::RectangleShape mainArea({780.f, 400.f});
    mainArea.setFillColor(sf::Color(20, 20, 20));
    mainArea.setPosition({10.f, 80.f});

    // --- Navigation Buttons (The 3 missing buttons) ---
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

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Nav Clicks
                if (btnNews.getGlobalBounds().contains(mousePos)) currentState = GameState::News;
                if (btnPortfolio.getGlobalBounds().contains(mousePos)) currentState = GameState::Portfolio;
                if (btnSell.getGlobalBounds().contains(mousePos)) currentState = GameState::Sell;
                
                // Reset to Market if clicking background or somewhere else
                if (mainArea.getGlobalBounds().contains(mousePos) && currentState != GameState::Market) {
                    // Logic to return to market could go here
                }

                // Next Day Logic
                if (btnNextDay.getGlobalBounds().contains(mousePos)) {
                    day++;
                    for (auto& s : market) s.updatePrice();
                    currentState = GameState::Market; // Default back to market on new day
                }
            }
        }

        headerText.setString("Day: " + std::to_string(day) + "  |  Balance: $" + std::to_string((int)balance));

        window.clear(sf::Color::Black);
        window.draw(header);
        window.draw(headerText);
        window.draw(btnNextDay);
        window.draw(nextDayLabel);
        window.draw(mainArea);

        // Conditional Rendering based on state
        if (currentState == GameState::Market) {
            for (auto& s : market) s.draw(window);
        } else {
            // Placeholder text for other states
            sf::Text stateMsg(font, "Viewing: " + std::to_string((int)currentState), 30);
            stateMsg.setPosition({300.f, 250.f});
            window.draw(stateMsg);
        }

        // Always draw navigation
        window.draw(btnNews); window.draw(newsLabel);
        window.draw(btnPortfolio); window.draw(portLabel);
        window.draw(btnSell); window.draw(sellLabel);

        window.display();
    }
    return 0;
}