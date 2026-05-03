#pragma once

#include <SFML/Graphics.hpp>

#include "../model/news.hpp"

/// Renders the News tab: wrapped text and layout sized for the 800×600 game view.
class NewsRenderer {
public:
    static void draw(sf::RenderWindow& window, const sf::Font& font, const News& pendingNews);
};
