#include "newsRenderer.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace {

constexpr float kMarginX = 40.f;
constexpr float kContentWidth = 800.f - 2.f * kMarginX;
constexpr float kYStart = 100.f;
constexpr float kYMax = 465.f;

float textWidth(const sf::Font& font, unsigned int charSize, const std::string& s) {
    if (s.empty()) {
        return 0.f;
    }
    const sf::Text t(font, s, charSize);
    return t.getLocalBounds().size.x;
}

/// Greedy word wrap; overlong words are split to fit `maxWidth`.
std::vector<std::string> wrapText(
    const sf::Font& font, unsigned int charSize, const std::string& text, float maxWidth
) {
    std::vector<std::string> lines;
    if (text.empty() || maxWidth <= 0.f) {
        return lines;
    }

    std::string line;
    std::istringstream in(text);
    std::string word;

    auto flushLine = [&]() {
        if (!line.empty()) {
            lines.push_back(std::move(line));
            line.clear();
        }
    };

    auto appendHardBrokenWord = [&](std::string w) {
        while (!w.empty()) {
            if (textWidth(font, charSize, w) <= maxWidth) {
                line = w;
                return;
            }
            size_t n = w.size();
            while (n > 1 && textWidth(font, charSize, w.substr(0, n)) > maxWidth) {
                --n;
            }
            if (n == 0) {
                n = 1;
            }
            lines.push_back(w.substr(0, n));
            w.erase(0, n);
        }
    };

    while (in >> word) {
        const std::string trial = line.empty() ? word : line + " " + word;
        if (textWidth(font, charSize, trial) <= maxWidth) {
            line = trial;
            continue;
        }
        if (!line.empty()) {
            flushLine();
        }
        if (textWidth(font, charSize, word) <= maxWidth) {
            line = word;
        } else {
            appendHardBrokenWord(std::move(word));
        }
    }
    flushLine();
    return lines;
}

void drawTextLines(
    sf::RenderWindow& window,
    const sf::Font& font,
    const std::vector<std::string>& lines,
    float x,
    float& y,
    unsigned int charSize,
    const sf::Color& color,
    float lineStep
) {
    for (const std::string& ln : lines) {
        sf::Text text(font, ln, charSize);
        text.setFillColor(color);
        text.setPosition({x, y});
        window.draw(text);
        y += lineStep;
    }
}

}  // namespace

void NewsRenderer::draw(sf::RenderWindow& window, const sf::Font& font, const News& pendingNews) {
    float y = kYStart;

    sf::Text screenTitle(font, "News Feed", 28u);
    screenTitle.setPosition({kMarginX, y});
    window.draw(screenTitle);
    y += 38.f;

    if (pendingNews.getTargetStockName().empty()) {
        sf::Text empty(font, "No headlines yet.", 22u);
        empty.setPosition({kMarginX, y});
        window.draw(empty);
        return;
    }

    std::string tone;
    switch (pendingNews.getSentiment()) {
        case NewsSentiment::Negative:
            tone = "Negative";
            break;
        case NewsSentiment::Positive:
            tone = "Positive";
            break;
        case NewsSentiment::Neutral:
        default:
            tone = "Neutral";
            break;
    }

    const sf::Color headlineColor = pendingNews.getSentiment() == NewsSentiment::Negative
        ? sf::Color(255, 120, 120)
        : (pendingNews.getSentiment() == NewsSentiment::Positive ? sf::Color(120, 255, 160) : sf::Color::White);

    const std::vector<std::string> headlineLines = wrapText(font, 24u, pendingNews.getTitle(), kContentWidth);
    drawTextLines(window, font, headlineLines, kMarginX, y, 24u, headlineColor, 28.f);
    y += 4.f;

    std::ostringstream impactStr;
    impactStr << std::fixed << std::setprecision(1) << (pendingNews.getImpactFraction() * 100.f);
    const std::string metaString = "Company: " + pendingNews.getTargetStockName() + "  |  Sentiment: " + tone
        + "  |  Next-day gap: ~" + impactStr.str() + "%";
    const std::vector<std::string> metaLines = wrapText(font, 16u, metaString, kContentWidth);
    drawTextLines(window, font, metaLines, kMarginX, y, 16u, sf::Color(200, 200, 200), 20.f);
    y += 8.f;

    const std::string& bodyText = pendingNews.getContent();
    constexpr float kFootnoteGap = 8.f;
    constexpr unsigned int kFootnoteCharSize = 16;
    const float footnoteHeight = 20.f;

    unsigned int bodySize = 20u;
    std::vector<std::string> bodyLines;
    float bodyLineStep = 0.f;

    for (; bodySize >= 12u; --bodySize) {
        bodyLineStep = static_cast<float>(bodySize) * 1.28f;
        bodyLines = wrapText(font, bodySize, bodyText, kContentWidth);
        const float bodyHeight = static_cast<float>(bodyLines.size()) * bodyLineStep;
        const float bottom = y + bodyHeight + kFootnoteGap + footnoteHeight;
        if (bottom <= kYMax) {
            break;
        }
    }

    drawTextLines(window, font, bodyLines, kMarginX, y, bodySize, sf::Color::White, bodyLineStep);

    y += kFootnoteGap;
    if (y <= kYMax) {
        sf::Text footnote(font, "Effect applies when you press NEXT DAY, before the usual market move.", kFootnoteCharSize);
        footnote.setFillColor(sf::Color(160, 160, 160));
        footnote.setPosition({kMarginX, std::min(y, kYMax - 18.f)});
        window.draw(footnote);
    }
}
