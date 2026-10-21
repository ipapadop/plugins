#include "flood_fill.h"

static int fill(uint8_t grid[GRID_ROWS][GRID_COLS], int row, int col,
                uint8_t old_color, uint8_t new_color)
{
    if (row < 0 || row >= GRID_ROWS || col < 0 || col >= GRID_COLS) {
        return 0;
    }
    if (grid[row][col] != old_color) {
        return 0;
    }
    grid[row][col] = new_color;
    return 1 + fill(grid, row + 1, col, old_color, new_color)
             + fill(grid, row - 1, col, old_color, new_color)
             + fill(grid, row, col + 1, old_color, new_color)
             + fill(grid, row, col - 1, old_color, new_color);
}

int flood_fill(uint8_t grid[GRID_ROWS][GRID_COLS], int row, int col,
               uint8_t new_color)
{
    uint8_t old_color;

    if (row < 0 || row >= GRID_ROWS || col < 0 || col >= GRID_COLS) {
        return -1;
    }
    old_color = grid[row][col];
    if (old_color == new_color) {
        return 0;
    }
    return fill(grid, row, col, old_color, new_color);
}
