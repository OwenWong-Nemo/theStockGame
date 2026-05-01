#pragma once

#include <SFML/Window/Keyboard.hpp>

/// Central place for physical key choices and “what does this key mean?” checks.
/// Add new binds here; gameplay code should call the predicates instead of comparing raw keys.
namespace Keybinds {

// --- Tab / screen shortcuts (toggle with same key returns to Market elsewhere) ---
inline constexpr sf::Keyboard::Key kNewsTab = sf::Keyboard::Key::N;
inline constexpr sf::Keyboard::Key kPortfolioTab = sf::Keyboard::Key::P;
inline constexpr sf::Keyboard::Key kSellTab = sf::Keyboard::Key::S;

[[nodiscard]] inline bool matchesNewsTab(sf::Keyboard::Key key) { return key == kNewsTab; }
[[nodiscard]] inline bool matchesPortfolioTab(sf::Keyboard::Key key) { return key == kPortfolioTab; }
[[nodiscard]] inline bool matchesSellTab(sf::Keyboard::Key key) { return key == kSellTab; }

// --- Shared UI / order popup ---
[[nodiscard]] inline bool matchesCancel(sf::Keyboard::Key key) { return key == sf::Keyboard::Key::Escape; }

[[nodiscard]] inline bool matchesConfirm(sf::Keyboard::Key key) { return key == sf::Keyboard::Key::Enter; }

/// Increase quantity (buy/sell popups, etc.).
[[nodiscard]] inline bool matchesIncreaseAmount(sf::Keyboard::Key key)
{
    return key == sf::Keyboard::Key::Up || key == sf::Keyboard::Key::Add || key == sf::Keyboard::Key::Equal;
}

/// Decrease quantity (buy/sell popups, etc.).
[[nodiscard]] inline bool matchesDecreaseAmount(sf::Keyboard::Key key)
{
    return key == sf::Keyboard::Key::Down || key == sf::Keyboard::Key::Hyphen || key == sf::Keyboard::Key::Subtract;
}

}  // namespace Keybinds
