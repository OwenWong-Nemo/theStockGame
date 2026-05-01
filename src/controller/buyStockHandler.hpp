#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "../model/stock.hpp"

/// Executes buy-side validation and ledger updates when the player confirms an order.
class BuyStockHandler {
public:
    /// Largest whole-share count affordable at current balance (0 if unaffordable or invalid price).
    [[nodiscard]] static int maxAffordableBuyShares(float balance, float unitPrice);

    /// Validates selected index and share count vs balance, applies the trade if valid.
    /// On success: debits balance, credits portfolio, clears `buyMessage` to caller via order path (see below), closes popup (`showBuyPopup = false`).
    /// On validation failure: sets `buyMessage` and leaves `showBuyPopup` true.
    static void tryExecute(
        const std::vector<Stock>& market,
        int selectedStockIndex,
        int buyShares,
        float& balance,
        std::unordered_map<std::string, int>& portfolio,
        bool& showBuyPopup,
        std::string& buyMessage
    );
};
