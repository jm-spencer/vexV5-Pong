#include "main.h"
#include "ball.hpp"
#include <string>

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  //config
  constexpr int paddle_dampening_value = -30;//must be negative
  constexpr double acceleration = 0.0005;

  pros::Controller controller(pros::E_CONTROLLER_MASTER);
  int left_delta, right_delta;


  int left_score = 0, right_score = 0;

  while(true){
    int left_paddle_pos = 95, right_paddle_pos = 95;
    bool missed = false;
    int reset = 0;

    //initialize screen and labels
    //screen
    lv_obj_t* scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);

    //left paddle
    lv_obj_t* left_paddle = lv_obj_create(scr, NULL);
    lv_obj_set_size(left_paddle, 5, 50);
    lv_obj_set_pos(left_paddle, 20, 95);

    lv_style_t left_paddle_style;
    lv_style_copy(&left_paddle_style, &lv_style_plain_color);
    lv_color_t left_color;
    left_color.red = 0xFF;
    left_color.green = 0x00;
    left_color.blue = 0x00;
    left_paddle_style.body.main_color = left_color;
    left_paddle_style.body.grad_color = left_color;

    lv_obj_set_style(left_paddle, &left_paddle_style);
    lv_obj_refresh_style(left_paddle);

    //right paddle
    lv_obj_t* right_paddle = lv_obj_create(scr, left_paddle);
    lv_obj_set_pos(right_paddle, 455, 95);

    lv_style_t right_paddle_style;
    lv_style_copy(&right_paddle_style, &lv_style_plain_color);
    lv_color_t right_color;
    right_color.red = 0x00;
    right_color.green = 0x00;
    right_color.blue = 0xFF;
    right_paddle_style.body.main_color = right_color;
    right_paddle_style.body.grad_color = right_color;

    lv_obj_set_style(right_paddle, &right_paddle_style);
    lv_obj_refresh_style(right_paddle);

    //ball
    lv_obj_t* ball_image = lv_obj_create(scr, NULL);
    lv_obj_set_size(ball_image, 5, 5);

    lv_style_t ball_style;
    lv_style_copy(&ball_style, &lv_style_plain_color);
    lv_color_t ball_color;
    ball_color.red = 0xFF;
    ball_color.green = 0xFF;
    ball_color.blue = 0xFF;
    ball_style.body.main_color = ball_color;
    ball_style.body.grad_color = ball_color;

    lv_obj_set_style(ball_image, &ball_style);
    lv_obj_refresh_style(ball_image);

    movingObject ball(ball_image, 240, 120, 5, M_PI_4);

    //scoreboard left
    //lv_obj_t* scoreboard_left = lv_obj_create(scr, NULL);
    //lv_label_set_text(scoreboard_left, std::to_string(left_score).c_str());

    controller.print(1, 6, "%i", left_score);

    pros::delay(50);

    while(!reset){
      //control
      left_delta = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / paddle_dampening_value;
      right_delta = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / paddle_dampening_value;


      if(left_paddle_pos + left_delta < 2){
        left_paddle_pos = 2;
      }else if(left_paddle_pos + left_delta > 188){
        left_paddle_pos = 188;
      }else{
        left_paddle_pos += left_delta;
      }

      if(right_paddle_pos + right_delta < 2){
        right_paddle_pos = 2;
      }else if(right_paddle_pos + right_delta > 188){
        right_paddle_pos = 188;
      }else{
        right_paddle_pos += right_delta;
      }

      reset = ball.iterate(left_paddle_pos, right_paddle_pos, &missed);

      //update screen
      lv_obj_set_y(left_paddle, left_paddle_pos);
      lv_obj_set_y(right_paddle, right_paddle_pos);
      ball.print();

      ball.increaseVelocity(acceleration);
      pros::delay(16);
    }

    if(reset == 1){
      ++left_score;
    }else if(reset == 2){
      ++right_score;
    }

    lv_obj_clean(scr);
  }
}
