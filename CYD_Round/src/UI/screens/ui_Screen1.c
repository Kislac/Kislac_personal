// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: CYD_Round_SquareLine_Project2

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label1 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "HELLO WORD!");

    ui_Switch1 = lv_switch_create(ui_Screen1);
    lv_obj_set_width(ui_Switch1, 50);
    lv_obj_set_height(ui_Switch1, 25);
    lv_obj_set_x(ui_Switch1, 48);
    lv_obj_set_y(ui_Switch1, 29);
    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);


    ui_Switch2 = lv_switch_create(ui_Screen1);
    lv_obj_set_width(ui_Switch2, 50);
    lv_obj_set_height(ui_Switch2, 25);
    lv_obj_set_x(ui_Switch2, 2);
    lv_obj_set_y(ui_Switch2, -64);
    lv_obj_set_align(ui_Switch2, LV_ALIGN_CENTER);


    ui_Switch3 = lv_switch_create(ui_Screen1);
    lv_obj_set_width(ui_Switch3, 50);
    lv_obj_set_height(ui_Switch3, 25);
    lv_obj_set_x(ui_Switch3, -60);
    lv_obj_set_y(ui_Switch3, 53);
    lv_obj_set_align(ui_Switch3, LV_ALIGN_CENTER);



}
