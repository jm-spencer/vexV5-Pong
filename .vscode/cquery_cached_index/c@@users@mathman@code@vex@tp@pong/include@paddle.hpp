#ifndef PADDLE_HPP_
#define PADDLE_HPP_
#include "main.h"

class paddle{
public:
  //declate size of the object
  const int width = 5;
  const int height = 50;

  //constant for adjusting control of the paddle
  const double control_dampening_value = -0.04;

  //handle to the lvgl object
  lv_obj_t* image;

  //attributes for calculation
  int top_position;
  int left_position;
  int right_position;
  int bottom_position;

  //constructor
  paddle(lv_style_t* style, const int initial_X, const int initial_Y);

  //move the paddle by translation * control_dampening_value
  void move(const int translation);

  //print the object to the brain's screen
  void print();
};

#endif //header guard
