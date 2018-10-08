#include "main.h"
#include "ball.hpp"
#include <math.h>
#include <iostream>
#include <random>

movingObject::movingObject(lv_obj_t* image_ptr,
                           const int initalX,
                           const int initalY,
                           const double initialVelocity,
                           const double initialHeading):

                           image(image_ptr),
                           x(initalX),
                           y(initalY),
                           velocity(initialVelocity),
                           heading(initialHeading)
{
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
  lv_obj_set_pos(image, x, y);
}

int movingObject::getY(){
  return y;
}

int movingObject::getX(){
  return x;
}

int movingObject::iterate(int left_paddle, int right_paddle, bool* miss){
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

  if(x_prime < 25 && !(*miss)){ //left paddle
    int intersection_y = ((25 - x) * tan(heading)) + y;

    if(intersection_y >= left_paddle - 3 && intersection_y <= left_paddle + 53){
      x_prime = 50 - x_prime;   //reflection over x=20

      heading = asin(double(left_paddle + 25 - intersection_y) / 30); //change heading based on where the ball is on the paddle
    }else{
      *miss = true;
    }

  }else if(x_prime > 455 && !(*miss)){
    int intersection_y = ((455 - x) * tan(heading)) + y;

    if(intersection_y >= right_paddle - 3 && intersection_y <= right_paddle + 53){
      x_prime = 910 - x_prime; //reflection over x=460

      heading = M_PI - asin(double(right_paddle + 25 - intersection_y) / 30); //change heading based on where the ball is on the paddle
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

double get_random_heading(const bool goRight){

  double max = goRight ? M_PI /  3.0 : M_PI * (4.0 / 3.0);
  double min = goRight ? M_PI / -3.0 : M_PI * (2.0 / 3.0);

  return (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
}
