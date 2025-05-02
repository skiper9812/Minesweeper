#include <SFML/Graphics.hpp>
#include "GameLogic.h"

int main()
{
    const int cols = 16;
    const int rows = 16;
    const unsigned mineCount = 40;
    const int tileSize = 32;

    // 1) Load tileset
    sf::Texture tileset;
    if (!tileset.loadFromFile("assets/tileset.png"))
        return -1;

    // 2) Init game logic
    Minesweeper::Game game;
    game.initialize(cols, rows, mineCount);

    // 3) Create window
    sf::RenderWindow window(
        sf::VideoMode({ cols * tileSize, rows * tileSize }),
        "Minesweeper");

    sf::Sprite sprite(tileset);

    // 4) Main loop
    while (window.isOpen())
    {
        // — SFML 3.0 event loop
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // 2) Mouse button pressed
            if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // MouseButtonEvent has .button, .x, .y :contentReference[oaicite:0]{index=0}
                int x = mouse->position.x / tileSize;
                int y = mouse->position.y / tileSize;

                if (x >= 0 && x < cols && y >= 0 && y < rows)
                {
                    if (mouse->button == sf::Mouse::Button::Left)
                        game.reveal(x, y);
                    else if (mouse->button == sf::Mouse::Button::Right)
                        game.toggleFlag(x, y);
                }
            }
        }

        // — draw grid
        window.clear();
        auto& grid = game.getGrid();

        for (int y = 0; y < rows; ++y)
            for (int x = 0; x < cols; ++x)
            {
                const auto& cell = grid[y][x];
                int tu = 0;

                switch (cell.state)
                {
                case Minesweeper::CellState::Hidden:     tu = 0; break;
                case Minesweeper::CellState::Flagged:    tu = 1; break;
                case Minesweeper::CellState::Questioned: tu = 2; break;
                case Minesweeper::CellState::Revealed:
                    tu = cell.hasMine ? 3 : 4 + cell.adjacentMines;
                    break;
                }

                sprite.setTextureRect(
                    sf::IntRect({ tu * tileSize, 0 }, { tileSize, tileSize })
                );
                sprite.setPosition(sf::Vector2f{ float(x * tileSize), float(y * tileSize) });
                window.draw(sprite);
            }

        window.display();
    }

    return 0;
}