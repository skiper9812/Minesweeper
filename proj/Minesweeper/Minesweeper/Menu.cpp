#include "Menu.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

Menu::Menu(sf::RenderWindow& window) : windowRef(window)
{
    try {
        if (!font.openFromFile("assets/font.ttf")) {
            throw std::runtime_error("Failed to open font from 'assets/font.ttf'");
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Menu font load error: ") + e.what());
    }

    const float windowWidth = static_cast<float>(windowRef.getSize().x);

    // Title
    title = sf::Text(font, "Minesweeper", 48);
    title->setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = title->getLocalBounds();
    title->setOrigin(titleBounds.size / 2.f);
    title->setPosition({ windowWidth / 2.f, 80.f });

    // Start Game Button
    startButton = sf::Text(font, "Start Game", 32);
    startButton->setFillColor(sf::Color::Green);
    sf::FloatRect startBounds = startButton->getLocalBounds();
    startButton->setOrigin(startBounds.size / 2.f);
    startButton->setPosition({ windowWidth / 2.f, 180.f });

    // Config Button (commented for now)
    configButton = sf::Text(font, "Config", 32);
    configButton->setFillColor(sf::Color::Yellow);
    sf::FloatRect configBounds = configButton->getLocalBounds();
    configButton->setOrigin(configBounds.size / 2.f);
    configButton->setPosition(sf::Vector2f(windowWidth / 2.f, 240.f));

    // Credits Button
    creditsButton = sf::Text(font, "Credits", 32);
    creditsButton->setFillColor(sf::Color::Yellow);
    sf::FloatRect creditsBounds = creditsButton->getLocalBounds();
    creditsButton->setOrigin(creditsBounds.size / 2.f);
    creditsButton->setPosition(sf::Vector2f(windowWidth / 2.f, 300.f));

    // Exit Button
    exitButton = sf::Text(font, "Exit", 32);
    exitButton->setFillColor(sf::Color::Yellow);
    sf::FloatRect exitBounds = exitButton->getLocalBounds();
    exitButton->setOrigin(exitBounds.size / 2.f);
    exitButton->setPosition(sf::Vector2f(windowWidth / 2.f, 360.f));

    menuButtons = { &startButton, &configButton, &creditsButton, &exitButton };
    credits = std::make_unique<Credits>(windowRef, font);
    config = std::make_unique<Config>(windowRef, font);
    updateSelectionVisuals();
}

void Menu::updateSelectionVisuals() {
    for (size_t i = 0; i < menuButtons.size(); ++i) {
        if (menuButtons[i] && menuButtons[i]->has_value()) {
            auto& text = menuButtons[i]->value();

            // Set fill color based on selection
            if (i == selectedIndex) {
                text.setFillColor(sf::Color::Green);
            }
            else {
                text.setFillColor(sf::Color::Yellow);
            }

            // Always re-center the text
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f);
            text.setPosition({ windowRef.getSize().x / 2.f, text.getPosition().y });
        }
    }
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
    if(inConfig)
        config->handleEvent(event, returnToMenu);
    else if(inCredits)
        credits->handleEvent(event, returnToMenu);
    else 
    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>()->code;

        if (key == sf::Keyboard::Key::Up) {
            selectedIndex = (selectedIndex - 1 + menuButtons.size()) % menuButtons.size();
            updateSelectionVisuals();
        }
        else if (key == sf::Keyboard::Key::Down) {
            selectedIndex = (selectedIndex + 1) % menuButtons.size();
            updateSelectionVisuals();
        }
        else if (key == sf::Keyboard::Key::Enter) {
            switch (selectedIndex) {
            case 0:
                std::cout << "Start Game selected. Grid size: "
                    << config->getGridSize() << "x" << config->getGridSize() << "\n";
                gridSize = config->getGridSize();
                startGame = true;
                break;
            case 1:
                inConfig = !inConfig;
                config->setSelectedIndex(0);
                std::cout << "Config selected\n";
                break;
            case 2:
                inCredits = !inCredits;
                std::cout << (inCredits ? "Entered Credits" : "Left Credits") << "\n";
                break;
            case 3:
                std::cout << "Exit Game selected\n";
                windowRef.close();
                break;
            default:
                break;
            }
        }
    }

}

void Menu::drawGameOverMessage(std::string message) {
    // Title
    gameOverText = sf::Text(font, message, 48);
    gameOverText->setFillColor(sf::Color::Red);
    gameOverText->setStyle(sf::Text::Bold);
    sf::FloatRect gameOverBounds = gameOverText->getLocalBounds();
    gameOverText->setOrigin(gameOverBounds.size / 2.f);
    gameOverText->setPosition({ windowRef.getSize().x / 2.f, windowRef.getSize().y / 2.f - 250 });
    

    // Start Game Button
    continueText = sf::Text(font, "Press Enter to continue", 32);
    continueText->setFillColor(sf::Color::White);
    sf::FloatRect continueTextBounds = continueText->getLocalBounds();
    continueText->setOrigin(continueTextBounds.size / 2.f);
    continueText->setPosition({ windowRef.getSize().x / 2.f, windowRef.getSize().y / 2.f - 210 });

    windowRef.draw(*gameOverText);
    windowRef.draw(*continueText);
}

void Menu::drawMenuButtons() {
    //Drawing arrows next to selected option
    for (size_t i = 0; i < menuButtons.size(); ++i) {
        if (menuButtons[i] && menuButtons[i]->has_value()) {
            auto& text = menuButtons[i]->value();
            windowRef.draw(text);

            if (i == selectedIndex) {
                // Compute bounds and position for arrow drawing
                sf::FloatRect bounds = text.getLocalBounds();
                sf::Vector2f textPos = text.getPosition();
                float textWidth = bounds.size.x;
                float textHeight = bounds.size.y;

                // Create left arrow
                sf::Text leftArrow(font, "=>", text.getCharacterSize());
                leftArrow.setFillColor(sf::Color::Green);
                sf::FloatRect leftBounds = leftArrow.getLocalBounds();
                leftArrow.setOrigin(leftBounds.position + leftBounds.size / 2.f);
                leftArrow.setPosition(sf::Vector2f(textPos.x - textWidth / 2.f - 20.f, textPos.y));

                // Create right arrow
                sf::Text rightArrow(font, "<=", text.getCharacterSize());
                rightArrow.setFillColor(sf::Color::Green);
                sf::FloatRect rightBounds = rightArrow.getLocalBounds();
                rightArrow.setOrigin(rightBounds.position + rightBounds.size / 2.f);
                rightArrow.setPosition(sf::Vector2f(textPos.x + textWidth / 2.f + 20.f, textPos.y));

                windowRef.draw(leftArrow);
                windowRef.draw(rightArrow);
            }
        }
    }
}


void Menu::draw() {
    windowRef.clear();

    // Draw title if it exists
    if (title) windowRef.draw(*title);

    if (returnToMenu) {
        inCredits = false;
        inConfig = false;
        returnToMenu = false;
    }

    if (inCredits) {
        credits->draw();
    }
    else if (inConfig) {
        config->draw();
    }
    else {
        drawMenuButtons();
    }
}

void Menu::endGame() {
    startGame = false;
}

bool Menu::shouldStartGame() const {
    return startGame;
}

unsigned int Menu::getGridSize() const {
    return gridSize;
}
