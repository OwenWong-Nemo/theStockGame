#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include "controller/keyHandler.hpp"
#include "controller/mouseHandler.hpp"
#include "model/news.hpp"
#include "model/stock.hpp"
#include "view/gameRender.hpp"

namespace {

constexpr int kWinDeadlineDay = 10;
constexpr int kWinTargetBalance = 500;

/// 1 = win, 0 = keep playing, -1 = lose (deadline passed without enough balance).
[[nodiscard]] int checkWinCondition(int gameDay, float gameBalance) {
    if (gameDay <= kWinDeadlineDay && gameBalance >= static_cast<float>(kWinTargetBalance)) {
        return 1;
    }
    if (gameDay <= kWinDeadlineDay) {
        return 0;
    }
    return -1;
}

}  // namespace

int main() {
    // Setup Window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Stock Game");
    window.setFramerateLimit(60);

    // Load Assets
    sf::Font font;
    if (!font.openFromFile("assets/font.ttf")) return -1;
    GameRender gameRender(font);

    // Load Game Data
    GameState currentState = GameState::Market;
    int day = 1;
    float balance = 100.0f;

    // Initialise Market Data
    std::vector<Stock> market;
    market.reserve(5);
    market.emplace_back("TECH NOVA");
    market.emplace_back("BIO ROOT");
    market.emplace_back("SOLARIS");
    market.emplace_back("CYBER CORE");
    market.emplace_back("AERO DYNAMICS");

    News pendingNews = News::generateRandom(market);

    // Initialise Portfolio Data
    std::unordered_map<std::string, int> portfolio;
    bool showBuyPopup = false;
    int selectedStockIndex = -1;
    int buyShares = 1;
    std::string buyMessage;

    bool showSellPopup = false;
    std::string sellSelectedSymbol;
    int sellShares = 1;
    std::string sellMessage;

    std::string gameOverMessage;
    bool gameOverIsWin = false;

    // Main Game Loop
    while (window.isOpen()) {
        if (gameOverMessage.empty()) {
            const int result = checkWinCondition(day, balance);
            if (result == 1) {
                gameOverMessage = "You win!";
                gameOverIsWin = true;
            } else if (result == -1) {
                gameOverMessage = "You lose!\nOut of time — reach $" + std::to_string(kWinTargetBalance) + " by end of day "
                    + std::to_string(kWinDeadlineDay) + ".";
                gameOverIsWin = false;
            }
        }

        // Event Polling
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (!gameOverMessage.empty()) {
                        if (keyEvent->code == sf::Keyboard::Key::Enter || keyEvent->code == sf::Keyboard::Key::Escape) {
                            window.close();
                        }
                    } else if (showBuyPopup) {
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

            if (event->is<sf::Event::MouseButtonPressed>() && gameOverMessage.empty()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                MouseHandler::handleMouseClick(
                    mousePos,
                    gameRender,
                    market,
                    pendingNews,
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
            pendingNews,
            showBuyPopup,
            selectedStockIndex,
            buyShares,
            buyMessage,
            showSellPopup,
            sellSelectedSymbol,
            sellShares,
            sellMessage,
            gameOverMessage,
            gameOverIsWin
        );
    }

    return 0;
}
