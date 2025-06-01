#include <SFML/Graphics.hpp>
#include "GameLogic.h"
#include "Menu.h"
#include <iostream>

int main()
{
    // 1) Create window for both Menu and Game
    sf::RenderWindow window(
        sf::VideoMode({ 800, 600 }),
        "Minesweeper",
        sf::Style::Titlebar | sf::Style::Close);

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

        if (!window.isOpen()) return 0;

        // 3) Initialize game
        const unsigned int size = menu.getGridSize();
        const int tileSize = 32;

        sf::Texture tileset;
        if (!tileset.loadFromFile("assets/tileset.png")) {
            std::cerr << "Failed to load tileset.png\n";
            return -1;
        }

        Minesweeper::Game game;
        game.initialize(size);
        sf::Sprite tileSprite(tileset);

        // 4) Game loop
        while (window.isOpen()) {
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (!game.isGameOver()) {
                    if (event->is<sf::Event::MouseButtonPressed>()) {
                        auto mouse = event->getIf<sf::Event::MouseButtonPressed>();
                        int MouseX = mouse->position.x / tileSize;
                        int MouseY = mouse->position.y / tileSize;

                        if (mouse->button == sf::Mouse::Button::Left) {
                            bool safe = game.reveal(MouseX, MouseY);
                            if (!safe)
                                std::cout << "You hit a mine!\n";

                            std::cout << "Clicked Left\n" << safe;
                        }
                        else if (mouse->button == sf::Mouse::Button::Right) {
                            game.toggleFlag(MouseX, MouseY);
                            std::cout << "Clicked Right\n";
                        }

                        if (game.checkWin()) {
                            std::cout << "You won!\n";
                        }
                    }
                }
            }

            window.clear();

            // Calculate centered position for the grid
            sf::Vector2u windowSize = window.getSize();
            int gridPixelWidth = size * tileSize;
            int gridPixelHeight = size * tileSize;

            float offsetX = (windowSize.x - gridPixelWidth) / 2.f;
            float offsetY = (windowSize.y - gridPixelHeight) / 2.f;


            // 5) Draw game grid
            const auto& grid = game.getGrid();
            for (int y = 0; y < size; ++y) {
                for (int x = 0; x < size; ++x) {
                    const auto& cell = grid[y][x];

                    int tileIndex = 0; // Default: hidden
                    if (cell.state == Minesweeper::CellState::Revealed) {
                        if (cell.hasMine)
                            tileIndex = 3; //Mine
                        else
                            tileIndex = 4 + cell.adjacentMines; //Numbers 0-8
                    }
                    else if (cell.state == Minesweeper::CellState::Flagged) {
                        tileIndex = 1; //Flag
                    }
                    else if (cell.state == Minesweeper::CellState::Questioned) {
                        tileIndex = 2; //Question mark
                    }

                    tileSprite.setTextureRect(sf::IntRect({ tileIndex * tileSize, 0 }, { tileSize, tileSize }));
                    tileSprite.setPosition({
                        offsetX + static_cast<float>(x * tileSize),
                        offsetY + static_cast<float>(y * tileSize)
                        });
                    window.draw(tileSprite);
                }
            }

            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
