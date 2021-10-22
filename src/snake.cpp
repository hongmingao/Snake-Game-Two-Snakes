#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died. 
  if (the_other_snake == nullptr) {alive = true;}
  else {alive = CheckDead(current_head_cell) ? false : true;}
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

bool Snake::CheckOwnBody(SDL_Point &current_head_cell) // true if head in own body, else false 
{
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      return true;
    }
  }
  return false;
}

bool Snake::CheckOtherBody(SDL_Point &current_head_cell) // true if head in own body, else false 
{
  if (the_other_snake == nullptr) {return false;} // game_snake doesn't check itself or player snake
  if (current_head_cell.x == static_cast<int>(the_other_snake->head_x) 
   && current_head_cell.y == static_cast<int>(the_other_snake->head_y) )
  { return true; }
  for (auto const &item : the_other_snake->body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      return true;
    }
  }
  return false;
}

bool Snake::CheckDead(SDL_Point &current_head_cell)
{
  return CheckOwnBody(current_head_cell) || CheckOtherBody(current_head_cell);
}

void Snake::AutoDirection(SDL_Point* food_ptr)
{
  if (static_cast<int>(head_y) != food_ptr->y) {
    direction = (static_cast<int>(head_y) > food_ptr->y) ? Direction::kUp : Direction::kDown;
  } else {
    direction = (static_cast<int>(head_x) > food_ptr->x) ? Direction::kLeft : Direction::kRight;
  }
}