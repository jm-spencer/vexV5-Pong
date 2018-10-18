#include "main.h"
#include "ball.hpp"
#include "paddle.hpp"
#include <string>
#include <iostream>

//Made by Joseph from VRC 5588D
//Calvert Hall Robotics

void opcontrol() {
  //config
  constexpr double initial_speed = 4.5;
  constexpr double acceleration = 0.003;
  constexpr int win_score = 7;

  //initialization
  pros::Controller main_controller(pros::E_CONTROLLER_MASTER);
  main_controller.set_text(2, 8, "0  /  0");

  int left_delta, right_delta;

  int left_score = 0, right_score = 0;

  //styles
  lv_style_copy(&screen_style, &lv_style_plain_color);
  lv_color_t black;
  black.red = 0x00;
  black.green = 0x00;
  black.blue = 0x00;
  screen_style.body.main_color = black;
  screen_style.body.grad_color = black;

  lv_style_copy(&line_style, &lv_style_plain_color);
  lv_color_t gray;
  gray.red = 0xB0;
  gray.green = 0xB0;
  gray.blue = 0xB0;
  line_style.body.main_color = gray;
  line_style.body.grad_color = gray;

  lv_style_copy(&left_paddle_style, &lv_style_plain_color);
  lv_color_t red;
  red.red = 0xFF;
  red.green = 0x00;
  red.blue = 0x00;
  left_paddle_style.body.main_color = red;
  left_paddle_style.body.grad_color = red;

  lv_style_copy(&right_paddle_style, &lv_style_plain_color);
  lv_color_t blue;
  blue.red = 0x00;
  blue.green = 0x00;
  blue.blue = 0xFF;
  right_paddle_style.body.main_color = blue;
  right_paddle_style.body.grad_color = blue;

  lv_style_copy(&ball_style, &lv_style_plain_color);
  lv_color_t white;
  white.red = 0xFF;
  white.green = 0xFF;
  white.blue = 0xFF;
  ball_style.body.main_color = white;
  ball_style.body.grad_color = white;
  ball_style.body.radius = LV_RADIUS_CIRCLE;

  //initialize screen
  lv_obj_t* scr = lv_obj_create(NULL, NULL);
  lv_scr_load(scr);

  lv_obj_set_style(scr, &screen_style);
  lv_obj_refresh_style(scr);

  while(left_score < win_score && right_score < win_score){

    bool missed = false;
    int reset = 0;

    //initialize lvgl objects

    //middle line
    lv_obj_t* middle_line = lv_obj_create(scr, NULL);
    lv_obj_set_size(middle_line, 6, 240); //6 pixels wide, 240 tall
    lv_obj_set_pos(middle_line, 237, 0);  //set in middle of screen

    lv_obj_set_style(middle_line, &line_style); //set color
    lv_obj_refresh_style(middle_line);

    //paddles
    paddle left_paddle(&left_paddle_style, 20, 95);   //middle of the screen, 20 pixels to the left
    paddle right_paddle(&right_paddle_style, 455, 95);//middle of the screen, 20 pixles from the right

    //ball
    movingObject ball(&ball_style, 240, 120, initial_speed, get_random_heading(left_score < right_score));
        //start in the middle of the screen

    pros::delay(1000);

    while(!reset){
      //control
      left_paddle.move(
        main_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));

      right_paddle.move(
        main_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

      //iterate
      reset = ball.step(left_paddle, right_paddle, &missed);
      //

      //Update Screen
      left_paddle.print();
      right_paddle.print();
      ball.print();

      ball.increaseVelocity(acceleration);
      pros::delay(16);
    }
    //post - round cleanup

    if(reset == 1){
      ++left_score;
    }else if(reset == 2){
      ++right_score;
    }

    const char* scoreboard = (std::to_string(left_score) + "  /  " + std::to_string(right_score)).c_str();
    main_controller.set_text(2, 8, scoreboard);

    lv_obj_clean(scr);
  }
  //post - game
  pros::delay(100);

  main_controller.set_text(0, 4, "The winner is");
  pros::delay(100);

  main_controller.set_text(1, 9, left_score > right_score ? " Left" : "Right");

  while(true){
    pros::delay(100);
  }
}
