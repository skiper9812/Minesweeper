#pragma once
#include "API.h"

#include <vector>
#include <unordered_set>

namespace Minesweeper {

    struct pair_hash {
        std::size_t operator()(const std::pair<unsigned int, unsigned int>& p) const {
            return std::hash<unsigned int>()(p.first) ^ (std::hash<unsigned int>()(p.second) << 1);
        }
    };

    // Possible states of a cell
    enum class CellState { Hidden, Revealed, Flagged, Questioned }; 

    // Represents a single cell in the grid
    struct Cell {
        bool hasMine = false;
        unsigned int adjacentMines = 0;
        CellState state = CellState::Hidden;
    };

    // Main game logic class
    class EXPORT_API Game {
    public:
        // Initialize a new game with given size
        void initialize(unsigned int size);

        // Reveal the cell at (x, y); returns false if a mine was revealed
        bool reveal(unsigned int x, unsigned int y);

        // Toggle flag state on the cell at (x, y)
        void toggleFlag(unsigned int x, unsigned int y);

        // Check for win/lose condition (all non-mine cells revealed)
        bool checkWin() const;
        bool isGameOver() const;
        bool hasEnded() const;

        // Accessors
        const std::vector<std::vector<Cell>>& getGrid() const;

    private:
        std::vector<std::vector<Cell>> grid;  // 2D grid of cells
        unsigned int size = 0, safeParam = 2;
        unsigned int mineCount = 0;
        bool isInitialized = false;
        bool gameOver = false;

        
        std::pair<unsigned int, unsigned int> firstClickPos;

        // Recursively reveal neighbors if safe
        void floodFillReveal(unsigned int x, unsigned int y);

        // Counts mines adjacent to (x, y)
        unsigned int countAdjacent(unsigned int x, unsigned int y) const;

        // Generates Safe Zone based on first click position
        std::unordered_set<std::pair<unsigned int, unsigned int>, pair_hash>
            generateSafeZone(unsigned int startX, unsigned int startY, unsigned int count) const;

        // Place mines based on the Safe Zone
        void placeMines(unsigned int safeX, unsigned int safeY);
    };
}