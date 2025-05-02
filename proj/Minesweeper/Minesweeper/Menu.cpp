#include "Menu.h"
#include <sstream>
#include <stdexcept>

Menu::Menu(sf::RenderWindow& window) : windowRef(window)
{
    if (!font.openFromFile("assets/font.ttf")) {
        throw std::runtime_error("Failed to open font from 'assets/font.ttf'");
    }

    // Create and configure title
    title = sf::Text(font);
    title->setString("Minesweeper");
    title->setCharacterSize(48);

    title->setFillColor(sf::Color::White);
    title->setPosition(sf::Vector2f(50.f, 30.f));

    // Grid size label
    gridLabel = sf::Text(font);
    gridLabel->setString("");
    gridLabel->setCharacterSize(24);

    gridLabel->setFillColor(sf::Color::White);
    gridLabel->setPosition(sf::Vector2f(50.f, 120.f));
    updateGridLabel();

    // Minus button
    minusButton = sf::Text(font);
    minusButton->setString("[-]");
    minusButton->setCharacterSize(24);

    minusButton->setFillColor(sf::Color::Cyan);
    minusButton->setPosition(sf::Vector2f(50.f, 160.f));

    // Plus button
    plusButton = sf::Text(font);
    plusButton->setString("[+]");
    plusButton->setCharacterSize(24);

    plusButton->setFillColor(sf::Color::Cyan);
    plusButton->setPosition(sf::Vector2f(150.f, 160.f));

    // Start button
    startButton = sf::Text(font);
    startButton->setString("Start Game");
    startButton->setCharacterSize(28);

    startButton->setFillColor(sf::Color::Green);
    startButton->setPosition(sf::Vector2f(50.f, 220.f));
}

void Menu::updateGridLabel() {
    std::ostringstream ss;
    ss << "Grid size: " << gridSize << "x" << gridSize;
    if (gridLabel)
        gridLabel->setString(ss.str());
}

bool Menu::isMouseOver(const sf::Text& text, sf::Vector2f mousePos) const {
    return text.getGlobalBounds().contains(mousePos);
}

void Menu::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto& mouse = event.getIf<sf::Event::MouseButtonPressed>();
        sf::Vector2f mousePos = sf::Vector2f(
            static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y)
        );

        if (minusButton && isMouseOver(*minusButton, mousePos) && gridSize > minSize) {
            --gridSize;
            updateGridLabel();
        }
        else if (plusButton && isMouseOver(*plusButton, mousePos) && gridSize < maxSize) {
            ++gridSize;
            updateGridLabel();
        }
        else if (startButton && isMouseOver(*startButton, mousePos)) {
            startGame = true;
        }
    }
}


void Menu::draw() {
    if (title) windowRef.draw(*title);
    if (gridLabel) windowRef.draw(*gridLabel);
    if (minusButton) windowRef.draw(*minusButton);
    if (plusButton) windowRef.draw(*plusButton);
    if (startButton) windowRef.draw(*startButton);
}

bool Menu::shouldStartGame() const {
    return startGame;
}

unsigned int Menu::getGridWidth() const {
    return gridSize;
}

unsigned int Menu::getGridHeight() const {
    return gridSize;
}
