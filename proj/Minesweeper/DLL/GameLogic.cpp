#include "GameLogic.h"
#include <random>

namespace Minesweeper {

    void Game::initialize(unsigned int w, unsigned int h, unsigned int mines) {
        width = w;
        height = h;
        mineCount = mines;

        // Resize grid to height rows of width columns
        grid.assign(height, std::vector<Cell>(width));

        // Place mines randomly
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distX(0, width - 1);
        std::uniform_int_distribution<> distY(0, height - 1);

        unsigned int placed = 0;
        while (placed < mineCount) {
            unsigned int x = distX(gen);
            unsigned int y = distY(gen);
            if (!grid[y][x].hasMine) {
                grid[y][x].hasMine = true;
                ++placed;
            }
        }

        // Compute adjacent mine counts
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                grid[y][x].adjacentMines = countAdjacent(x, y);
            }
        }
    }

    unsigned int Game::countAdjacent(unsigned int x, unsigned int y) const {
        unsigned int count = 0;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                int nx = static_cast<int>(x) + dx;
                int ny = static_cast<int>(y) + dy;
                if (nx >= 0 && nx < static_cast<int>(width) && ny >= 0 && ny < static_cast<int>(height)) {
                    if (grid[ny][nx].hasMine)
                        ++count;
                }
            }
        }
        return count;
    }

    void Game::floodFillReveal(unsigned int x, unsigned int y) {
        // Base cases: out of bounds or already revealed or flagged
        if (x >= width || y >= height) return;
        Cell& cell = grid[y][x];
        if (cell.state != CellState::Hidden) return;

        // Reveal this cell
        cell.state = CellState::Revealed;

        // If no adjacent mines, recursively reveal neighbors
        if (cell.adjacentMines == 0 && !cell.hasMine) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = static_cast<int>(x) + dx;
                    int ny = static_cast<int>(y) + dy;
                    if (nx >= 0 && nx < static_cast<int>(width) && ny >= 0 && ny < static_cast<int>(height)) {
                        floodFillReveal(static_cast<unsigned int>(nx), static_cast<unsigned int>(ny));
                    }
                }
            }
        }
    }

    bool Game::reveal(unsigned int x, unsigned int y) {
        if (x >= width || y >= height) return true; // ignore out of bounds
        Cell& cell = grid[y][x];
        if (cell.state == CellState::Revealed || cell.state == CellState::Flagged) return true;

        // If it's a mine, game over
        if (cell.hasMine) {
            cell.state = CellState::Revealed;
            return false;
        }

        // Flood fill reveal
        floodFillReveal(x, y);
        return true;
    }

    void Game::toggleFlag(unsigned int x, unsigned int y) {
        if (x >= width || y >= height) return;
        Cell& cell = grid[y][x];
        if (cell.state == CellState::Hidden) {
            cell.state = CellState::Flagged;
        }
        else if (cell.state == CellState::Flagged) {
            cell.state = CellState::Questioned;
        }
        else if (cell.state == CellState::Questioned) {
            cell.state = CellState::Hidden;
        }
    }

    unsigned int Game::getWidth() const {
        return width;
    }

    unsigned int Game::getHeight() const {
        return height;
    }

    const std::vector<std::vector<Cell>>& Game::getGrid() const {
        return grid;
    }

    bool Game::checkWin() const {
        unsigned int revealedCount = 0;
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                if (!cell.hasMine && cell.state == CellState::Revealed)
                    ++revealedCount;
            }
        }
        // Win if all non-mine cells are revealed
        return (revealedCount == (width * height - mineCount));
    }

} // namespace Minesweeper
