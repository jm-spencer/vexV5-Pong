#include "main.h"
#include "ball.hpp"
#include <string>

void opcontrol() {
  //config
  constexpr int paddle_dampening_value = -30;//must be negative
  constexpr double initialSpeed = 5;
  constexpr double acceleration = 0.001;

  //initialization

  pros::Controller mainController(pros::E_CONTROLLER_MASTER);
  int left_delta, right_delta;

  int left_score = 0, right_score = 0;

  //styles

  lv_style_t screen_style;
  lv_style_copy(&screen_style, &lv_style_plain_color);
  lv_color_t black;
  black.red = 0x00;
  black.green = 0x00;
  black.blue = 0x00;
  screen_style.body.main_color = black;
  screen_style.body.grad_color = black;

  lv_style_t left_paddle_style;
  lv_style_copy(&left_paddle_style, &lv_style_plain_color);
  lv_color_t red;
  red.red = 0xFF;
  red.green = 0x00;
  red.blue = 0x00;
  left_paddle_style.body.main_color = red;
  left_paddle_style.body.grad_color = red;

  lv_style_t right_paddle_style;
  lv_style_copy(&right_paddle_style, &lv_style_plain_color);
  lv_color_t blue;
  blue.red = 0x00;
  blue.green = 0x00;
  blue.blue = 0xFF;
  right_paddle_style.body.main_color = blue;
  right_paddle_style.body.grad_color = blue;

  lv_style_t ball_style;
  lv_style_copy(&ball_style, &lv_style_plain_color);
  lv_color_t white;
  white.red = 0xFF;
  white.green = 0xFF;
  white.blue = 0xFF;
  ball_style.body.main_color = white;
  ball_style.body.grad_color = white;

  //initialize screen
  lv_obj_t* scr = lv_obj_create(NULL, NULL);
  lv_scr_load(scr);

  lv_obj_set_style(scr, &screen_style);
  lv_obj_refresh_style(scr);

  while(left_score < 10 && right_score < 10){
    int left_paddle_pos = 95, right_paddle_pos = 95;
    bool missed = false;
    int reset = 0;

    //initialize lvgl objects

    //left paddle
    lv_obj_t* left_paddle = lv_obj_create(scr, NULL);
    lv_obj_set_size(left_paddle, 5, 50);
    lv_obj_set_pos(left_paddle, 20, 95);

    lv_obj_set_style(left_paddle, &left_paddle_style);
    lv_obj_refresh_style(left_paddle);

    //right paddle
    lv_obj_t* right_paddle = lv_obj_create(scr, left_paddle);
    lv_obj_set_pos(right_paddle, 455, 95);

    lv_obj_set_style(right_paddle, &right_paddle_style);
    lv_obj_refresh_style(right_paddle);

    //ball
    lv_obj_t* ball_image = lv_obj_create(scr, NULL);
    lv_obj_set_size(ball_image, 5, 5);

    lv_obj_set_style(ball_image, &ball_style);
    lv_obj_refresh_style(ball_image);

    movingObject ball(ball_image, 240, 120, initialSpeed, getRandomHeading(left_score > right_score));

    /*
    //scoreboard
    lv_obj_t* scoreboardParent = lv_label_create(scr, scr);
    lv_obj_set_size(scoreboardParent, 120, 90);

    //scoreboard left
    lv_obj_t* scoreboard_left = lv_label_create(scoreboardParent, NULL);
    char* left_text = std::to_string(left_score).c_str();
    lv_label_set_text(scoreboard_left, left_text);

    scoreboard right
    lv_obj_t* scoreboard_right = lv_label_create(scoreboardParent, NULL);
    char* right_text = std::to_string(right_score).c_str();
    lv_label_set_text(scoreboard_right, right_text);
    */

    const char* scoreboard = (std::to_string(left_score) + "  " + std::to_string(right_score)).c_str();
    mainController.set_text(2, 6, scoreboard);

    pros::delay(1000);

    while(!reset){
      //control
      left_delta = mainController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / paddle_dampening_value;
      right_delta = mainController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / paddle_dampening_value;


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

    switch(reset){
      case 1:
        ++left_score;
        break;
      case 2:
        ++right_score;
        break;
    }

    mainController.clear();
    lv_obj_clean(scr);
  }
  lv_obj_del(scr);
}
