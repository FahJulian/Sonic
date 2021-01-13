#pragma once
#include <Sonic/Renderer/Color.h>

const float TIME_STEP = 0.25f;

const int PADDING = 50;
const int BORDER = 2;
const int X0 = PADDING + BORDER;
const int Y0 = PADDING + BORDER;

const int CELL_SIZE = 20;

const int COLS = 15;
const int ROWS = 15;

const int WIDTH = COLS * CELL_SIZE;
const int HEIGHT = ROWS * CELL_SIZE;

const int SNAKE_START_LENGTH = 4;
const int SNAKE_START_X = X0 + (COLS / 2) * CELL_SIZE;
const int SNAKE_START_Y = Y0 + (ROWS / 2) * CELL_SIZE;

const int WINDOW_WIDTH = WIDTH + 2 * (BORDER + PADDING);
const int WINDOW_HEIGHT = HEIGHT + 2 * (BORDER + PADDING);
static const char* WINDOW_TITLE = "Snake";

const Sonic::Color BORDER_COLOR = Sonic::Colors::Black;
const Sonic::Color HEAD_COLOR = Sonic::Colors::DarkGreen;
const Sonic::Color TAIL_COLOR = Sonic::Colors::LightGreen;
const Sonic::Color FOOD_COLOR = Sonic::Colors::LightRed;
