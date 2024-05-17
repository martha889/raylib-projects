#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 1200;

#define ROWS 100
#define COLS 100
#define ROW_DIFF (SCREEN_HEIGHT / ROWS)
#define COL_DIFF (SCREEN_WIDTH / COLS)
// +1 as we also consider the first edge of the window (r-1) + 1
#define GRID_ROW_SIZE ROWS
#define GRID_COL_SIZE COLS

#define BACKGROUND_COLOR BLUE
#define GRID_LINE_COLOR BLACK
#define GRID_BLOCK_COLOR GREEN

typedef struct {
  // store the x and y positions of lines in the grid
  // this is also the top-left corner of the squares in the grid
  int rows[GRID_ROW_SIZE];
  int cols[GRID_COL_SIZE];
  bool alive[GRID_ROW_SIZE][GRID_COL_SIZE];
} Grid;

void print_grid(bool alive_grid[GRID_ROW_SIZE][GRID_COL_SIZE]) {
  for (int i=0; i<GRID_ROW_SIZE; i++) {
    for (int j=0; j<GRID_COL_SIZE; j++) {
      printf("%d ", alive_grid[i][j]);
    }
    printf("\n");
  }
}

void init_liveness(Grid* grid) {
  // only used once at the beginning

  for (int i=0; i<GRID_ROW_SIZE; i++) {
    for (int j=0; j<GRID_COL_SIZE; j++) {
      grid->alive[i][j] = false;
    }
  }
}

double get_random_number() {
  return (double) rand() / (double) RAND_MAX;
}

// ADD PATTERNS TO THE GRID
// 1. Still lifes
void add_block(int x, int y, Grid* grid) {
  grid->alive[y][x] = true;
  grid->alive[y+1][x] = true;
  grid->alive[y][x+1] = true;
  grid->alive[y+1][x+1] = true;
}

// 2. Oscillators
void add_blinker(int x, int y, Grid* grid) {
  grid->alive[y][x] = true;
  grid->alive[y][x+1] = true;
  grid->alive[y][x+2] = true;
}

// 3. Spaceships
void add_glider(int x, int y, Grid* grid) {
  grid->alive[y+1][x+3] = true;
  grid->alive[y+2][x+1] = true;
  grid->alive[y+2][x+3] = true;
  grid->alive[y+3][x+2] = true;
  grid->alive[y+3][x+3] = true;
}

// 4. Glider guns
void add_gosper_glider_gun(int x, int y, Grid* grid) {
  // add gosper glider gun; (x, y) are the coordinates of the top-left corner of this pattern

  grid->alive[y+5][x+1] = true;
  grid->alive[y+5][x+2] = true;
  grid->alive[y+6][x+1] = true;
  grid->alive[y+6][x+2] = true;

  grid->alive[y+5][x+11] = true;
  grid->alive[y+6][x+11] = true;
  grid->alive[y+7][x+11] = true;

  grid->alive[y+4][x+12] = true;
  grid->alive[y+8][x+12] = true;

  grid->alive[y+3][x+13] = true;
  grid->alive[y+3][x+14] = true;
  grid->alive[y+9][x+13] = true;
  grid->alive[y+9][x+14] = true;

  grid->alive[y+6][x+15] = true;
  grid->alive[y+4][x+16] = true;
  grid->alive[y+8][x+16] = true;
  
  grid->alive[y+5][x+17] = true;
  grid->alive[y+6][x+17] = true;
  grid->alive[y+7][x+17] = true;

  grid->alive[y+6][x+18] = true;

  grid->alive[y+5][x+21] = true;
  grid->alive[y+5][x+22] = true;
  grid->alive[y+4][x+21] = true;
  grid->alive[y+4][x+22] = true;
  grid->alive[y+3][x+21] = true;
  grid->alive[y+3][x+22] = true;

  grid->alive[y+2][x+23] = true;
  grid->alive[y+6][x+23] = true;
  grid->alive[y+2][x+25] = true;
  grid->alive[y+1][x+25] = true;
  grid->alive[y+6][x+25] = true;
  grid->alive[y+7][x+25] = true;

  grid->alive[y+3][x+35] = true;
  grid->alive[y+3][x+36] = true;
  grid->alive[y+4][x+35] = true;
  grid->alive[y+4][x+36] = true;

}

