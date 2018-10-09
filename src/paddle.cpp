#include "main.h"
#include "paddle.hpp"

paddle::paddle(lv_obj_t* screen_ptr, lv_style_t style, const int initial_X, const int initial_Y){
  lv_obj_t* image = lv_obj_create(screen_ptr, NULL);

  lv_obj_set_size(image, width, height);
  lv_obj_set_pos(image, initial_Y, initial_Y);

  lv_obj_set_style(image, &style);
  lv_obj_refresh_style(image);

  top_position = initial_Y;
  left_position = initial_X;
  right_position = initial_X + width;
  bottom_position = initial_Y + height;
}

void paddle::move(const int translation){

  if(top_position + translation < 2){
    top_position = 2;
  }else if(bottom_position + translation > 238){
    top_position = 188;
  }else{
    top_position -= translation  / control_dampening_value;
  }

  bottom_position = top_position + height;
}

void paddle::print(){
  lv_obj_set_y(image, top_position);
}
