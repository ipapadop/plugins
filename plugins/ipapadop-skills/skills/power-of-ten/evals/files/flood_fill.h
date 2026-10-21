#ifndef FLOOD_FILL_H
#define FLOOD_FILL_H

#include <stdint.h>

#define GRID_ROWS 32
#define GRID_COLS 32

/* Repaints the 4-connected region that contains (row, col) with new_color.
 * Returns the number of cells repainted, or -1 if (row, col) is outside the
 * grid. */
int flood_fill(uint8_t grid[GRID_ROWS][GRID_COLS], int row, int col,
               uint8_t new_color);

#endif
