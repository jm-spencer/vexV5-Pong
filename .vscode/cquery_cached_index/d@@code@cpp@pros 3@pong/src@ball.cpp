#include "main.h"
#include "ball.hpp"
#include "paddle.hpp"
#include <math.h>
#include <iostream>
#include <random>

movingObject::movingObject(lv_style_t* style,
                           const int inital_X,
                           const int inital_Y,
                           const double initial_velocity,
                           const double initial_heading):

                           x(inital_X),
                           y(inital_Y),
                           velocity(initial_velocity),
                           heading(initial_heading)
{
  image = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(image, width, height);

  lv_obj_set_style(image, style);
  lv_obj_refresh_style(image);

  print();
}

double movingObject::increaseVelocity(const double delta){
  velocity += delta;
  return velocity;
}

void movingObject::changeHeading(const double new_heading){
  heading = new_heading;
}

void movingObject::print(){
  lv_obj_set_pos(image, x - (width / 2), y - (height / 2));
}

int movingObject::step(paddle left_paddle, paddle right_paddle, bool* miss){
  const int skew = 5; //necessary to prevent verticle heading

  //step values
  double x_prime = x + (cos(heading) * velocity);
  double y_prime = y - (sin(heading) * velocity);

  //check for collision
  if(y_prime < 2){ //top collision
    y_prime = 4 - y_prime; //reflection over y=2

    heading *= -1; //reflect the heading over y=0 (unit circle)
  }else if(y_prime > 238){ //bottom collision
    y_prime = 476 - y_prime; //reflection over y=188

    heading *= -1; //reflect the heading over y=0 (unit circle)
  }

  if(x_prime < left_paddle.right_position && !(*miss)){ //left paddle
    int intersection_y = ((left_paddle.right_position - x) * tan(heading)) + y;

    if(intersection_y >= left_paddle.top_position && intersection_y <= left_paddle.bottom_position){
      x_prime = (2 * left_paddle.right_position) - x_prime;   //reflection over x=20

      heading = asin(double(left_paddle.top_position + (left_paddle.height / 2) - intersection_y) / ((left_paddle.height / 2) + skew)); //change heading based on where the ball is on the paddle
    }else{
      *miss = true;
    }

  }else if(x_prime > right_paddle.left_position && !(*miss)){
    int intersection_y = ((right_paddle.left_position - x) * tan(heading)) + y;

    if(intersection_y >= right_paddle.top_position && intersection_y <= right_paddle.bottom_position){
      x_prime = (2 * right_paddle.left_position) - x_prime; //reflection over x=455

      heading = M_PI - asin(double(right_paddle.top_position + (right_paddle.height / 2) - intersection_y) / ((right_paddle.height / 2) + skew)); //change heading based on where the ball is on the paddle
    }else{
      *miss = true;
    }
  }

  if(x_prime > 480){
    return 1;
  }else if(x_prime < 0){
    return 2;
  }

  x = x_prime;
  y = y_prime;

  return 0;
}

double get_random_heading(const bool go_right){

  double max = go_right ? M_PI /  3.0 : M_PI * (4.0 / 3.0);
  double min = go_right ? M_PI / -3.0 : M_PI * (2.0 / 3.0);

  return (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
}
