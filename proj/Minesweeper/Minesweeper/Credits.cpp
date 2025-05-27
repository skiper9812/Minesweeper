#include "Credits.h"
#include <iostream>

Credits::Credits(sf::RenderWindow& window, const sf::Font& font) : windowRef(window)
{
    creditsText = sf::Text(font, "Created by Piotr Katniak\n\nPress Enter to return", 24);
    creditsText->setFillColor(sf::Color::White);

    sf::FloatRect bounds = creditsText->getLocalBounds();
    creditsText->setOrigin(bounds.size / 2.f);
    creditsText->setPosition({windowRef.getSize().x / 2.f,windowRef.getSize().y / 2.f - 80.f });
}

void Credits::handleEvent(const sf::Event& event, bool& returnToMenu) {
    if (event.is<sf::Event::KeyPressed>() && event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter) {
        returnToMenu = true;
    }
}

void Credits::draw() {
    if (creditsText.has_value()) {
        windowRef.draw(creditsText.value());
    }
}
