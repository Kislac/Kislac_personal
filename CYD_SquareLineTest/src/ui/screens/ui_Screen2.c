// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: SquareLine_Project_Test2

#include "../ui.h"

void ui_Screen2_screen_init(void)
{
    ui_Screen2 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image1 = lv_image_create(ui_Screen2);
    lv_image_set_src(ui_Image1, &ui_img_1331304740);
    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image1, 65);
    lv_obj_set_y(ui_Image1, 67);
    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_scale(ui_Image1, 150);

    ui_Keyboard1 = lv_keyboard_create(ui_Screen2);
    lv_obj_set_width(ui_Keyboard1, 316);
    lv_obj_set_height(ui_Keyboard1, 120);
    lv_obj_set_x(ui_Keyboard1, 0);
    lv_obj_set_y(ui_Keyboard1, 57);
    lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN);     /// Flags

    ui_TextArea1 = lv_textarea_create(ui_Screen2);
    lv_obj_set_width(ui_TextArea1, 216);
    lv_obj_set_height(ui_TextArea1, 75);
    lv_obj_set_x(ui_TextArea1, -47);
    lv_obj_set_y(ui_TextArea1, -51);
    lv_obj_set_align(ui_TextArea1, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_TextArea1, "Placeholder...");



    ui_Label11 = lv_label_create(ui_Screen2);
    lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label11, -130);
    lv_obj_set_y(ui_Label11, -102);
    lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label11, "Ide Irj");

    ui_Spinbox2 = lv_spinbox_create(ui_Screen2);
    lv_obj_set_width(ui_Spinbox2, 70);
    lv_obj_set_height(ui_Spinbox2, 42);
    lv_obj_set_x(ui_Spinbox2, 114);
    lv_obj_set_y(ui_Spinbox2, -55);
    lv_obj_set_align(ui_Spinbox2, LV_ALIGN_CENTER);
    lv_spinbox_set_digit_format(ui_Spinbox2, 4, 2);
    lv_spinbox_set_range(ui_Spinbox2, 0, 9999);
    lv_spinbox_set_cursor_pos(ui_Spinbox2, 1 - 1);

    lv_obj_add_event_cb(ui_Keyboard1, ui_event_Keyboard1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TextArea1, ui_event_TextArea1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Screen2, ui_event_Screen2, LV_EVENT_ALL, NULL);

}
