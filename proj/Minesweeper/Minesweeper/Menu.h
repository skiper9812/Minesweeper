#pragma once
#include <SFML/Graphics.hpp>
#include "Credits.h"
#include "Config.h"
#include <string>
#include <optional>

class Menu {
public:
    Menu(sf::RenderWindow& window); // No font passed
    void handleEvent(const sf::Event& event);
    void updateSelectionVisuals();
    void drawMenuButtons();
    void drawGameOverMessage(std::string message);
    void draw();
    void endGame();

    bool shouldStartGame() const;
    unsigned int getGridSize() const;
    int selectedIndex = 0; // Index of the currently selected button

private:
    void updateGridLabel();
    bool isMouseOver(const sf::Text& text, sf::Vector2f mousePos) const;

    sf::RenderWindow& windowRef;
    sf::Font font;

    std::optional<sf::Text> title;
    std::optional<sf::Text> startButton;
    std::optional<sf::Text> configButton;
    std::optional<sf::Text> creditsButton;
    std::optional<sf::Text> exitButton;
    std::optional<sf::Text> gridLabel;
    std::optional<sf::Text> gameOverText;
    std::optional<sf::Text> continueText;

    std::unique_ptr<Credits> credits;
    std::unique_ptr<Config> config;
    std::vector<std::optional<sf::Text>*> menuButtons; // vector of pointers to the optionals

    unsigned int gridSize = 7;

    bool startGame = false;
    bool inCredits = false;
    bool inConfig = false;
    bool returnToMenu = false;
    
};
