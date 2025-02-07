#pragma once

#include <raylib.h>

constexpr const char* WINDOW_TITLE = "Client";

constexpr Color TEXT_COLOR = { 0, 0, 0, 255 };
constexpr Color UI_ELEMENT_BACKGROUND_COLOR = { 50, 50, 50, 255 };
constexpr Color UI_ELEMENT_HOVER_COLOR = { 75, 75, 75, 255 };
constexpr Color UI_ELEMENT_FOCUS_COLOR = { 100, 100, 100, 255 };
constexpr Color UI_ELEMENT_BORDER_COLOR = { 20, 20, 20, 255 };
constexpr int UI_ELEMENT_BORDER_SIZE = 2;

constexpr int FONT_SIZE = 16;

constexpr int INPUT_WIDTH = 250;
constexpr int INPUT_HEIGHT = 35;

constexpr int BUTTON_WIDTH = 200;
constexpr int BUTTON_HEIGHT = 35;
constexpr Color BUTTON_TEXT_COLOR = { 255, 255, 255, 255 };

constexpr float PARTY_ENTY_ABS_MARGIN = 100.f;