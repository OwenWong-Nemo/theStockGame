#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "../model/stock.hpp"

/// Executes sell-side validation and ledger updates when the player confirms an order.
class SellStockHandler {
public:
    /// Looks up live price by symbol in `market`. Validates holdings and shares.
    /// On success: credits balance, removes shares from portfolio (erases symbol if holdings hit zero), closes popup.
    /// On failure: sets `sellMessage`, leaves `showSellPopup` true unless invalid selection resets message only.
    static void tryExecute(
        const std::vector<Stock>& market,
        const std::string& symbol,
        int sellShares,
        float& balance,
        std::unordered_map<std::string, int>& portfolio,
        bool& showSellPopup,
        std::string& sellMessage
    );
};
