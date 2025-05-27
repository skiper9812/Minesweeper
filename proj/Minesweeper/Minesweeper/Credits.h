#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Credits {
public:
    Credits(sf::RenderWindow& window, const sf::Font& font);

    void handleEvent(const sf::Event& event, bool& returnToMenu);
    void draw();

private:
    sf::RenderWindow& windowRef;
    std::optional<sf::Text> creditsText;
};