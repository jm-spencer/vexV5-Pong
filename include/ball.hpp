#ifndef BALL_HPP_
#define BALL_HPP_
#include "main.h"
#include "paddle.hpp"

class movingObject{
  const int width = 6;
  const int height = 6;

  lv_obj_t* image;
  double x;
  double y;
  double velocity;
  double heading;

public:
  movingObject(lv_obj_t* screen_ptr, lv_style_t style, const int inital_X, const int inital_Y, const double initial_velocity, const double initial_heading);

  double increaseVelocity(const double delta);
  void changeHeading(const double new_heading);
  void print();
  int iterate(paddle left_paddle, paddle right_paddle, bool* miss);
};

double get_random_heading(const bool go_right);

#endif
