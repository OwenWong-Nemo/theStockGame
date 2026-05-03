#include "news.hpp"

#include "../utils/randomHelper.hpp"

namespace {

[[nodiscard]] float priceMultiplier(NewsSentiment sentiment, float impactFraction) {
    switch (sentiment) {
        case NewsSentiment::Negative:
            return 1.f - impactFraction;
        case NewsSentiment::Positive:
            return 1.f + impactFraction;
        case NewsSentiment::Neutral:
        default:
            return 1.f;
    }
}

}  // namespace

News::News(std::string t, std::string c, std::string targetStock, NewsSentiment s, float impact)
    : title(std::move(t)),
      content(std::move(c)),
      targetStockName(std::move(targetStock)),
      sentiment(s),
      impactFraction(impact)
{
}

void News::applyToMarket(std::vector<Stock>& market) const {
    if (targetStockName.empty()) {
        return;
    }
    const float factor = priceMultiplier(sentiment, impactFraction);
    for (Stock& stock : market) {
        if (stock.getName() == targetStockName) {
            stock.applyPriceMultiplier(factor);
            return;
        }
    }
}

News News::generateRandom(const std::vector<Stock>& market) {
    if (market.empty()) {
        return {};
    }

    const int idx = RandomHelper::getInt(0, static_cast<int>(market.size()) - 1);
    const std::string& name = market[idx].getName();
    const NewsSentiment sentiment = RandomHelper::flipCoin() ? NewsSentiment::Negative : NewsSentiment::Positive;
    const float impact = RandomHelper::getFloat(0.04f, 0.12f);

    if (sentiment == NewsSentiment::Negative) {
        return News(
            "Breaking: " + name,
            "Regulatory pressure mounts and analysts cut outlook. Markets expect a tough quarter ahead.",
            name,
            NewsSentiment::Negative,
            impact
        );
    }

    return News(
        "Spotlight: " + name,
        "Strong demand and a major contract win lift sentiment. Traders price in upside for the session.",
        name,
        NewsSentiment::Positive,
        impact
    );
}
