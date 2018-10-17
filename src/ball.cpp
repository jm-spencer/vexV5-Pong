#include "main.h"
#include "ball.hpp"
#include "paddle.hpp"
#include <math.h>
#include <random>

ball::ball(lv_style_t* style,
                           const int inital_X,
                           const int inital_Y,
                           const double initial_velocity,
                           const double initial_heading):

                           x(inital_X),
                           y(inital_Y),
                           speed(initial_velocity),
                           heading(initial_heading)
{
  image = lv_obj_create(lv_scr_act(), NULL);    //initalize the lvgl object as a child of the screen
  lv_obj_set_size(image, width, height);        //set the size of the image to the definitions in ball.hpp

  lv_obj_set_style(image, style);               //set the style (color) of the paddle to whatever is passed as the parameter

  print();                                      //display initial state
  missed = false;                               //missed, essentially being a latch, must be given the correct starting state
}

double ball::increaseSpeed(const double delta){
  speed += delta;                               //increase speed by delta (acceleration const in opcontroll.cpp)
  return speed;                                 //return speed in the event it is needed
}

void ball::changeHeading(const double new_heading){
  heading = new_heading;                        //overwrite heading
}

void ball::print(){
  //x and y refer to the middle of the image, while the function below takes in
  //coordinates of the top left corner. The line below translates it accordingly
  lv_obj_set_pos(image, x - (width / 2), y - (height / 2));
}

int ball::step(paddle left_paddle, paddle right_paddle){
  const int skew = 5; //necessary to prevent verticle heading

  //step values
  double x_prime = x + (cos(heading) * speed);
  double y_prime = y - (sin(heading) * speed);

  //check for collision
  if(y_prime < 2){ //top collision
    y_prime = 4 - y_prime; //reflection over y=2

    heading *= -1; //reflect the heading over a verticle line
  }else if(y_prime > 238){ //bottom collision
    y_prime = 476 - y_prime; //reflection over y=188

    heading *= -1; //reflect the heading over a verticle line
  }

  if(x_prime < left_paddle.right_position && !missed){
    //if the next step of the ball crosses the line where the (left) paddle is, the program must check for a collision

    int intersection_y = ((left_paddle.right_position - x) * tan(heading)) + y;
    //intersection_y is the Y value of point C, where the ball's trajectory crosses the line where the paddle can exist

    if(intersection_y >= left_paddle.top_position && intersection_y <= left_paddle.bottom_position){
      //if intersection_y is within the threshhold of the paddle, a collision has occured

      x_prime = (2 * left_paddle.right_position) - x_prime;
      //reflection over the right side of the paddle

      heading = asin((double)(left_paddle.top_position + (left_paddle.height / 2) - intersection_y) / ((left_paddle.height / 2) + skew));
      /*the above line is a gives a new heading based on where the ball intersects with the (left) paddle
      * Given a circle with radius paddle::height + skew, and whose center B lays on the
      * perpendicular bisector of the paddle (which intersects the paddle at point A),
      * point C, the point of intersection between the ball and the paddle, lays on the circle.
      * The formula returns the value of the angle ABC.
      */

    }else{
      //if a collision did not occur, mark the ball as missed so the ball will not recalculate the collision
      missed = true;
    }

  }else if(x_prime > right_paddle.left_position && !missed){
    //if the next step of the ball crosses the line where the (left) paddle is, the program must check for a collision

    int intersection_y = ((right_paddle.left_position - x) * tan(heading)) + y;
    //intersection_y is the Y value of point C, where the ball's trajectory crosses the line where the paddle can exist

    if(intersection_y >= right_paddle.top_position && intersection_y <= right_paddle.bottom_position){
      //if intersection_y is within the threshhold of the paddle, a collision has occured

      x_prime = (2 * right_paddle.left_position) - x_prime;
      //reflection over the left side of the paddle

      heading = M_PI - asin((double)(right_paddle.top_position + (right_paddle.height / 2) - intersection_y) / ((right_paddle.height / 2) + skew));
      //See line 73
      //the heading is reflected horizontally, as the ball is now traveling left

    }else{
      //if a collision did not occur, mark the ball as missed so the ball will not recalculate the collision
      missed = true;
    }
  }

  if(x_prime > 480){
    //if the ball goes off the right side of the screen, reset miss and notify the main execution

    missed = false;
    return 1;
  }else if(x_prime < 0){
    //if the ball goes off the left side of the screen, reset miss and notify the main execution

    missed = false;
    return 2;
  }

  //if the ball will still be on the screen, update the position of the ball
  x = x_prime;
  y = y_prime;

  //return 0 to notify no abnormal events have occured
  return 0;
}

double get_random_heading(const bool go_right){
  /*
  * the following ternary statements select the proper bounds based on the boolean passed
  * if go_right is true, the bounds are [-pi/3, pi/3] or [-60,60] degrees
  * if go_right is false, the bounds are [4pi/3, 2pi/3] or [120, 240] degrees
  */

  double max = go_right ? M_PI /  3.0 : M_PI * (4.0 / 3.0);
  double min = go_right ? M_PI / -3.0 : M_PI * (2.0 / 3.0);

  return (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
  //return a random value between the given bounds
}
