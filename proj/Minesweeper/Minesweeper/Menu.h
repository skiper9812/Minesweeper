#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

class Menu {
public:
    Menu(sf::RenderWindow& window); // No font passed
    void handleEvent(const sf::Event& event);
    void draw();

    bool shouldStartGame() const;
    unsigned int getGridWidth() const;
    unsigned int getGridHeight() const;

private:
    void updateGridLabel();
    bool isMouseOver(const sf::Text& text, sf::Vector2f mousePos) const;

    sf::RenderWindow& windowRef;
    sf::Font font;

    // Use std::optional instead of smart pointers
    std::optional<sf::Text> title;
    std::optional<sf::Text> gridLabel;
    std::optional<sf::Text> minusButton;
    std::optional<sf::Text> plusButton;
    std::optional<sf::Text> startButton;

    unsigned int gridSize = 16;
    const unsigned int minSize = 8;
    const unsigned int maxSize = 32;

    bool startGame = false;
};
