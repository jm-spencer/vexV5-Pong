#ifndef BALL_HPP_
#define BALL_HPP_
#include "main.h"
#include "paddle.hpp"

class ball{
  //boolean to denote when to stop doing calculations on the ball
  bool missed;

public:
  //declate size of the object
  const int width = 6;
  const int height = 6;

  //handle to the lvgl object
  lv_obj_t* image;

  //attributes for calculation
  double x;
  double y;
  double speed;
  double heading;

  //contructor
  ball(lv_style_t* style, const int inital_X, const int inital_Y, const double initial_velocity, const double initial_heading);

  //increment movingObject::speed by delta
  double increaseSpeed(const double delta);

  //overwrite movingObject::heading with new_heading
  void changeHeading(const double new_heading);

  //print the object to the brain's screen
  void print();

  //calculate the balls position one step forward in space, compensate for collisions, and determine the winner
  int step(paddle left_paddle, paddle right_paddle);
};

//utility for generating a random heading for a ball object
double get_random_heading(const bool go_right);

#endif
