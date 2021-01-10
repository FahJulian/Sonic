#pragma once

const int SNAKE_ELEMENT_SIZE = 20;
const float TIME_STEP = 0.5f;
const int ROWS = 20;
const int COLS = 20;
const int PADDING = 40;
const int BORDER_THICKNESS = 3;
const int BORDER_COLOR = 0x000000ff;
const int SNAKE_HEAD_COLOR = 0x084d08ff;
const int SNAKE_BODY_COLOR = 0x12db13ff;
const int FOOD_COLOR = 0xf42828ff;
const int INTIAL_SNAKE_SIZE = 4;

const int WIDTH = COLS * SNAKE_ELEMENT_SIZE + 2 * (PADDING + BORDER_THICKNESS);
const int HEIGHT = ROWS * SNAKE_ELEMENT_SIZE + 2 * (PADDING + BORDER_THICKNESS);


static const char* TITLE = "Snake";
