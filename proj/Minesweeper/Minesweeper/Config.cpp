#include "Config.h"
#include <iostream>
#include <sstream>
#include <fstream>

void Config::saveToFile(const std::string& filename) const {
    try {
        std::ofstream file(filename);
        if (!file) {
            throw std::ios_base::failure("Failed to open file for writing.");
        }

        file << "gridSize:" << gridSize << ";\n";

        if (!file) {
            throw std::ios_base::failure("Failed to write to file.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving config: " << e.what() << std::endl;
    }
}


void Config::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file) {
            throw std::ios_base::failure("Config file not found.");
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key, valueStr;

            if (std::getline(iss, key, ':') && std::getline(iss, valueStr, ';')) {
                if (key == "gridSize") {
                    int value = std::stoi(valueStr);
                    if (value >= minSize && value <= maxSize) {
                        gridSize = value;
                    }
                    else {
                        throw std::runtime_error("Grid size out of valid range.");
                    }
                }
            }
        }

        updateGridText();
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        gridSize = defaultGridSize; // Fallback default
        updateGridText();
    }
}

Config::Config(sf::RenderWindow& window, const sf::Font& font)
    : windowRef(window), fontRef(&font), selectedIndex(0)
{
    loadFromFile();

    // Label
    gridLabel = sf::Text(font, "Choose grid size", 32);
    gridLabel->setFillColor(sf::Color::White);
    sf::FloatRect glBounds = gridLabel->getLocalBounds();
    gridLabel->setOrigin(glBounds.position + glBounds.size / 2.f);
    gridLabel->setPosition({ windowRef.getSize().x / 2.f, 180.f });

    // Grid text
    gridText = sf::Text(font, "", 36);
    gridText->setFillColor(sf::Color::Yellow);
    updateGridText();

    // Return button
    returnButton = sf::Text(font, "Return to Menu", 32);
    returnButton->setFillColor(sf::Color::Yellow);
    sf::FloatRect rBounds = returnButton->getLocalBounds();
    returnButton->setOrigin(rBounds.position + rBounds.size / 2.f);
    returnButton->setPosition({ windowRef.getSize().x / 2.f, 320.f });

    updateSelectionVisuals();
}

void Config::updateGridText() {
    if (gridText) {
        saveToFile();

        std::ostringstream ss;
        ss << gridSize << "x" << gridSize;
        gridText->setString(ss.str());

        sf::FloatRect bounds = gridText->getLocalBounds();
        gridText->setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        gridText->setPosition({ windowRef.getSize().x / 2.f, 240.f });
    }
}

void Config::updateSelectionVisuals() {
    if (gridText)
        gridText->setFillColor(selectedIndex == 0 ? sf::Color::Green : sf::Color::Yellow);

    if (returnButton)
        returnButton->setFillColor(selectedIndex == 1 ? sf::Color::Green : sf::Color::Yellow);
}

void Config::handleEvent(const sf::Event& event, bool& returnToMenu) {
    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>()->code;

        if (key == sf::Keyboard::Key::Up) {
            selectedIndex = (selectedIndex - 1 + 2) % 2;
            updateSelectionVisuals();
        }
        else if (key == sf::Keyboard::Key::Down) {
            selectedIndex = (selectedIndex + 1) % 2;
            updateSelectionVisuals();
        }
        else if (key == sf::Keyboard::Key::Left && selectedIndex == 0 && gridSize > minSize) {
            gridSize--;
            updateGridText();
        }
        else if (key == sf::Keyboard::Key::Right && selectedIndex == 0 && gridSize < maxSize) {
            gridSize++;
            updateGridText();
        }
        else if (key == sf::Keyboard::Key::Enter && selectedIndex == 1) {
            returnToMenu = true;
        }
    }
}

void Config::drawArrows(const sf::Text& targetText, float yOffset) {
    const float spacing = 30.f;
    sf::FloatRect bounds = targetText.getLocalBounds();
    sf::Vector2f pos = targetText.getPosition();

    // Left arrow
    sf::Text leftArrow(*fontRef, "=>", targetText.getCharacterSize());
    leftArrow.setFillColor(sf::Color::Green);
    sf::FloatRect lBounds = leftArrow.getLocalBounds();
    leftArrow.setOrigin({lBounds.size.x / 2.f, lBounds.size.y / 2.f});
    leftArrow.setPosition({ pos.x - bounds.size.x / 2.f - spacing, pos.y + yOffset });

    // Right arrow
    sf::Text rightArrow(*fontRef, "<=", targetText.getCharacterSize());
    rightArrow.setFillColor(sf::Color::Green);
    sf::FloatRect rBounds = rightArrow.getLocalBounds();
    rightArrow.setOrigin({ rBounds.size.x / 2.f, rBounds.size.y / 2.f });
    rightArrow.setPosition({ pos.x + bounds.size.x / 2.f + spacing, pos.y + yOffset });

    windowRef.draw(leftArrow);
    windowRef.draw(rightArrow);
}

void Config::draw() {
    if (gridLabel) windowRef.draw(gridLabel.value());

    if (gridText) {
        const auto& text = gridText.value();
        windowRef.draw(text);

        if (selectedIndex == 0 && fontRef) {
            drawArrows(text, 0.f);
        }
    }

    if (returnButton) {
        const auto& text = returnButton.value();
        windowRef.draw(text);

        if (selectedIndex == 1 && fontRef) {
            drawArrows(text, -12.f);
        }
    }
}

int Config::getGridSize() const {
    return gridSize;
}

void Config::setSelectedIndex(int id) {
    selectedIndex = id;
    updateSelectionVisuals();
}
