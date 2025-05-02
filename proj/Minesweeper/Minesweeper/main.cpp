#include <SFML/Graphics.hpp>
#include "GameLogic.h"
#include "Menu.h" // Include your menu header
#include <iostream>

int main()
{
    // 1) Create window early so Menu and Game can both use it
    sf::RenderWindow window(
        sf::VideoMode({ 800, 600 }),
        "Minesweeper");

    try {
        // 2) Show menu
        Menu menu(window);

        while (window.isOpen() && !menu.shouldStartGame()) {
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();

                menu.handleEvent(*event);
            }

            window.clear();
            menu.draw();
            window.display();
        }

        if (!window.isOpen()) return 0; // User closed window

        // --- Game initialization would go here ---

        /*
        const int cols = menu.getGridWidth();
        const int rows = menu.getGridHeight();
        const unsigned mineCount = 40; // or adjust based on size
        const int tileSize = 32;

        sf::Texture tileset;
        if (!tileset.loadFromFile("assets/tileset.png"))
            return -1;

        Minesweeper::Game game;
        game.initialize(cols, rows, mineCount);

        sf::Sprite sprite(tileset);

        // Main game loop...
        */

    }
    catch (const std::exception& e) {
        // Error loading font or other assets
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
