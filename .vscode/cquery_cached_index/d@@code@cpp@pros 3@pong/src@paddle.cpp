#include "main.h"
#include "paddle.hpp"

paddle::paddle(lv_style_t* style, const int initial_X, const int initial_Y){
  image = lv_obj_create(lv_scr_act(), NULL);

  lv_obj_set_size(image, width, height);
  lv_obj_set_pos(image, initial_X, initial_Y);

  lv_obj_set_style(image, style);
  lv_obj_refresh_style(image);

  top_position = initial_Y;
  left_position = initial_X;
  right_position = initial_X + width;
  bottom_position = initial_Y + height;
}

void paddle::print(){
  lv_obj_set_y(image, top_position);
}

void paddle::move(const int translation){
  int delta = (double)translation * control_dampening_value;

  if(top_position + delta < 2){
    top_position = 2;
  }else if(bottom_position + delta > 238){
    top_position = 238 - height;
  }else{
    top_position += delta;
  }

  bottom_position = top_position + height;
}
