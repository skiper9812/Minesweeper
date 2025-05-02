#include <SFML/Graphics.hpp>
#include <optional>  // for std::optional

int main()
{
    // 1) Create the window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML 3.0 Test");

    // 2) Main loop
    while (window.isOpen())
    {
        // 3) Poll events (std::optional<sf::Event>)
        while (auto event = window.pollEvent())
        {
            // 4) Check for close event
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 5) Clear and display
        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
