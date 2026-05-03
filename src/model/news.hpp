#pragma once

#include <string>
#include <vector>

#include "stock.hpp"

enum class NewsSentiment { Negative, Positive, Neutral };

/// A headline about one listed company. Shown in the News tab immediately; when the player
/// advances to the next day, `applyToMarket` runs first, then the usual random price walk.
class News {
public:
    News() = default;

    News(std::string title, std::string content, std::string targetStock, NewsSentiment sentiment, float impactFraction);

    [[nodiscard]] const std::string& getTitle() const { return title; }
    [[nodiscard]] const std::string& getContent() const { return content; }
    [[nodiscard]] const std::string& getTargetStockName() const { return targetStockName; }
    [[nodiscard]] NewsSentiment getSentiment() const { return sentiment; }
    /// Unsigned magnitude (e.g. 0.06 = 6% move); sign comes from `sentiment`.
    [[nodiscard]] float getImpactFraction() const { return impactFraction; }

    /// Applies this story's one-time price effect to the matching stock (if any).
    void applyToMarket(std::vector<Stock>& market) const;

    /// Picks a random company and sentiment; impact is a modest band suitable for next-day gaps.
    [[nodiscard]] static News generateRandom(const std::vector<Stock>& market);

private:
    std::string title;
    std::string content;
    std::string targetStockName;
    NewsSentiment sentiment = NewsSentiment::Neutral;
    float impactFraction = 0.f;
};
