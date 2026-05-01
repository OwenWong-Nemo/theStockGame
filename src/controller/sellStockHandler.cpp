#include "sellStockHandler.hpp"

#include <optional>

namespace {

[[nodiscard]] std::optional<float> findMarketPrice(const std::vector<Stock>& market, const std::string& symbol) {
    for (const Stock& stock : market) {
        if (stock.getName() == symbol) {
            return stock.getCurrentPrice();
        }
    }
    return std::nullopt;
}

}  // namespace

void SellStockHandler::tryExecute(
    const std::vector<Stock>& market,
    const std::string& symbol,
    int sellShares,
    float& balance,
    std::unordered_map<std::string, int>& portfolio,
    bool& showSellPopup,
    std::string& sellMessage
) {
    if (symbol.empty()) {
        sellMessage = "Select a holding to sell.";
        return;
    }

    const std::optional<float> maybePrice = findMarketPrice(market, symbol);
    if (!maybePrice.has_value()) {
        sellMessage = "Stock is not traded on this market.";
        return;
    }

    if (sellShares <= 0) {
        sellMessage = "Shares must be at least 1.";
        return;
    }

    const auto it = portfolio.find(symbol);
    if (it == portfolio.end() || it->second <= 0) {
        sellMessage = "No shares to sell.";
        return;
    }

    if (it->second < sellShares) {
        sellMessage = "Not enough shares to sell.";
        return;
    }

    const float proceeds = maybePrice.value() * static_cast<float>(sellShares);
    balance += proceeds;

    it->second -= sellShares;
    if (it->second <= 0) {
        portfolio.erase(it);
    }

    sellMessage = "Sale executed.";
    showSellPopup = false;
}
