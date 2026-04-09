#include <SFML/Graphics.hpp>
#include <optional>

// Render graphics
int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Stock Game UI Test");

    // Stock Board UI Element
    sf::RectangleShape stockBoard({300.f, 200.f});
    stockBoard.setFillColor(sf::Color(200, 200, 200)); //
    stockBoard.setPosition({250.f, 200.f}); // Centered on screen

    // // 1. Create a "Buy Button"
    // sf::RectangleShape buyButton({150.f, 50.f});
    // buyButton.setFillColor(sf::Color::Green);
    // buyButton.setPosition({50.f, 50.f}); // Top left area

    // // 2. Create a "Stock Indicator"
    // sf::CircleShape indicator(10.f);
    // indicator.setFillColor(sf::Color::Red);
    // indicator.setOrigin({10.f, 10.f}); // Center it
    // indicator.setPosition({400.f, 300.f}); // Dead center of screen

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White); 
        
        // window.draw(buyButton);
        // window.draw(indicator);

        window.draw(stockBoard);
        
        window.display();
    }
}