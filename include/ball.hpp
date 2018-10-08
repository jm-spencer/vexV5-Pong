#ifndef BALL_HPP_
#define BALL_HPP_
#include "main.h"

class movingObject{
  lv_obj_t* image;
  double x;
  double y;
  double velocity;
  double heading;

public:
  movingObject(lv_obj_t* image_ptr, const int initalX, const int initalY, const double initialVelocity, const double initialHeading);

  double increaseVelocity(const double delta);
  void changeHeading(const double new_heading);
  void print();
  int getX();
  int getY();
  int iterate(int left_paddle, int right_paddle, bool* miss);
};

double get_random_heading(const bool goRight);

#endif
