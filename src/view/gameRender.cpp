#include "gameRender.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "newsRenderer.hpp"
#include "stockRenderer.hpp"

GameRender::GameRender(const sf::Font& loadedFont):
      font(loadedFont),
      header({780.f, 60.f}),
      btnNextDay({120.f, 40.f}),
      mainArea({780.f, 400.f}),
      btnNews({250.f, 80.f}),
      btnPortfolio({250.f, 80.f}),
      btnSell({250.f, 80.f}),
      headerText(font),
      nextDayLabel(font, "NEXT DAY"),
      newsLabel(font, "NEWS", 20),
      portLabel(font, "PORTFOLIO", 20),
      sellLabel(font, "SELL", 20)
{
    header.setFillColor(sf::Color(35, 35, 35));
    header.setPosition({10.f, 10.f});

    headerText.setCharacterSize(20);
    headerText.setPosition({25.f, 25.f});
    headerText.setFillColor(sf::Color::White);

    btnNextDay.setFillColor(sf::Color(0, 120, 0));
    btnNextDay.setPosition({650.f, 20.f});

    nextDayLabel.setCharacterSize(14);
    nextDayLabel.setPosition({670.f, 32.f});

    mainArea.setFillColor(sf::Color(20, 20, 20));
    mainArea.setPosition({10.f, 80.f});

    btnNews.setPosition({10.f, 500.f});
    btnPortfolio.setPosition({275.f, 500.f});
    btnSell.setPosition({540.f, 500.f});

    btnNews.setFillColor(sf::Color(0, 50, 150));
    btnPortfolio.setFillColor(sf::Color(100, 0, 150));
    btnSell.setFillColor(sf::Color(150, 0, 0));

    newsLabel.setPosition({105.f, 525.f});
    portLabel.setPosition({340.f, 525.f});
    sellLabel.setPosition({640.f, 525.f});
}