// ---------------------------
void set_random_liveness(Grid* grid) {
  srand(time(0));
  for (int i=0; i<GRID_ROW_SIZE; i++) {
    for (int j=0; j<GRID_COL_SIZE; j++) {
      double k = get_random_number();
      if (k > 0.5) {
        grid->alive[i][j] = true;
      }
    }
  }
}

void next_grid(Grid* grid) {
  // set liveness value for each iteration after calculating the number of neighbours of each cell
  bool new_alive[GRID_ROW_SIZE][GRID_COL_SIZE];
  for (int i=0; i<GRID_ROW_SIZE; i++) {
    for (int j=0; j<GRID_COL_SIZE; j++) {
      new_alive[i][j] = false;
    }
  }
  int row_diff[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int col_diff[] = {-1, 0, 1, 1, -1, -1, 0, 1};


  for (int i=0; i<GRID_ROW_SIZE; i++) {
    for (int j=0; j<GRID_COL_SIZE; j++) {
      int live_neighbours = 0;
      for (int k=0; k<8; k++) {
        int n_row = i + row_diff[k];
        int n_col = j + col_diff[k];

        if (n_row < GRID_ROW_SIZE && n_row >= 0 && n_col < GRID_COL_SIZE && n_col >= 0 && grid->alive[n_row][n_col]) {
          live_neighbours += 1;
        }

      }

      if (grid->alive[i][j]) {
        // live cell
        if (live_neighbours < 2) {
          new_alive[i][j] = false;
        } else if (live_neighbours <= 3) {
          new_alive[i][j] = true;
        } else {
          new_alive[i][j] = false;
        }
      } else {
        if (live_neighbours == 3) {
          new_alive[i][j] = true;
        }
      }
      
    }
  }
  memcpy(grid->alive, new_alive, sizeof(new_alive));
}

// compute the y coordinates of rows; store them inside grid
// compute the x coordinates of columns; store them inside grid
void compute_grid(Grid* grid) {

  for (int i=0; i<GRID_ROW_SIZE; i++) {
    grid->rows[i] = i * ROW_DIFF;
  }

  for (int i=0; i<GRID_COL_SIZE; i++) {
    grid->cols[i] = i * COL_DIFF;
  }
}

void draw_grid(Grid* grid) {
  for (int i=0; i<ROWS; i++) {
    DrawLine(0, grid->rows[i], SCREEN_WIDTH, grid->rows[i], GRID_LINE_COLOR);
  }
  
  for (int i=0; i<COLS; i++) {
    DrawLine(grid->cols[i], 0, grid->cols[i], SCREEN_HEIGHT, GRID_LINE_COLOR);
  }
}

void draw_cells(Grid* grid) {
  for (int i=0; i<GRID_ROW_SIZE; i++) {
    for (int j=0; j<GRID_COL_SIZE; j++) {
      if (grid->alive[i][j]) {
        DrawRectangle(grid->cols[j], grid->rows[i], COL_DIFF, ROW_DIFF, GRID_BLOCK_COLOR);
      }
    }
  }
}


int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game-of-life");
  SetTargetFPS(16);
  Grid *grid = malloc(sizeof(Grid));
  init_liveness(grid);
  // set_random_liveness(grid);
  compute_grid(grid);
  // set_glider(grid);
  add_gosper_glider_gun(0, 0, grid);
  add_gosper_glider_gun(0, 30, grid);
  // add_glider(40, 40, grid);
  // add_blinker(60, 60, grid);
  // add_block(80, 80, grid);
  while (!WindowShouldClose()) {
    // do something
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    // printf("Mouse (%d, %d)\n", GetMouseX(), GetMouseY());

    draw_cells(grid);
    draw_grid(grid);
    next_grid(grid);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
