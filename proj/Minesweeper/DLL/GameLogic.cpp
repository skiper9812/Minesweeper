#include "GameLogic.h"
#include <random>
#include <iostream>
#include <set>
#include <queue>

namespace Minesweeper {

    void Game::initialize(unsigned int s) {
        size = s;
        grid.assign(size, std::vector<Cell>(size));
        isInitialized = false;  // Wait for first click
    }

    std::unordered_set<std::pair<unsigned int, unsigned int>, pair_hash>
        Game::generateSafeZone(unsigned int startX, unsigned int startY, unsigned int count) const {
        std::unordered_set<std::pair<unsigned int, unsigned int>, pair_hash> safeZone;
        std::queue<std::pair<unsigned int, unsigned int>> q;

        q.push({ startX, startY });
        safeZone.insert({ startX, startY });

        while (!q.empty() && safeZone.size() < count) {
            auto [x, y] = q.front(); q.pop();

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = static_cast<int>(x) + dx;
                    int ny = static_cast<int>(y) + dy;
                    if (dx == 0 && dy == 0) continue;
                    if (nx >= 0 && ny >= 0 && nx < static_cast<int>(size) && ny < static_cast<int>(size)) {
                        std::pair<unsigned int, unsigned int> pos = { (unsigned int)nx, (unsigned int)ny };
                        if (!safeZone.count(pos)) {
                            safeZone.insert(pos);
                            q.push(pos);
                            if (safeZone.size() >= count) break;
                        }
                    }
                }
                if (safeZone.size() >= count) break;
            }
        }

        if (safeZone.size() != count)
            throw std::runtime_error("Failed to create a safe zone.");

        return safeZone;
    }

    void Game::placeMines(unsigned int safeX, unsigned int safeY) {
        // 1. Generate safe zone
        auto safeZone = generateSafeZone(safeX, safeY, safeParam);

        // 2. Also forbid placing mines around the safe zone
        std::unordered_set<std::pair<unsigned int, unsigned int>, pair_hash> forbidden;
        for (const auto& [x, y] : safeZone) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = static_cast<int>(x) + dx;
                    int ny = static_cast<int>(y) + dy;
                    if (nx >= 0 && ny >= 0 && nx < static_cast<int>(size) && ny < static_cast<int>(size)) {
                        forbidden.insert({ (unsigned int)nx, (unsigned int)ny });
                    }
                }
            }
        }

        // 3. Place mines outside the forbidden area
        mineCount = static_cast<unsigned int>(size * size * 0.175);
        unsigned int placed = 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distX(0, size - 1);
        std::uniform_int_distribution<> distY(0, size - 1);

        while (placed < mineCount) {
            unsigned int x = distX(gen);
            unsigned int y = distY(gen);
            std::pair<unsigned int, unsigned int> pos = { x, y };

            if (!grid[y][x].hasMine && forbidden.count(pos) == 0) {
                grid[y][x].hasMine = true;
                ++placed;
            }
        }

        // 4. Debug output
        std::cout << "\nMinefield Map (Debug View):\n";
        for (unsigned int y = 0; y < size; ++y) {
            for (unsigned int x = 0; x < size; ++x) {
                std::cout << (grid[y][x].hasMine ? " *" : " .");
            }
            std::cout << '\n';
        }

        // 5. Count adjacent mines
        for (unsigned int y = 0; y < size; ++y) {
            for (unsigned int x = 0; x < size; ++x) {
                grid[y][x].adjacentMines = countAdjacent(x, y);
            }
        }

        // 6. Reveal safe zone
        for (const auto& [x, y] : safeZone) {
            floodFillReveal(x, y);
        }

        isInitialized = true;
    }

    unsigned int Game::countAdjacent(unsigned int x, unsigned int y) const {
        unsigned int count = 0;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                int nx = static_cast<int>(x) + dx;
                int ny = static_cast<int>(y) + dy;
                if (nx >= 0 && nx < static_cast<int>(size) && ny >= 0 && ny < static_cast<int>(size)) {
                    if (grid[ny][nx].hasMine)
                        ++count;
                }
            }
        }
        return count;
    }

    void Game::floodFillReveal(unsigned int x, unsigned int y) {
        // Base cases: out of bounds or already revealed or flagged
        if (x >= size || y >= size) return;
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
                    if (nx >= 0 && nx < static_cast<int>(size) && ny >= 0 && ny < static_cast<int>(size)) {
                        floodFillReveal(static_cast<unsigned int>(nx), static_cast<unsigned int>(ny));
                    }
                }
            }
        }
    }

    bool Game::reveal(unsigned int x, unsigned int y) {
        if (!isInitialized) {
            placeMines(x, y);
        }

        if (x >= size || y >= size) return true; // ignore out of bounds
        Cell& cell = grid[y][x];
        if (cell.state == CellState::Revealed || cell.state == CellState::Flagged) return true;

        // If it's a mine, game over
        if (cell.hasMine) {
            cell.state = CellState::Revealed;
            gameOver = true;
            return false;
        }

        // Flood fill reveal
        floodFillReveal(x, y);
        return true;
    }

    void Game::toggleFlag(unsigned int x, unsigned int y) {
        if (x >= size || y >= size) return;
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
        return (revealedCount == (size * size - mineCount));
    }

    bool Game::isGameOver() const { 
        return gameOver;
    }

    bool Game::hasEnded() const {
        return gameOver || checkWin();
    }

} // namespace Minesweeper