void GameRender::drawFrame(
    sf::RenderWindow& window,
    GameState currentState,
    int day,
    float balance,
    const std::vector<Stock>& market,
    const std::unordered_map<std::string, int>& portfolio,
    const News& pendingNews,
    bool showBuyPopup,
    int selectedStockIndex,
    int buyShares,
    const std::string& buyMessage,
    bool showSellPopup,
    const std::string& sellSelectedSymbol,
    int sellShares,
    const std::string& sellMessage,
    const std::string& gameOverMessage,
    bool gameOverIsWin
)
{
    headerText.setString("Day: " + std::to_string(day) + "  |  Balance: $" + std::to_string(static_cast<int>(balance)));

    window.clear(sf::Color::Black);

    window.draw(header);
    window.draw(headerText);
    window.draw(btnNextDay);
    window.draw(nextDayLabel);
    window.draw(mainArea);

    if (currentState == GameState::Market) {
        for (int i = 0; i < static_cast<int>(market.size()); ++i) {
            StockRenderer::drawStockRow(window, font, market[i], i);
        }
    } else if (currentState == GameState::Portfolio) {
        sf::Text title(font, "My Portfolio", 28);
        title.setPosition({40.f, 100.f});
        window.draw(title);

        float y = 150.f;
        if (portfolio.empty()) {
            sf::Text empty(font, "No holdings yet.", 22);
            empty.setPosition({40.f, y});
            window.draw(empty);
        } else {
            std::vector<std::pair<std::string, int>> rows(portfolio.begin(), portfolio.end());
            std::sort(rows.begin(), rows.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
            for (const auto& row : rows) {
                sf::Text line(font, row.first + " : " + std::to_string(row.second) + " shares", 22);
                line.setPosition({40.f, y});
                window.draw(line);
                y += 35.f;
            }
        }
    } else if (currentState == GameState::Sell) {
        sf::Text title(font, "Sell", 28);
        title.setPosition({40.f, 100.f});
        window.draw(title);

        sf::Text subtitle(font, "Click a holding to place a sell order.", 18);
        subtitle.setFillColor(sf::Color(190, 190, 190));
        subtitle.setPosition({40.f, 135.f});
        window.draw(subtitle);

        float y = 150.f;
        if (portfolio.empty()) {
            sf::Text empty(font, "Nothing to sell", 22);
            empty.setPosition({40.f, y});
            window.draw(empty);
        } else {
            std::vector<std::pair<std::string, int>> rows(portfolio.begin(), portfolio.end());
            std::sort(rows.begin(), rows.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
            for (const auto& row : rows) {
                sf::Text line(font, row.first + " : " + std::to_string(row.second) + " shares", 22);
                line.setPosition({40.f, y});
                window.draw(line);
                y += 35.f;
            }
        }
    } else if (currentState == GameState::News) {
        NewsRenderer::draw(window, font, pendingNews);
    } else {
        sf::Text msg(font, getStateName(currentState), 30);
        msg.setPosition({300.f, 250.f});
        window.draw(msg);
    }

    if (showBuyPopup && selectedStockIndex >= 0 && selectedStockIndex < static_cast<int>(market.size())) {
        sf::RectangleShape overlay({800.f, 600.f});
        overlay.setFillColor(sf::Color(0, 0, 0, 140));
        window.draw(overlay);

        sf::RectangleShape popup({520.f, 260.f});
        popup.setFillColor(sf::Color(30, 30, 30));
        popup.setOutlineThickness(2.f);
        popup.setOutlineColor(sf::Color::White);
        popup.setPosition({140.f, 170.f});
        window.draw(popup);

        sf::Text title(font, "Buy Order", 28);
        title.setPosition({170.f, 190.f});
        window.draw(title);

        const Stock& selectedStock = market[selectedStockIndex];
        const float unitPrice = selectedStock.getCurrentPrice();
        const float totalCost = unitPrice * static_cast<float>(buyShares);

        std::ostringstream unitPriceStr;
        unitPriceStr << std::fixed << std::setprecision(2) << unitPrice;
        std::ostringstream totalCostStr;
        totalCostStr << std::fixed << std::setprecision(2) << totalCost;

        sf::Text details(
            font,
            "Stock: " + selectedStock.getName() + "\n"
            "Price: $" + unitPriceStr.str() + " each\n"
            "Shares: " + std::to_string(buyShares) + "\n"
            "Total: $" + totalCostStr.str(),
            22
        );
        details.setPosition({170.f, 240.f});
        window.draw(details);

        sf::Text hint(font, "UP/DOWN change shares, ENTER confirm, ESC cancel", 15);
        hint.setPosition({170.f, 360.f});
        hint.setFillColor(sf::Color(200, 200, 200));
        window.draw(hint);

        if (!buyMessage.empty()) {
            sf::Text msg(font, buyMessage, 18);
            msg.setPosition({170.f, 390.f});
            msg.setFillColor(sf::Color::Red);
            window.draw(msg);
        }
    }

    if (showSellPopup && !sellSelectedSymbol.empty()) {
        float unitPrice = 0.f;
        for (const Stock& s : market) {
            if (s.getName() == sellSelectedSymbol) {
                unitPrice = s.getCurrentPrice();
                break;
            }
        }

        const auto holdingIt = portfolio.find(sellSelectedSymbol);
        const int owned = (holdingIt != portfolio.end()) ? holdingIt->second : 0;
        const float proceeds = unitPrice * static_cast<float>(sellShares);

        sf::RectangleShape overlay({800.f, 600.f});
        overlay.setFillColor(sf::Color(0, 0, 0, 140));
        window.draw(overlay);

        sf::RectangleShape popup({520.f, 260.f});
        popup.setFillColor(sf::Color(30, 30, 30));
        popup.setOutlineThickness(2.f);
        popup.setOutlineColor(sf::Color::White);
        popup.setPosition({140.f, 170.f});
        window.draw(popup);

        sf::Text popupTitle(font, "Sell Order", 28);
        popupTitle.setPosition({170.f, 190.f});
        window.draw(popupTitle);

        std::ostringstream unitPriceStr;
        unitPriceStr << std::fixed << std::setprecision(2) << unitPrice;
        std::ostringstream proceedsStr;
        proceedsStr << std::fixed << std::setprecision(2) << proceeds;

        sf::Text details(
            font,
            "Stock: " + sellSelectedSymbol + "\n"
            "Owned: " + std::to_string(owned) + " shares\n"
            "Price: $" + unitPriceStr.str() + " each\n"
            "Shares to sell: " + std::to_string(sellShares) + "\n"
            "Est. proceeds: $" + proceedsStr.str(),
            22
        );
        details.setPosition({170.f, 240.f});
        window.draw(details);

        sf::Text hint(font, "UP/DOWN change quantity, ENTER confirm, ESC cancel", 18);
        hint.setPosition({170.f, 360.f});
        hint.setFillColor(sf::Color(200, 200, 200));
        window.draw(hint);

        if (!sellMessage.empty()) {
            sf::Text msg(font, sellMessage, 18);
            msg.setPosition({170.f, 390.f});
            msg.setFillColor(sf::Color::Red);
            window.draw(msg);
        }
    }

    sf::Text tabHint(font, "Tabs: N News   P Portfolio   S Sell (press again for Market)", 14);
    tabHint.setFillColor(sf::Color(150, 150, 150));
    tabHint.setPosition({10.f, 472.f});
    window.draw(tabHint);

    window.draw(btnNews);
    window.draw(newsLabel);
    window.draw(btnPortfolio);
    window.draw(portLabel);
    window.draw(btnSell);
    window.draw(sellLabel);

    if (!gameOverMessage.empty()) {
        sf::RectangleShape overlay({800.f, 600.f});
        overlay.setFillColor(sf::Color(0, 0, 0, 210));
        window.draw(overlay);

        sf::RectangleShape panel({600.f, 280.f});
        panel.setFillColor(sf::Color(35, 35, 45));
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(sf::Color::White);
        panel.setPosition({100.f, 160.f});
        window.draw(panel);

        const unsigned int titleSize = gameOverIsWin ? 40u : 28u;
        sf::Text title(font, gameOverMessage, titleSize);
        title.setFillColor(gameOverIsWin ? sf::Color(100, 255, 140) : sf::Color(255, 140, 140));
        const sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin({tb.position.x + tb.size.x * 0.5f, tb.position.y + tb.size.y * 0.5f});
        title.setPosition({400.f, 270.f});
        window.draw(title);

        sf::Text hint(font, "Press ENTER or ESC to close", 20u);
        hint.setFillColor(sf::Color(200, 200, 200));
        const sf::FloatRect hb = hint.getLocalBounds();
        hint.setOrigin({hb.position.x + hb.size.x * 0.5f, hb.position.y + hb.size.y * 0.5f});
        hint.setPosition({400.f, 380.f});
        window.draw(hint);
    }

    window.display();
}

bool GameRender::isNextDayClicked(sf::Vector2f mousePos) const
{
    return btnNextDay.getGlobalBounds().contains(mousePos);
}

int GameRender::getClickedStockIndex(sf::Vector2f mousePos, int stockCount) const
{
    for (int i = 0; i < stockCount; ++i) {
        if (StockRenderer::isClicked(mousePos, i)) {
            return i;
        }
    }
    return -1;
}

int GameRender::getClickedHoldingRowIndex(sf::Vector2f mousePos, int rowCount) const
{
    for (int i = 0; i < rowCount; ++i) {
        const float y = 150.f + static_cast<float>(i) * 35.f;
        const sf::FloatRect row{{40.f, y}, {740.f, 32.f}};
        if (row.contains(mousePos)) {
            return i;
        }
    }
    return -1;
}

std::string GameRender::getStateName(GameState currentState) const
{
    if (currentState == GameState::News) {
        return "News Feed";
    }
    if (currentState == GameState::Sell) {
        return "Sell Center";
    }
    return "View";
}
