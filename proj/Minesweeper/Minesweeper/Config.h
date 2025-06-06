#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

class Config {
public:
    Config(sf::RenderWindow& window, const sf::Font& font);

    void handleEvent(const sf::Event& event, bool& returnToMenu);
    void setSelectedIndex(int id);
    void draw();
    int getGridSize() const;

private:
    void updateGridText();
    void updateSelectionVisuals();
    void drawArrows(const sf::Text& targetText, float yOffset);
    void saveToFile(const std::string& filename = "config.txt") const;
    void loadFromFile(const std::string& filename = "config.txt");

    sf::RenderWindow& windowRef;
    const sf::Font* fontRef = nullptr;

    std::optional<sf::Text> gridLabel;
    std::optional<sf::Text> gridText;
    std::optional<sf::Text> returnButton;

    int defaultGridSize = 7, gridSize = defaultGridSize;
    const int minSize = 5;
    const int maxSize = 10;
    int selectedIndex = 0;
};
