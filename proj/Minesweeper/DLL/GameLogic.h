#pragma once
#include "API.h"

#include <vector>

namespace Minesweeper {
    // Possible states of a cell
    enum class CellState {
        Hidden,
        Revealed,
        Flagged,
        Questioned
    };

    // Represents a single cell in the grid
    struct Cell {
        bool hasMine = false;
        unsigned int adjacentMines = 0;
        CellState state = CellState::Hidden;
    };

    // Main game logic class
    class EXPORT_API Game {
    public:
        // Initialize a new game with given dimensions and mine count
        void initialize(unsigned int width, unsigned int height, unsigned int mines);

        // Reveal the cell at (x, y); returns false if a mine was revealed
        bool reveal(unsigned int x, unsigned int y);

        // Toggle flag state on the cell at (x, y)
        void toggleFlag(unsigned int x, unsigned int y);

        // Accessors
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        const std::vector<std::vector<Cell>>& getGrid() const;

        // Check for win condition (all non-mine cells revealed)
        bool checkWin() const;

    private:
        unsigned int width = 0;
        unsigned int height = 0;
        unsigned int mineCount = 0;
        std::vector<std::vector<Cell>> grid;  // 2D grid of cells

        // Helper: counts mines adjacent to (x, y)
        unsigned int countAdjacent(unsigned int x, unsigned int y) const;

        // Helper: recursively reveal neighbors if safe
        void floodFillReveal(unsigned int x, unsigned int y);
    };
}