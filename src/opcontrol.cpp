#include "main.h"
#include "ball.hpp"
#include "paddle.hpp"
#include <string>

/*
* vexV5-Pong
* Made by Joseph from VRC 5588D
* Calvert Hall Robotics
*/

void opcontrol() {
  //config
  constexpr double initial_speed = 4.5;   //initial speed of the ball
  constexpr double acceleration = 0.003;  //speed of the ball increased by this amount every frame
  constexpr int win_score = 7;            //after one player has this many wins, the game will end

  //initialization
  pros::Controller main_controller(pros::E_CONTROLLER_MASTER);
  main_controller.set_text(2, 8, "0  /  0");
    //display empty scoreboard on newly initialized controller

  int left_score = 0, right_score = 0;
    //initialize a variable to hold each player's score

  //styles

  //define all the styles
  lv_style_t screen_style;
  lv_style_t line_style;
  lv_style_t left_paddle_style;
  lv_style_t right_paddle_style;
  lv_style_t ball_style;


  //set the screen's background color to black
  lv_style_copy(&screen_style, &lv_style_plain_color);
  screen_style.body.main_color = LV_COLOR_BLACK;
  screen_style.body.grad_color = LV_COLOR_BLACK;

  //set the middle line's color to gray
  lv_style_copy(&line_style, &lv_style_plain_color);
  line_style.body.main_color = LV_COLOR_GRAY;
  line_style.body.grad_color = LV_COLOR_GRAY;

  //set left_paddle's color to red
  lv_style_copy(&left_paddle_style, &lv_style_plain_color);
  left_paddle_style.body.main_color = LV_COLOR_RED;
  left_paddle_style.body.grad_color = LV_COLOR_RED;

  //set right_paddle's color to blue
  lv_style_copy(&right_paddle_style, &lv_style_plain_color);
  right_paddle_style.body.main_color = LV_COLOR_BLUE;
  right_paddle_style.body.grad_color = LV_COLOR_BLUE;

  //set the ball's color to white, and make it circular
  lv_style_copy(&ball_style, &lv_style_plain_color);
  ball_style.body.main_color = LV_COLOR_WHITE;
  ball_style.body.grad_color = LV_COLOR_WHITE;
  ball_style.body.radius = LV_RADIUS_CIRCLE;

  //initialize screen
  lv_obj_t* scr = lv_obj_create(NULL, NULL);
  lv_scr_load(scr);

  //apply style made previously
  lv_obj_set_style(scr, &screen_style);

  while(left_score < win_score && right_score < win_score){
    //while both scores are less than the win condition, run another round

    int reset = 0;
    //reset denotes the ball has gone of screen, and the ball animation should stop
    //it also tells which side has won


    //initialize lvgl objects

    //middle line
    lv_obj_t* middle_line = lv_obj_create(scr, NULL); //create the middle line on the screen
    lv_obj_set_size(middle_line, 6, 240);             //6 pixels wide, 240 tall
    lv_obj_set_pos(middle_line, 237, 0);              //set in middle of screen (240 - width/2)

    lv_obj_set_style(middle_line, &line_style);       //give it the previously defined color

    //paddles
    paddle left_paddle(&left_paddle_style, 20, 95);   //middle of the screen, 20 pixels to the left
    paddle right_paddle(&right_paddle_style, 455, 95);//middle of the screen, 20 pixles from the right (minus 5 due to width)

    //ball
    ball ball_1(&ball_style, 240, 120, initial_speed, get_random_heading(left_score < right_score));
        //start in the middle of the screen
        //speed starts at the previously defined initial_speed
        //heading is randomly generated, and goes in the direction of the current winner
        //untested, but theoretically, multiple ball objects could run in a single game

    pros::delay(1000);
        //slight delay so players can prepare; if it did not exist can cause some printing issues

    while(!reset){
      //terminate when reset is set with the winner

      //control
      left_paddle.move(
        main_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));

      right_paddle.move(
        main_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

      //iterate
      reset = ball_1.step(left_paddle, right_paddle);
      //given paddles to detect collisions, and moves the ball one frame forward

      //Print updated objects to the screen
      left_paddle.print();
      right_paddle.print();
      ball_1.print();

      //increase speed by previously defined acceleration
      ball_1.increaseSpeed(acceleration);
      pros::delay(16);
    }
    //post - round cleanup

    //read reset and increment the appropriate score
    if(reset == 1){
      ++left_score;
    }else if(reset == 2){
      ++right_score;
    }

    //generate a string and update the scoreboard
    const char* scoreboard = (std::to_string(left_score) + "  /  " + std::to_string(right_score)).c_str();
    main_controller.set_text(2, 8, scoreboard);

    //delete all the children of the screen to prevent any memory leaking
    lv_obj_clean(scr);
  }
  //post - game

  //delay to prevent error from printing to frequently to the screen
  pros::delay(100);

  //Print heading and delay again for the aforementioned reason
  main_controller.set_text(0, 4, "The winner is");
  pros::delay(100);

  //Print appropriate winner
  main_controller.set_text(1, 9, left_score > right_score ? " Left" : "Right");

  //stall execution, as opcontrol() should never exit itself
  while(true){
    pros::delay(100);
  }
}
