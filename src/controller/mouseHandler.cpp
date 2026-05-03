#include "mouseHandler.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include "buyStockHandler.hpp"

void MouseHandler::handleMouseClick(
    sf::Vector2f mousePos,
    GameRender& gameRender,
    std::vector<Stock>& market,
    News& pendingNews,
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
) {
    if (showBuyPopup || showSellPopup) {
        return;
    }

    if (gameRender.isNextDayClicked(mousePos)) {
        pendingNews.applyToMarket(market);
        day++;
        for (auto& stock : market) {
            stock.updatePrice();
        }
        pendingNews = News::generateRandom(market);
        currentState = GameState::Market;
    }

    if (currentState == GameState::Market) {
        selectedStockIndex = gameRender.getClickedStockIndex(mousePos, static_cast<int>(market.size()));
        if (selectedStockIndex >= 0) {
            const float unitPrice = market[selectedStockIndex].getCurrentPrice();
            const int maxBuy = BuyStockHandler::maxAffordableBuyShares(balance, unitPrice);
            buyShares = (maxBuy >= 1) ? 1 : 0;
            buyMessage.clear();
            showBuyPopup = true;
        }
        return;
    }

    if (currentState == GameState::Sell && !portfolio.empty()) {
        std::vector<std::pair<std::string, int>> rows(portfolio.begin(), portfolio.end());
        std::sort(rows.begin(), rows.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        const int row = gameRender.getClickedHoldingRowIndex(mousePos, static_cast<int>(rows.size()));
        if (row >= 0 && row < static_cast<int>(rows.size())) {
            sellSelectedSymbol = rows[row].first;
            const int owned = rows[row].second;
            sellShares = (owned >= 1) ? 1 : 0;
            sellMessage.clear();
            showSellPopup = true;
        }
    }
}
