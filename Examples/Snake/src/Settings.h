#pragma once

// Settings
const int ELEMENT_SIZE = 20;
const float TIME_STEP = 0.3f;
const int ROWS = 16;
const int COLS = 16;
const int PADDING = 40;
const int BORDER = 3;
const int BORDER_COLOR = 0x000000ff;
const int SNAKE_HEAD_COLOR = 0x084d08ff;
const int SNAKE_BODY_COLOR = 0x12db13ff;
const int FOOD_COLOR = 0xf42828ff;
const int INTIAL_SNAKE_SIZE = 4;
static const char* TITLE = "Snake";



// Sizes calculated from settings
const int GRID_X0 = PADDING + BORDER;
const int GRID_Y0 = PADDING + BORDER;
const int GRID_WIDTH = COLS * ELEMENT_SIZE;
const int GRID_HEIGHT = ROWS * ELEMENT_SIZE;
const int SNAKE_START_X = GRID_X0 + COLS / 2 * ELEMENT_SIZE;
const int SNAKE_START_Y = GRID_Y0 + ROWS / 2 * ELEMENT_SIZE;

const int WIDTH = COLS * ELEMENT_SIZE + 2 * (PADDING + BORDER);
const int HEIGHT = ROWS * ELEMENT_SIZE + 2 * (PADDING + BORDER);


