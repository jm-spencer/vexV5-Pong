#include "main.h"
#include "paddle.hpp"

paddle::paddle(lv_style_t* style, const int initial_X, const int initial_Y){
  image = lv_obj_create(lv_scr_act(), NULL);    //initalize the lvgl object as a child of the screen

  lv_obj_set_size(image, width, height);        //set the size of the image to the definitions in paddle.hpp
  lv_obj_set_pos(image, initial_X, initial_Y);  //set the position of the object to the values passed as parameters

  lv_obj_set_style(image, style);               //set the style (color) of the paddle to whatever is passed as the parameter

  top_position = initial_Y;                     //top line is inline with the graph y=top_position
  left_position = initial_X;                    //left line is inline with the graph x=left_position
  right_position = initial_X + width;           //right line is inline with the graph x=right_position
  bottom_position = initial_Y + height;         //bottom line is inline with the graph y=bottom_position
}

void paddle::print(){
  //update position
  lv_obj_set_y(image, top_position);
}

void paddle::move(const int translation){
  //scale input by control_dampening_value
  int delta = (double)translation * control_dampening_value;

  //scale position by delta, bounded by the top at y=2, and at the bottom at y=238
  if(top_position + delta < 2){
    top_position = 2;
  }else if(bottom_position + delta > 238){
    top_position = 238 - height;
  }else{
    top_position += delta;
  }

  //update bottom_position
  bottom_position = top_position + height;
}
