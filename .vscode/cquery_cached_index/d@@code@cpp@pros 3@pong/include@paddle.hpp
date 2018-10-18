#ifndef PADDLE_HPP_
#define PADDLE_HPP_
#include "main.h"

class paddle{
public:
  lv_obj_t* image;

  const int width = 5;
  const int height = 50;
  const double control_dampening_value = -0.04;

  int top_position;
  int left_position;
  int right_position;
  int bottom_position;

  paddle(lv_style_t* style, const int initial_X, const int initial_Y);

  void move(const int translation);
  void print();
};

#endif //header guard
