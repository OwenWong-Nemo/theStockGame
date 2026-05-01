#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include "controller/keyHandler.hpp"
#include "controller/mouseHandler.hpp"
#include "model/stock.hpp"
#include "view/gameRender.hpp"

int main() {
    // Setup Window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Stock Game MVP");
    window.setFramerateLimit(60);

    // Load Assets
    sf::Font font;
    if (!font.openFromFile("assets/font.ttf")) return -1;
    GameRender gameRender(font);

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

    std::unordered_map<std::string, int> portfolio;
    bool showBuyPopup = false;
    int selectedStockIndex = -1;
    int buyShares = 1;
    std::string buyMessage;

    bool showSellPopup = false;
    std::string sellSelectedSymbol;
    int sellShares = 1;
    std::string sellMessage;

    // Main Game Loop
    while (window.isOpen()) {
        // Event Polling
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (showBuyPopup) {
                        KeyHandler::handleBuyPopupKeyPress(
                            *keyEvent,
                            market,
                            balance,
                            portfolio,
                            showBuyPopup,
                            selectedStockIndex,
                            buyShares,
                            buyMessage
                        );
                    } else if (showSellPopup) {
                        KeyHandler::handleSellPopupKeyPress(
                            *keyEvent,
                            market,
                            balance,
                            portfolio,
                            showSellPopup,
                            sellSelectedSymbol,
                            sellShares,
                            sellMessage
                        );
                    } else {
                        KeyHandler::handleNavigationKeyPress(*keyEvent, currentState);
                    }
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                MouseHandler::handleMouseClick(
                    mousePos,
                    gameRender,
                    market,
                    currentState,
                    balance,
                    day,
                    showBuyPopup,
                    selectedStockIndex,
                    buyShares,
                    buyMessage,
                    showSellPopup,
                    sellSelectedSymbol,
                    sellShares,
                    sellMessage,
                    portfolio
                );
            }
        }

        gameRender.drawFrame(
            window,
            currentState,
            day,
            balance,
            market,
            portfolio,
            showBuyPopup,
            selectedStockIndex,
            buyShares,
            buyMessage,
            showSellPopup,
            sellSelectedSymbol,
            sellShares,
            sellMessage
        );
    }

    return 0;
}